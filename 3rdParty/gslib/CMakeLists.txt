project(gslib)
# # #set(gslib VERSION 0.1)
#add_subdirectory(src)

set(gslib-src
  src/gs.c 
  src/sort.c 
  src/sarray_transfer.c 
  src/sarray_sort.c 
  src/gs_local.c 
  src/fail.c 
  src/crystal.c
  src/comm.c 
  src/tensor.c
  src/fcrystal.c
  src/findpts_local.c
  src/obbox.c
  src/poly.c
  src/lob_bnd.c
  src/findpts_el_3.c
  src/findpts_el_2.c
)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMPI -DUSEMPIIO -DUNDERSCORE -DGLOBAL_LONG_LONG")
add_library(gs STATIC ${gslib-src})
target_link_libraries(gs PUBLIC MPI::MPI_C)
install(TARGETS gs DESTINATION lib)