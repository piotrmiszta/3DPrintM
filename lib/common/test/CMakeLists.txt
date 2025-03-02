add_executable(test_lib_string test_lib_str.c)
target_link_libraries(test_lib_string PRIVATE lib_common cmocka)

add_test(NAME test_lib_string COMMAND test_lib_string)

add_executable(test_lib_darray test_lib_darray.c)
target_link_libraries(test_lib_darray PRIVATE lib_common cmocka)

add_test(NAME test_lib_darray COMMAND test_lib_darray)