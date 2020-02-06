static occa::kernel axKernel;

void loadAxKernel(occa::device device, char *threadModel,
                  std::string arch, std::string kernelName,
                  int N, dlong Nelements){

  int rank = 1;
#ifdef USEMPI
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#endif

  const int Nq = N + 1;
  const int Np = Nq*Nq*Nq;
  occa::env::OCCA_MEM_BYTE_ALIGN = USE_OCCA_MEM_BYTE_ALIGN;
 
  occa::properties props;
  props["defines"].asObject();
  props["includes"].asArray();
  props["header"].asArray();
  props["flags"].asObject();

  props["defines/p_Nq"] = Nq;
  props["defines/p_Np"] = Np;

  props["defines/p_Nggeo"] = p_Nggeo;
  props["defines/p_G00ID"] = p_G00ID;
  props["defines/p_G01ID"] = p_G01ID;
  props["defines/p_G02ID"] = p_G02ID;
  props["defines/p_G11ID"] = p_G11ID;
  props["defines/p_G12ID"] = p_G12ID;
  props["defines/p_G22ID"] = p_G22ID;
  props["defines/p_GWJID"] = p_GWJID;

  props["defines/dfloat"] = dfloatString;
  props["defines/dlong"]  = dlongString;

  props["okl"] = false;

  std::string filename = "BK5" + arch;
  for (int r=0;r<2;r++){
    if ((r==0 && rank==0) || (r==1 && rank>0)) {
      if(strstr(threadModel, "NATIVE+CUDA")){
        axKernel = device.buildKernel(filename + ".cu", kernelName, props);
        axKernel.setRunDims(Nelements, Nq*Nq);
      } else if(strstr(threadModel, "NATIVE+SERIAL")){
        props["defines/USE_OCCA_MEM_BYTE_ALIGN"] = USE_OCCA_MEM_BYTE_ALIGN;
        axKernel = device.buildKernel(filename + ".c", kernelName, props);
      } else { // fallback is okl
        props["okl"] = true;
        axKernel = device.buildKernel(filename + ".okl", kernelName, props);
      }
    }
#ifdef USEMPI
    MPI_Barrier(MPI_COMM_WORLD);
#endif
  }
}

void runAxKernel(dlong Nelements, occa::memory o_ggeo, occa::memory o_DrV, dfloat lambda, 
                 occa::memory o_q, occa::memory o_Aq, int assembled = 0){

    if(assembled) {
/*
      axKernel(NglobalGatherElements, o_globalGatherElementList, o_ggeo, o_DrV, 
               lambda, o_q, o_Aq);
      ogsGatherScatterStart(o_Aq, ogsDfloat, ogsAdd, ogs);
      axKernel(NlocalGatherElements, o_localGatherElementList, o_ggeo, o_DrV, 
               lambda, o_q, o_Aq);
      ogsGatherScatterFinish(o_Aq, ogsDfloat, ogsAdd, ogs);
*/
      std::cout << "ERROR: assembled version not implemented yet!\n";
      exit(1);
    } else {
      axKernel(Nelements, o_ggeo, o_DrV, lambda, o_q, o_Aq);
    }
}
