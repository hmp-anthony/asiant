add_executable(separation separation.cpp ../main.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(separation ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
target_link_libraries(separation application)
