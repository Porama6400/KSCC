@echo off
C:\cygwin64\bin\cmake.exe . -B build
cd build
C:\cygwin64\bin\make.exe
echo ============================================================
KSCC.exe

REM Old argument was --build /cygdrive/c/Users/Porama6400/Documents/KSCC/cmake-build-debug --target KSCC -- -j 4
pause