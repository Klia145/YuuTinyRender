@echo off
echo �����դ����Ⱦ��....
set SDL_PATH=x86_64-w64-mingw32
set INCLUDE_DIRS=-Iinclude -I%SDL_PATH%/include/SDL2
set LIB_DIRS=-L%SDL_PATH%/lib
set LIBS=-lmingw32 -lSDL2main -lSDL2
set FLAGS=-Wall -g3 -std=c++17

set SOURCES=^
src/test_window.cpp^
src/tgaimage.cpp ^
src/vec3.cpp ^
src/vec4.cpp ^
src/vec2.cpp ^
src/mat4.cpp ^
src/model.cpp ^
src/antialiasing.cpp ^
src/colorTable.cpp^

if %ERRORLEVEL%EQU 0(
    echo.
    echo ����ɹ���
    echo ���ڸ���SDL2.dll....
    copy %SDL_PATH%/bin/SDL2.dll build\ >nul 2>&1
    echo ��ɣ����У�cd build ^&^&main.exe
)else(
    echo.
    echo ����ʧ�ܣ�

)