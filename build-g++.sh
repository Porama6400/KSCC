#! /bin/bash
g++-9 -o build-mac -fopenmp main.cpp 
echo compile completed
./build-mac
exit 0