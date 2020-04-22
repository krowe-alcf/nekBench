project(nekBench)
set(nekBench VERSION 0.1)
cmake_minimum_required(VERSION 3.0)

enable_language(C)
enable_language(CXX)
enable_language(Fortran)
enable_testing()
include(GNUInstallDirs)
include(FortranCInterface)

set(libraries "")
#- Set preprocessor flag depending on the compiler
#set(Fortran_FPP_FLAG -cpp)
#set(CMAKE_Fortran_FLAGS "${Fortran_FPP_FLAG} ${CMAKE_Fortran_FLAGS}")

option(USE_MPI "Use the MPI library." ON)
if(USE_MPI)
    find_package(MPI)
    #add_compile_definitions(USE_MPI=TRUE)
    #set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -DUSE_MPI=TRUE")
    #set(libraries ${libraries} MPI::MPI_Fortran)
endif()

option(USE_VENDOR_BLASLAPACK "Use an external vendor-provided Blas/Lapack library" OFF)
#if(USE_VENDOR_BLASLAPACK)
#endif()

#TODO: Add build options to use OpenCL, CUDA, etc.

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/include)

add_compile_definitions(
    "dfloat=double"
    "dfloatString=\"double\""
    "dfloatFormat=\"%lf\""
    "dlong=int"
    "dlongString=\"int\""
    "dlongFormat=\"%d\""
    "hlong=long long int"
    "hlongString=\"long long int\""
    "hlongFormat=\"%lld\""
    "USE_OCCA_MEM_BYTE_ALIGN=64"
) 

add_subdirectory(3rdParty)
#add_subdirectory(axhelm)
#add_subdirectory(nekBone)