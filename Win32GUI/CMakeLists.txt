cmake_minimum_required(VERSION 3.13)

project(Win32GUITutorials
        VERSION 1.0.0.0
        DESCRIPTION "Win32 GUI Programming Tutorials")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include_directories(include)
add_compile_options(-DUNICODE -D_UNICODE -municode)
add_link_options(-municode)

add_executable(HelloWindows src/HelloWindows.c)
add_executable(FirstWindow src/FirstWindow.c)
add_executable(SysMets src/SysMets.c)
add_executable(DevCaps src/DevCaps.c)
add_executable(SineWave src/SineWave.c)
add_executable(Beeper1 src/Beeper1.c)
add_executable(BeeperWithCallback src/BeeperWithCallback.c)