@echo off
:start
cls
gcc virus.c -fopenmp -o virus.exe
virus.exe < virus.1000.50.in
pause
goto start