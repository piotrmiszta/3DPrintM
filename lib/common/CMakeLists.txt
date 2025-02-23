set(LIB_COMMON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/lib/common/include")
set(LIB_COMMON_SRC_DIR "${CMAKE_SOURCE_DIR}/lib/common/src")

add_library(lib_common
                   ${LIB_COMMON_SRC_DIR}/str.c
                   ${LIB_COMMON_INCLUDE_DIR}/err_codes.h
                   ${LIB_COMMON_INCLUDE_DIR}/list.h
                   ${LIB_COMMON_INCLUDE_DIR}/str.h
                   ${LIB_COMMON_INCLUDE_DIR}/logger.h
                   ${LIB_COMMON_INCLUDE_DIR}/queue.h)

set_target_properties(lib_common PROPERTIES LINKER_LANGUAGE C)

target_include_directories(lib_common PUBLIC ${LIB_COMMON_INCLUDE_DIR})

add_subdirectory(test)
