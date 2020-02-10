@echo off
:start
cls
gcc virus.c -fopenmp -o virus.exe
virus.exe < testdata.txt
pause
goto start