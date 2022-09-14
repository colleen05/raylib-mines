# raylib-mines
raylib-mines is a [raylib](https://www.raylib.com/) example project, cloning the classic minesweeper game. This is a community project, and not an official raylib example project. Official examples are available on bot the raylib website, and in the raylib GitHub repository. It demonstrates texture loading, drawing, and input handling.

[](https://i.imgur.com/l4DKvUy.png "raylib-mines screenshot")

## Compiling
This project has been tested for compiling with GCC version 11, using standard C99 (`-std=c99`). It can be compiled either with the `build_linux.sh` script on Linux, or `build_windows.bat` on Windows, which both assume you have GCC installed and added to your PATH envrionment variable. If you are using a different compiler, first see the [Raylib](#Raylib) section for compiling raylib for your compiler.

### Building and running on Linux
```
$ ./build_linux.sh
$ ./bin/linux/mines
```

### Building and running on Windows
```
> build_windows.bat
> bin\windows\mines.exe
```

## Raylib
Raylib is a simple and easy-to-use C library for game development. The code uses raylib version 4.0.0, with precompiled libraries for GCC. If you are using a different compiler, check out the "build and installation section" on raylib's [GitHub page](https://github.com/raysan5/raylib).

# Licensing
raylib-mines is distributed under the zlib license. See the `LICENSE` file for more information.