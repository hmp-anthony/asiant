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

add_executable(path_test path_test.cpp)
target_link_libraries(path_test gtest_main)
add_test(NAME path_test COMMAND path_test)

add_executable(graph_test graph_test.cpp)
target_link_libraries(graph_test graph gtest_main)
add_test(NAME graph_test COMMAND graph_test)

add_executable(priority_queue_test priority_queue_test.cpp)
target_link_libraries(priority_queue_test priority_queue gtest_main)
add_test(NAME priority_queue_test COMMAND priority_queue_test)

add_executable(level_test level_test.cpp)
target_link_libraries(level_test level graph gtest_main)
add_test(NAME level_test COMMAND level_test)
