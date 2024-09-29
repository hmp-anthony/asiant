add_executable(flocking flocking.cpp ../main.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_library(flock_steer flock_steer.cpp flock_steer.hpp)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(flocking ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
target_link_libraries(flocking flock_steer application)
