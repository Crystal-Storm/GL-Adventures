# OpenGL installation

- Windows

Get MSYS2: https://github.com/msys2/msys2-installer/releases/download/2025-02-21/msys2-x86_64-20250221.exe

run "pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain mingw-w64-x86_64-glfw" in MSYS2

Open environment variables (Open settings and search edit environment variables). Edit Path in user variables, add "C:\msys64\ucrt64\bin". Click ok and ok to close

Open VSCode

Get CMake Tools

open A terminal and run the following command

"cmake -B ./build"
