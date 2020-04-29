#If OCCA wasn't found, then build it now.
add_subdirectory(occa)

#If BLAS/LAPACK wasn't found, then build it now.
#...

add_subdirectory(gslib)
add_subdirectory(ogs)
