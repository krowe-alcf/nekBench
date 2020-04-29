#!/bin/bash
set -x
#-----
CC=gcc
CFLAGS=

CXX=g++
CXXFLAGS=

FC=gfortran
FFLAGS=

MPICC=mpicc
MPICXX=mpicxx
MPIFC=mpifort

ALLOW_OPENCL=ON
OpenCL_LIBRARY=/soft/libraries/khronos/loader/master-26Mar20/lib64/libOpenCL.so.1
OpenCL_INCLUDE_DIR=/soft/libraries/khronos/headers/master-26Mar20/include

INSTALL_DIR=`pwd`/install
#-----

cmake \
   -DCMAKE_C_COMPILER:STRING="${CC}" \
   -DCMAKE_C_FLAGS:STRING="${CFLAGS}" \
   -DCMAKE_CXX_COMPILER:STRING="${CXX}" \
   -DCMAKE_CXX_FLAGS:STRING="${CXXFLAGS}" \
   -DCMAKE_Fortran_COMPILER:STRING="${FC}" \
   -DCMAKE_Fortran_FLAGS:STRING="${FFLAGS}" \
   -DMPI_C_COMPILER:STRING="${MPICC}" \
   -DMPI_CXX_COMPILER:STRING="${MPICXX}" \
   -DMPI_Fortran_COMPILER:STRING="${MPIFC}" \
   -DALLOW_MPI:BOOL="OFF" \
   -DALLOW_CUDA:BOOL="OFF" \
   -DALLOW_METAL:BOOL="OFF" \
   -DALLOW_OPENCL:BOOL="${ALLOW_OPENCL}" \
   -DOpenCL_LIBRARY:PATH="${OpenCL_LIBRARY}" \
   -DOpenCL_INCLUDE_DIR:PATH="${OpenCL_INCLUDE_DIR}" \
   -S . -B build

cmake --build build --parallel 4

mkdir -p ${INSTALL_DIR}
cmake --install build/3rdParty/occa --prefix ${INSTALL_DIR}/occa
cmake --install build/axhelm --prefix ${INSTALL_DIR}
cmake --install build/nekBone --prefix ${INSTALL_DIR}
