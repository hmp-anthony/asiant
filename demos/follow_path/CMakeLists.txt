add_executable(follow_path follow_path.cpp ../main.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(follow_path ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
target_link_libraries(follow_path application)
