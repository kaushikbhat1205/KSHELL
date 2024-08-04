cmake_minimum_required(VERSION 3.28)
project(kshell)

set(CMAKE_CXX_STANDARD 26)

add_executable(kshell main.cpp
        bits.h)
