cmake_minimum_required(VERSION 3.10)
project(gtk_c)


set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

if (USE_GTK2)
  add_compile_options(${GTK2_CFLAGS})
  link_libraries(${GTK2_LIBRARIES})
else()
  add_compile_options(${GTK3_CFLAGS})
  link_libraries(${GTK3_LIBRARIES})
endif()

add_compile_options(${GLIB2_CFLAGS})
link_libraries(${GLIB2_LIBRARIES})
include_directories(${gtk_c_SOURCE_DIR}/include)

add_executable(helloworld_gtk src/helloworld.c)
add_executable(button_gtk src/button.c)
add_executable(exercise2.2 src/exercise2.2.c)
add_executable(gtkbox src/gtkbox.c)
add_executable(gtkpane src/panes.c)
add_executable(gtkgrid src/tables.c)
add_executable(gtkfixed src/fixed.c)
add_executable(gtkexpander src/expander.c)
add_executable(gtknotebook src/notebook.c)
add_executable(exercise3.1 src/exercise3.1.c)

add_executable(gtkradiobutton src/radiobuttons.c)
add_executable(gtkentry src/entry.c)
add_executable(gtkdialog src/SimpleDialog.c)
add_executable(gtkmsg src/MessageDialog.c)

add_executable(gtimeout src/timeouts.c)

add_executable(exercise6.1 src/exercise6.1.c)
add_executable(exercise6.2 src/exercise6.2.c)

add_executable(scrolledwindow src/scrolledwindows.c)
add_executable(textview src/textview2.c)
add_executable(iterator src/iterator.c)


# tests
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(glib_basics src/glib/basics.cc)
target_link_libraries(glib_basics GTest::gtest_main)

add_executable(glib_logging src/glib/logging.cc)
target_link_libraries(glib_logging GTest::gtest_main)

add_executable(glib_environment src/glib/environment.cc)
target_link_libraries(glib_environment GTest::gtest_main)

add_executable(glib_memory src/glib/memory.cc)
target_link_libraries(glib_memory GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(glib_basics)
gtest_discover_tests(glib_logging)
gtest_discover_tests(glib_environment)
gtest_discover_tests(glib_memory)