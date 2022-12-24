cmake_minimum_required(VERSION 3.10)
project(gtk_c)

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

include(GoogleTest)

gtest_discover_tests(glib_basics)
gtest_discover_tests(glib_logging)
gtest_discover_tests(glib_environment)