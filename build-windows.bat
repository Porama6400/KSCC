@echo off
:start
cls
gcc virus.c -fopenmp -o virus.exe
virus.exe < virus.100.100.in
pause
goto start