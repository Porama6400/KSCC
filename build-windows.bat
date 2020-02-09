@echo off
:start
cls
gcc virus.c -fopenmp -o virus.exe
virus.exe < virus.in
pause
goto start