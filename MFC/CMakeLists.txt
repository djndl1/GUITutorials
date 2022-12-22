cmake_minimum_required(VERSION 3.22)

project(MFCTutorials
        VERSION 1.0.0.0
        DESCRIPTION "MFC Programming Tutorials")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED 11)
set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

include(FindMFC)
set(CMAKE_MFC_FLAG 2)
add_definitions(-D_AFXDLL -DUNICODE -D_UNICODE -DWINVER=0x0601 -D_WIN32_WINNT=0x0601)
include_directories(include)

add_executable(Hello WIN32 src/hello_mcf/Hello.cpp)

set(CMAKE_INSTALL_PREFIX .)
set(CMAKE_INSTALL_UCRT_LIBRARIES TRUE)
set(CMAKE_INSTALL_MFC_LIBRARIES TRUE)
include(InstallRequiredSystemLibraries)
install(TARGETS Hello)