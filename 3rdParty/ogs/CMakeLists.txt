project(ogslib)

set(ogslib-src
  src/ogsHostGatherScatter.c
  src/ogsHostGatherScatterVec.c
  src/ogsHostGatherScatterMany.c
  src/ogsHostGather.c
  src/ogsHostGatherVec.c
  src/ogsHostGatherMany.c
  src/ogsHostScatter.c
  src/ogsHostScatterVec.c
  src/ogsHostScatterMany.c
  src/ogsHostSetup.c
  src/ogsGatherScatter.cpp
  src/ogsGatherScatterVec.cpp
  src/ogsGatherScatterMany.cpp
  src/ogsGather.cpp
  src/ogsGatherVec.cpp
  src/ogsGatherMany.cpp
  src/ogsScatter.cpp
  src/ogsScatterVec.cpp
  src/ogsScatterMany.cpp
  src/ogsSetup.cpp
  src/ogsKernels.cpp 
)

add_library(ogs STATIC ${ogslib-src})
target_include_directories(ogs PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/include 
)
target_link_libraries(ogs PUBLIC MPI::MPI_C libocca gs)
set_target_properties(ogs PROPERTIES PUBLIC_HEADER ogs.hpp)

target_compile_definitions(ogs PUBLIC 
  "USE_NEW_PINNED_MALLOC=1"
  "DOGS=\"${PROJECT_SOURCE_DIR}\""
)
