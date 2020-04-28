project(nekBone)
set(nekBone VERSION 0.1)

set(nekBone-src
  main.cpp
  setup.cpp
  solve.cpp
  ${CMAKE_SOURCE_DIR}/core/timer.cpp
  ${CMAKE_SOURCE_DIR}/core/meshBasis.cpp
  ${CMAKE_SOURCE_DIR}/core/meshSetup.cpp
  ${CMAKE_SOURCE_DIR}/core/setupAide.cpp
)

add_executable(nekBone ${nekBone-src})
target_include_directories(nekBone PUBLIC ${CMAKE_SOURCE_DIR}/core)
target_link_libraries(nekBone PUBLIC 
  MPI::MPI_C 
  libocca 
  ogs
  ${BLAS_LIBRARIES} 
  ${LAPACK_LIBRARIES}
)

target_compile_definitions(nekBone PUBLIC "DBP=\"./\"")

install(TARGETS nekBone RUNTIME DESTINATION bin)
install(DIRECTORY kernel/ DESTINATION kernel)
install(FILES "setup.ini" DESTINATION "./" RENAME "nekBone.ini")