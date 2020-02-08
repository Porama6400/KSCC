cmake_minimum_required(VERSION 3.6)
project(KSCC)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

# !!! COMMENT THIS OUT ON RELEASE
set(CMAKE_BUILD_TYPE Debug)


add_executable(KSCC main.cpp)