@echo off
echo ============================================
echo   Smart College Bus Management System
echo   Compiling with g++ (C++11)
echo ============================================
g++ -std=c++11 -Wall -o SmartBus.exe main.cpp
if %errorlevel% == 0 (
    echo.
    echo [OK] Compilation Successful!
    echo [>>] Run SmartBus.exe to start the program.
    echo.
) else (
    echo.
    echo [!!] Compilation Failed. Check errors above.
    echo.
)
pause
