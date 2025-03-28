add_executable(MyCheatEngine
    main.cpp
    ErrorLog.cpp
    SystemFunctionsCommon.cpp
    SystemFunctionsWindows.cpp
    SystemFunctionsLinux.cpp
    Program.cpp)
target_link_libraries(MyCheatEngine PRIVATE sokol imgui-docking)