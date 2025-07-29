# Unit tests for allocators and utilities
file(GLOB UNIT_TEST_SOURCES *.cpp)
add_executable(unit_tests ${UNIT_TEST_SOURCES})
target_link_libraries(unit_tests PRIVATE pthread high_performance_allocator) 