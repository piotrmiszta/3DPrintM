add_executable(test_lib_common test_common.c)
target_link_libraries(test_lib_common PRIVATE lib_common cmocka)

add_test(NAME testLibCommon COMMAND test_lib_common)