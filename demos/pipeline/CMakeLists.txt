add_executable(pipeline pipeline.cpp ../main.cpp)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )
target_link_libraries(pipeline ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
target_link_libraries(pipeline application)
