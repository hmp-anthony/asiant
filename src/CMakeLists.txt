add_library(timer timer.cpp ../include/asiant/timer.hpp)
target_include_directories(timer PUBLIC ../include)

add_library(utils utils.cpp ../include/asiant/utils.hpp)
target_include_directories(utils PUBLIC ../include)

add_library(steering_behaviour steering_behaviour.cpp ../include/asiant/steering_behaviour.hpp)
target_include_directories(steering_behaviour PUBLIC ../include)
target_link_libraries(steering_behaviour utils)

add_library(steering_pipeline steering_pipeline.cpp ../include/asiant/steering_pipeline.hpp)
target_include_directories(steering_pipeline PUBLIC ../include)
target_link_libraries(steering_pipeline utils)

add_library(path path.cpp ../include/asiant/path.hpp)
target_include_directories(path PUBLIC ../include)
target_link_libraries(path utils)
