# Stress tests for allocator system
file(GLOB STRESS_TEST_SOURCES *.cpp)
add_executable(stress_tests ${STRESS_TEST_SOURCES})
target_link_libraries(stress_tests PRIVATE pthread) 