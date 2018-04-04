cmake_minimum_required(VERSION 3.9)
project(NewGLEngine)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/modules/")

find_package(OpenGL)
find_package(freeglut)
find_package(DevIL)
find_package(glm)
find_package(Assimp)
find_package(Libunwind)

add_executable(NewGLEngine main.cpp Camera.h Camera.cpp gl_core_4_5.h gl_core_4_5.c GLSLShader.h GLSLShader.cpp
        Material.h Material.cpp Mesh.h Mesh.cpp Scene.h Scene.cpp Texture.h Texture.cpp Uniform.h Uniform.cpp
        Vertex.h Vertex.cpp)



include_directories(${OpenGL_INCLUDE_DIRS} ${freeglut_INCLUDE_DIRS} ${DevIL_INCLUDE_DIRS} ${glm_INCLUDE_DIRS}
        ${ASSIMP_INCLUDE_DIRS} ${LIBUNWIND_INCLUDE_DIR})
target_link_libraries(NewGLEngine ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${freeglut_LIBRARIES} ${IL_LIBRARIES}
        ${ILU_LIBRARIES} ${ASSIMP_LIBRARIES} ${OpenGL_LIBRARIES} ${LIBUNWIND_LIBRARIES})