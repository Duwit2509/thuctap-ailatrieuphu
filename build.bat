@echo off
cls
echo BUILDING AI LA TRIEU PHU 

:: Compiler và flags
set COMPILER=g++
set FLAGS=-std=c++17 -Iinclude -static

:: Thư viện cần thiết (Am thanh Windows)
set LIBS=-lwinmm

:: Các file nguồn
set SRC=src/main.cpp src/game.cpp src/game_ui.cpp src/admin.cpp src/helpers.cpp src/json_handler.cpp

:: Lệnh biên dịch (Đã thêm %LIBS% vào cuối)
echo Compiling...
%COMPILER% %FLAGS% %SRC% %LIBS% -o ailatrieuphu.exe 
IF %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b
)

echo Build successful!
echo Running program...
ailatrieuphu.exe
pause
