#!/bin/bash
CC=mpicc
CFLAGS=

CXX=mpicxx
CXXFLAGS=

FC=mpifort
FFLAGS=

MPICC=mpicc
MPICXX=mpicxx
MPIFC=mpifort

#OpenCL_LIBRARY=/soft/libraries/public_opencl/opencl_19.47.14903/usr/lib64
#OpenCL_LIBRARY=/soft/libraries/public_opencl/opencl_loader/ocl-icd/libOpenCL.so
#OpenCL_LIBRARY=/soft/libraries/khronos/loader/master-26Mar20/lib64/libOpenCL.so.1
#OpenCL_INCLUDE_DIRS=/soft/libraries/OpenCL-Headers

INSTALL_DIR=`pwd`/install

mkdir -p build
mkdir -p install

#cmake .... && make -j && make install
cd build

cmake -j4 .. -DCMAKE_INSTALL_PREFIX="${INSTALL_DIR}" \
   -DCMAKE_C_COMPILER="${CC}" \
   -DCMAKE_C_FLAGS="${CFLAGS}" \
   -DCMAKE_CXX_COMPILER="${CXX}" \
   -DCMAKE_CXX_FLAGS="${CXXFLAGS}" \
   -DCMAKE_Fortran_COMPILER="${FC}" \
   -DCMAKE_Fortran_FLAGS="${FFLAGS}" \
   -DMPI_C_COMPILER="${MPICC}" \
   -DMPI_CXX_COMPILER="${MPICXX}" \
   -DMPI_Fortran_COMPILER="${MPIFC}" \
   -DALLOW_MPI="ON" \
   -DALLOW_CUDA="OFF" \
   -DALLOW_METAL="OFF" \
   -DALLOW_OPENCL="OFF" \
   && make -j4 && make install