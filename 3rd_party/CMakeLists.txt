# sokol
add_library(sokol INTERFACE)
target_include_directories(sokol INTERFACE sokol)

# imgui-docking
add_library(imgui-docking
    imgui-docking/imgui.cpp
    imgui-docking/imgui_widgets.cpp
    imgui-docking/imgui_tables.cpp
    imgui-docking/imgui_demo.cpp
    imgui-docking/imgui_draw.cpp)
target_include_directories(imgui-docking PUBLIC imgui-docking)