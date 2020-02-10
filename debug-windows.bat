@echo off
:start
cls
gcc virus.c -fopenmp -g -o virus.exe
virus.exe < test.txt
pause
goto start