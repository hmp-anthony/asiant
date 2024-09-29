add_library(timer timer.cpp ../include/asiant/timer.hpp)
target_include_directories(timer PUBLIC ../include)

add_library(utils utils.cpp ../include/asiant/utils.hpp)
target_include_directories(utils PUBLIC ../include)

add_library(steering_behaviour steering_behaviour.cpp ../include/asiant/steering_behaviour.hpp)
target_include_directories(steering_behaviour PUBLIC ../include)
