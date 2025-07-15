# Integration tests for allocator system
file(GLOB INTEGRATION_TEST_SOURCES *.cpp)
add_executable(integration_tests ${INTEGRATION_TEST_SOURCES})
target_link_libraries(integration_tests PRIVATE pthread) 