add_executable(path_follow path_follow.cpp ../main.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(path_follow ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
target_link_libraries(path_follow application)
