cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

# Find SDL2
find_package(SDL2 REQUIRED)

# Include directories
include_directories(include/)

# Add executable
add_executable(opengl_project src/main.cpp src/Camera.cpp lib/glad.c)

# Link libraries
target_link_libraries(opengl_project SDL2::SDL2main SDL2::SDL2)