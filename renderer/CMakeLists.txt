cmake_minimum_required(VERSION 3.10)
project(FaceRenderer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 외부 라이브러리 경로
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)

# OpenGL
find_package(OpenGL REQUIRED)

# GLFW 
set(GLFW_INCLUDE_DIR ${EXTERNAL_DIR}/glfw/include)
set(GLFW_LIBRARY ${EXTERNAL_DIR}/glfw/lib-mingw-w64/libglfw3.a)

# GLEW
set(GLEW_INCLUDE_DIR ${EXTERNAL_DIR}/glew/include)
set(GLEW_LIBRARY ${EXTERNAL_DIR}/glew/lib/Release/x64/glew32.lib)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/shader.cpp
    src/model.cpp
    src/mesh.cpp
    src/camera.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${EXTERNAL_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    OpenGL::GL
    ${GLFW_LIBRARY}
    ${GLEW_LIBRARY}
    gdi32
    user32
    kernel32
    ws2_32
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR}/renderer)