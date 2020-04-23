project(nekBench)
set(nekBench VERSION 0.1)
cmake_minimum_required(VERSION 3.0)

enable_language(C)
enable_language(CXX)
enable_language(Fortran)
enable_testing()
include(GNUInstallDirs)
include(FortranCInterface)

option(USE_MPI "Use the MPI library." ON)
if(USE_MPI)
    find_package(MPI)
endif()

option(USE_VENDOR_BLASLAPACK "Use an external vendor-provided Blas/Lapack library" OFF)
#if(USE_VENDOR_BLASLAPACK)
#endif()
find_package(BLAS)
find_package(LAPACK)

#TODO: Add build options to use OpenCL, CUDA, etc.

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/include)

add_compile_definitions(
    "OCCA_VERSION_1_0"
    "USE_OCCA_MEM_BYTE_ALIGN=64"
    "dfloat=double"
    "dfloatString=\"double\""
    "dfloatFormat=\"%lf\""
    "dlong=int"
    "dlongString=\"int\""
    "dlongFormat=\"%d\""
    "hlong=long long int"
    "hlongString=\"long long int\""
    "hlongFormat=\"%lld\""
    "MPI_DFLOAT=MPI_DOUBLE"
    "MPI_DLONG=MPI_INT"
    "MPI_HLONG=MPI_LONG_LONG_INT"
) 

add_subdirectory(3rdParty)
add_subdirectory(axhelm)
add_subdirectory(nekBone)