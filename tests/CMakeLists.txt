add_executable(vector_test vector_test.cpp)
target_link_libraries(vector_test gtest_main)
add_test(NAME vector_test COMMAND vector_test)

add_executable(timer_test timer_test.cpp)
target_link_libraries(timer_test timer gtest_main)
add_test(NAME timer_test COMMAND timer_test)

add_executable(utils_test utils_test.cpp)
target_link_libraries(utils_test utils gtest_main)
add_test(NAME utils_test COMMAND utils_test)

add_executable(line_segment_test line_segment_test.cpp)
target_link_libraries(line_segment_test gtest_main)
add_test(NAME line_segment_test COMMAND line_segment_test)
