cmake_minimum_required(VERSION 3.30)
project(MyCheatEngine)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(3rd_party)

add_executable(victim victim.cpp)

add_executable(MyCheatEngine main.cpp)
target_link_libraries(MyCheatEngine PRIVATE sokol imgui-docking)
