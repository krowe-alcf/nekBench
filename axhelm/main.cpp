/*

The MIT License (MIT)

Copyright (c) 2017 Tim Warburton, Noel Chalmers, Jesse Chan, Ali Karakus

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mpi.h"
#ifdef _OPENMP
  #include "omp.h"
#endif

#include "occa.hpp"
#include "meshBasis.hpp"
#include "kernelHelper.cpp"
#include "axhelmReference.cpp"


static occa::kernel axKernel;

dfloat *drandAlloc(int N){

  dfloat *v = (dfloat*) calloc(N, sizeof(dfloat));

  for(int n=0;n<N;++n){
    v[n] = drand48();
  }

  return v;
}

int main(int argc, char **argv){

  if(argc<6){
    printf("Usage: ./axhelm N Ndim numElements [NATIVE|OKL]+SERIAL|CUDA|OPENCL CPU|VOLTA [nRepetitions] [kernelVersion]\n");
    return 1;
  }

  int rank = 0, size = 1;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  const int N = atoi(argv[1]);
  const int Ndim = atoi(argv[2]);
  const dlong Nelements = atoi(argv[3]);
  std::string threadModel;
  threadModel.assign(strdup(argv[4]));

  std::string arch("");
  if(argc>=6)
    arch.assign(argv[5]);

  int Ntests = 100;
  if(argc>=7)
    Ntests = atoi(argv[6]);

  int kernelVersion = 0;
  if(argc>=8)
    kernelVersion = atoi(argv[7]);

  const int deviceId = 0;
  const int platformId = 0;

  const int Nq = N+1;
  const int Np = Nq*Nq*Nq;
  const dfloat lambda = 1.1;
  
  const dlong offset = Nelements*Np;

  const int assembled = 0;

  // build element nodes and operators
  dfloat *rV, *wV, *DrV;
  meshJacobiGQ(0,0,N, &rV, &wV);
  meshDmatrix1D(N, Nq, rV, &DrV);

  // build device
  occa::device device;
  char deviceConfig[BUFSIZ];

  if(strstr(threadModel.c_str(), "CUDA")){
    sprintf(deviceConfig, "mode: 'CUDA', device_id: %d",deviceId);
  }
  else if(strstr(threadModel.c_str(),  "HIP")){
    sprintf(deviceConfig, "mode: 'HIP', device_id: %d",deviceId);
  }
  else if(strstr(threadModel.c_str(),  "OPENCL")){
    sprintf(deviceConfig, "mode: 'OpenCL', device_id: %d, platform_id: %d", deviceId, platformId);
  }
  else if(strstr(threadModel.c_str(),  "OPENMP")){
    sprintf(deviceConfig, "mode: 'OpenMP' ");
  }
  else{
    sprintf(deviceConfig, "mode: 'Serial' ");
  #ifdef _OPENMP
    omp_set_num_threads(1);
  #endif
  }
  
  int Nthreads = 0;
#ifdef _OPENMP
  Nthreads = omp_get_max_threads();
#endif

  std::string deviceConfigString(deviceConfig);
  device.setup(deviceConfigString);
  occa::env::OCCA_MEM_BYTE_ALIGN = USE_OCCA_MEM_BYTE_ALIGN;

  if(rank==0) {
   std::cout << "word size: " << sizeof(dfloat) << " bytes\n";
   std::cout << "active occa mode: " << device.mode() << "\n";
  }

  // load kernel
  std::string kernelName = "axhelm";
  if(assembled) kernelName = "axhelm_partial"; 
  if(Ndim > 1) kernelName += "_n" + std::to_string(Ndim);
  kernelName += "_v" + std::to_string(kernelVersion);
  axKernel = loadAxKernel(device, threadModel, arch, kernelName, N, Nelements);

  // populate device arrays
  dfloat *ggeo = drandAlloc(Np*Nelements*p_Nggeo);
  dfloat *q    = drandAlloc((Ndim*Np)*Nelements);
  dfloat *Aq   = drandAlloc((Ndim*Np)*Nelements);

  occa::memory o_ggeo  = device.malloc(Np*Nelements*p_Nggeo*sizeof(dfloat), ggeo);
  occa::memory o_q     = device.malloc((Ndim*Np)*Nelements*sizeof(dfloat), q);
  occa::memory o_Aq    = device.malloc((Ndim*Np)*Nelements*sizeof(dfloat), Aq);
  occa::memory o_DrV   = device.malloc(Nq*Nq*sizeof(dfloat), DrV);

  // warm up
  axKernel(Nelements, offset, o_ggeo, o_DrV, lambda, o_q, o_Aq);

  // check for correctness
  for(int n=0;n<Ndim;++n){
    dfloat *x = q + n*offset;
    dfloat *Ax = Aq + n*offset; 
    axhelmReference(Nq, Nelements, lambda, ggeo, DrV, x, Ax);
  }
  o_Aq.copyTo(q);
  dfloat maxDiff = 0;
  for(int n=0;n<Ndim*Np*Nelements;++n){
    dfloat diff = fabs(q[n]-Aq[n]);
    maxDiff = (maxDiff<diff) ? diff:maxDiff;
  }
  MPI_Allreduce(MPI_IN_PLACE, &maxDiff, 1, MPI_DFLOAT, MPI_SUM, MPI_COMM_WORLD);
  if (rank==0)
    std::cout << "Correctness check: maxError = " << maxDiff << "\n";

  // run kernel
  device.finish();
  MPI_Barrier(MPI_COMM_WORLD);
  const double start = MPI_Wtime();

  for(int test=0;test<Ntests;++test)
    axKernel(Nelements, offset, o_ggeo, o_DrV, lambda, o_q, o_Aq);

  device.finish();
  MPI_Barrier(MPI_COMM_WORLD);
  const double elapsed = (MPI_Wtime() - start)/Ntests;

  // print statistics
  const dfloat GDOFPerSecond = (size*Ndim*(N*N*N)*Nelements/elapsed)/1.e9;
  const long long bytesMoved = (Ndim*2*Np+7*Np)*sizeof(dfloat); // x, Mx, opa
  const double bw = (size*bytesMoved*Nelements/elapsed)/1.e9;
  double flopCount = Ndim*Np*12*Nq;
  if(Ndim == 1) flopCount += 18*Np;
  if(Ndim == 3) flopCount += 57*Np;
  double gflops = (flopCount*size*Nelements/elapsed)/1.e9;
  if(rank==0) {
    std::cout << "MPItasks=" << size
              << " OMPthreads=" << Nthreads
              << " NRepetitions=" << Ntests
              << " Ndim=" << Ndim
              << " N=" << N
              << " Nelements=" << size*Nelements
              << " elapsed time=" << elapsed
              << " GDOF/s=" << GDOFPerSecond
              << " GB/s=" << bw
              << " GFLOPS/s=" << gflops
              << "\n";
  } 

  MPI_Finalize();
  exit(0);
}
