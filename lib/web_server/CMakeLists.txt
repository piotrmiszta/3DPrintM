set(LIB_WEB_SERVER_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/web_server/include/")
set(LIB_WEB_SERVER_SRC_DIR "${CMAKE_SOURCE_DIR}/lib/web_server/src/")

add_library(lib_web_server ${LIB_WEB_SERVER_INCLUDE_DIR}/parser.h
                   ${LIB_WEB_SERVER_INCLUDE_DIR}/types.h
                   ${LIB_WEB_SERVER_INCLUDE_DIR}/web_server.h
                   ${LIB_WEB_SERVER_INCLUDE_DIR}/http_header.h
                   ${LIB_WEB_SERVER_SRC_DIR}/http_header.c
                   ${LIB_WEB_SERVER_SRC_DIR}/web_server.c
                   ${LIB_WEB_SERVER_SRC_DIR}/worker.c
                   ${LIB_WEB_SERVER_SRC_DIR}/parser.c)

set_target_properties(lib_web_server PROPERTIES LINKER_LANGUAGE C)

target_include_directories(lib_web_server PUBLIC ${LIB_WEB_SERVER_INCLUDE_DIR})

target_link_libraries(lib_web_server lib_common)
add_subdirectory(test)
