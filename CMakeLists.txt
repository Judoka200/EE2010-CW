cmake_minimum_required(VERSION 3.10.0)

project(TextDungeon VERSION 1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(main src/main.cpp)