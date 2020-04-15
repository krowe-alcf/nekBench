project(axhelm)
set(axhelm VERSION 0.1)

add_executable(axhelm main.cpp ${CMAKE_SOURCE_DIR}/core/meshBasis.cpp)
target_link_libraries(axhelm PUBLIC ogs occa BLAS LAPACK)

#add_library(sphere-lib STATIC ${sphere-mod} ${sphere-src})
#target_link_libraries(sphere-lib PUBLIC ${libraries})
#target_include_directories(sphere-lib INTERFACE
#    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
#    $<INSTALL_INTERFACE:include>
#    )
#target_compile_definitions(sphere-lib PUBLIC "-D")

#install(TARGETS sphere-lib DESTINATION ${CMAKE_INSTALL_LIBDIR})
#install(
#    DIRECTORY ${CMAKE_BINARY_DIR}/include/
#    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
#    FILES_MATCHING PATTERN "*.mod"
#    )

# Serial Tests

## Executables

## Tests
#add_test(NAME hello COMMAND hello.x)

#add_test(NAME test-abort COMMAND test_abort.x)
#set_property(TEST test-abort PROPERTY WILL_FAIL TRUE)

#add_test(NAME test-error COMMAND test_error.x)
#set_property(TEST test-error PROPERTY WILL_FAIL TRUE)

#add_test(NAME test-assert COMMAND test_assert.x)
#set_property(TEST test-assert PROPERTY WILL_FAIL TRUE)

## Installation
install(TARGETS axhelm DESTINATION ${CMAKE_INSTALL_BINDIR})
