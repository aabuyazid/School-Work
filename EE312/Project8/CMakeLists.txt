cmake_minimum_required(VERSION 3.9)
project(Project8)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Input.cpp Parse.h Node.cpp Node.h Text.cpp Symbols.cpp Symbols.h)
add_executable(Project8 main.cpp Input.cpp Parse.h Node.cpp Node.h Text.cpp Symbols.cpp Symbols.h)