cmake_minimum_required(VERSION 3.13)

project(WTLTutorials
        VERSION 1.0.0.0
        DESCRIPTION "WTL Programming Tutorials")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED 11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include_directories(include)
add_definitions(-DUNICODE -D_UNICODE)