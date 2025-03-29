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

The built file needs to be run in from the project directory, not the build directory.

If you don't want to use CMake to compile, this command should work:

"g++ -std=c++17 ./src/main.cpp ./lib/glad.c -o prog -I ./include/ -lmingw32 -lSDL2main -lSDL2"