add_executable(vector_test vector_test.cpp)
target_link_libraries(vector_test gtest_main)
add_test(NAME vector_test COMMAND vector_test)

add_executable(timer_test timer_test.cpp)
target_link_libraries(timer_test timer gtest_main)
add_test(NAME timer_test COMMAND timer_test)
