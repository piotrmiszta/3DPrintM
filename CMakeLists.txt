cmake_minimum_required(VERSION 3.5.0)
project(web_server VERSION 0.1.0 LANGUAGES C)

set(CMAKE_CXX_COMPILER gcc)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_FLAGS "-O0 -g -pedantic -Werror")

add_executable( web_server
                include/err_codes.h
                include/web_server.h
                include/list.h
                include/types.h

                src/web_server.c
                src/main.c)

target_include_directories(web_server PUBLIC include/)
