@echo off
setlocal

set output=knapsack_algorithm.exe
set stack_size=0x1000000
set optimization_level=-O2

echo compiling...
clang++ -std=c++20 -o %output% %optimization_level% main.cpp -Wl,--stack,%stack_size% bottom_up\bottom_up.cpp brute_force\brute_force.cpp genetic_algorithm\genetic_algorithm.cpp top_down\top_down.cpp

if %ERRORLEVEL% equ 0 (
    echo compilation successful.
    echo running %output%...
    .\%output%
) else (
    echo compilation failed.
    exit /b 1
)

endlocal
