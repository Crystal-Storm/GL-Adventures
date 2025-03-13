cmake_minimum_required(VERSION 3.10)
project(MyOpenGLProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Find GLFW
find_package(glfw3 REQUIRED)

# Include directories
include_directories(include)

# Add executable
add_executable(my_opengl_project src/main.cpp lib/glad.c)

# Link libraries
target_link_libraries(my_opengl_project glfw)