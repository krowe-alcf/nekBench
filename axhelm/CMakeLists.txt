project(axhelm)
set(axhelm VERSION 0.1)

add_executable(axhelm main.cpp ${CMAKE_SOURCE_DIR}/core/meshBasis.cpp)
target_include_directories(axhelm PUBLIC ${CMAKE_SOURCE_DIR}/core)
target_link_libraries(axhelm PUBLIC
  MPI::MPI_C 
  libocca
  ${BLAS_LIBRARIES} 
  ${LAPACK_LIBRARIES}
)

install(TARGETS axhelm RUNTIME DESTINATION bin)
install(DIRECTORY kernel/ DESTINATION kernel)