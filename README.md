## What is this project?

This project follows a youtube series created by Mike Shah, in which he teaches the concepts of what OpenGL is and how it can be used.

## What does it do?

At the momement the program renders a colored quad to the screen and you are able to move around relative to it using WASD. The mouse starts as locked; this can be toggled with escape. I have almost finished implementing looking around with your mouse as well. Each time the program is executed, both shaders are recompiled meaning they can be modified between runs.

## What did I learn?

From creating this project I have learned:

- Procedural and a little Object C++ programming

- What OpenGL is

- What the graphics pipeline is

- how to write simple GLSL shaders

- How to compile C++ programs

- How to use CMake

## OpenGL installation

### Windows

Download the MSYS2 installer and run: https://www.msys2.org/

+ run "pacman -S base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-glfw mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-glm" in MSYS2

+ Open environment variables (Open settings and search edit environment variables). Edit Path in user variables, add "C:\msys64\ucrt64\bin" (If you changed the default installation directory of MSYS2 then add "{Alternate path}\ucrt64\bin" instead). Click OK and OK to close.

+ Open VSCode and get CMake Tools extension

+ Open the VSCode command palette (Ctrl+Shift+P) and run "CMake: Build"

### Linux

Make sure that the following packages are installed:

+ base-devel

+ glfw

+ sdl2

+ glm

## Notes

The built file needs to be run in from the build directory, not the project directory. The minimum files required are the project directory with a shaders and build directory. The build directory contains the compiled program. And the shaders directory contains a fragment and vertex shader.

Running the following command should create an executable.

Windows:
```g++ -std=c++17 ../src/*.cpp ../lib/glad.c -o prog -I ../include/ -lmingw32 -lSDL2main -lSDL2```

Linux:
```g++ -std=c++17 ../src/*.cpp ../lib/glad.c -o prog -I ../include/ -lSDL2main -lSDL2```
