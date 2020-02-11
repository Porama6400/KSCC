@echo off
:start
cls
gcc virus.c -fopenmp -O3 -o virus.exe
pause
goto start