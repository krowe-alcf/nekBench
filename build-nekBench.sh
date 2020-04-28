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

BUILD_DIR=build
INSTALL_DIR=`pwd`/install
#-----
mkdir -p $BUILD_DIR
mkdir -p $INSTALL_DIR

cd $BUILD_DIR

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
   -DALLOW_MPI="OFF" \
   -DALLOW_CUDA="OFF" \
   -DALLOW_METAL="OFF" \
   -DALLOW_OPENCL="${ALLOW_OPENCL}" \
   -DOpenCL_LIBRARY="${OpenCL_LIBRARY}" \
   -DOpenCL_INCLUDE_DIR="${OpenCL_INCLUDE_DIR}" \
   && make -j4 && make install