cmake_minimum_required(VERSION 3.5.0)
project(3dprintm VERSION 0.1.0 LANGUAGES C)

set(CMAKE_CXX_COMPILER gcc)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "-O0 -g -pedantic -Werror")
set(CMAKE_C_STANDARD 11)

add_executable( 3dprintm
                app/main.c)


enable_testing()

target_link_libraries(3dprintm PUBLIC "lib_common")
target_link_libraries(3dprintm PUBLIC "lib_web_server")


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(cmocka 1.1.0 REQUIRED)
include(${CMAKE_SOURCE_DIR}/cmake/AddCMockaTest.cmake)

add_subdirectory(lib)
