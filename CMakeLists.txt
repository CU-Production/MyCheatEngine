cmake_minimum_required(VERSION 3.30)
project(MyCheatEngine)

set(CMAKE_CXX_STANDARD 20)

add_executable(victim victim.cpp)
add_executable(MyCheatEngine main.cpp)
