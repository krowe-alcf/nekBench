add_subdirectory(occa)
add_subdirectory(gslib)
#add_subdirectory(ogs)

#if(NOT ${USE_VENDOR_BLASLAPACK})
#add_subdirectory(BlasLapack)
#endif()

# set(gslib-src
#   gslib/src/comm.c
#   gslib/src/gs.c 
#   gslib/src/sort.c 
#   gslib/src/sarray_transfer.c 
#   gslib/src/sarray_sort.c 
#   gslib/src/gs_local.c 
#   gslib/src/fail.c 
#   gslib/src/crystal.c 
#   gslib/src/tensor.c
#   gslib/src/fcrystal.c
#   gslib/src/findpts_local.c
#   gslib/src/obbox.c
#   gslib/src/poly.c
#   gslib/src/lob_bnd.c
#   gslib/src/findpts_el_3.c
#   gslib/src/findpts_el_2.c)

# add_library(gslib STATIC ${gslib-src})
# target_link_libraries(gslib PUBLIC MPI::MPI_C)
# target_include_directories(gslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/gslib/src)
# install(TARGETS gslib DESTINATION ${CMAKE_INSTALL_LIBDIR})
