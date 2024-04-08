# ChessGame With Using C++ OOP

## Description
This is a simple chess game that is implemented using C++ OOP. It the project of my learning C++ OOP.
The game is playing in console, and the pieces are represented by unicode characters.
Unicode display is using Windows's WriteConsoleW function(with overrided operator<<) to display, so it may not work on other platforms.
(I try to use wcout, but it doesn't work well with unicode characters, if you know how to fix it, please let me know)

## Environment
- Windows 11
- GCC 13.2.0(MinGW)
- CMake 3.28.0-rc3

## How to build
1. Makesure you have installed GCC and CMake
2. Clone the repository
```bash
git clone https://github.com/Notch-FGJ/Chess-WithCppOOP.git
cd Chess-WithCppOOP
```
3. Use CMake to generate build files and build
```bash
cmake -B build -G "MinGW Makefiles"
cmake --build build
```
4. Run the game
```bash
cd build
./ChessGame.exe
```