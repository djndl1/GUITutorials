cmake_minimum_required(VERSION 3.22)

project(MFCTutorials
        VERSION 1.0.0.0
        DESCRIPTION "MFC Programming Tutorials")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED 11)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_MFC_FLAG 2)
add_definitions(-D_AFXDLL -DUNICODE -D_UNICODE)
include_directories(include)

add_executable(Hello WIN32 src/hello_mcf/Hello.cpp)