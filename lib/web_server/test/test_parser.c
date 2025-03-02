#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "http_header.h"

static void test_parser_http_get(void** state)
{
    (void) state;
    str_view_t str = STRING_VIEW("GET /contact HTTP/1.1\n"
                                 "Host: example.com\n"
                                 "User-Agent: curl/8.6.0\n"
                                 "Accept: */*\n");

    http_header_t header;
    assert_int_equal(http_header_parse(&header, str), WEB_SERVER_ERR_SUCCESS);
    str_view_t url = STRING_VIEW("/contact");
    assert_int_equal(string_view_equal(url, header.url), true);
    assert_int_equal(header.method, HTTP_HEADER_GET);
    assert_int_equal(header.version, HTTP_HEADER_VERSION_1_1);
    str_view_t host = STRING_VIEW("Host");
    str_view_t host_value = STRING_VIEW("example.com");
    str_view_t user = STRING_VIEW("User-Agent");
    str_view_t user_value = STRING_VIEW("curl/8.6.0");
    str_view_t acc = STRING_VIEW("Accept");
    str_view_t acc_value = STRING_VIEW("*/*");

    assert_int_equal(header.elements.size, 3);
    http_header_element_t first = darray_at(&header.elements, 0);
    assert_int_equal(string_view_equal(host, first.key), true);
    assert_int_equal(string_view_equal(host_value, first.value), true);
    http_header_element_t sec = darray_at(&header.elements, 1);
    assert_int_equal(string_view_equal(user, sec.key), true);
    assert_int_equal(string_view_equal(user_value, sec.value), true);
    http_header_element_t third = darray_at(&header.elements, 2);
    assert_int_equal(string_view_equal(acc, third.key), true);
    assert_int_equal(string_view_equal(acc_value, third.value), true);
}

static void test_parser_http_post(void** state)
{
    (void) state;
    str_view_t str = STRING_VIEW("POST /contact HTTP/1.0\n"
                                 "Host: example.com\n"
                                 "User-Agent: curl/8.6.0\n"
                                 "Accept: */*\n");

    http_header_t header;
    assert_int_equal(http_header_parse(&header, str), WEB_SERVER_ERR_SUCCESS);
    str_view_t url = STRING_VIEW("/contact");
    assert_int_equal(string_view_equal(url, header.url), true);
    assert_int_equal(header.method, HTTP_HEADER_POST);
    assert_int_equal(header.version, HTTP_HEADER_VERSION_1_0);
    str_view_t host = STRING_VIEW("Host");
    str_view_t host_value = STRING_VIEW("example.com");
    str_view_t user = STRING_VIEW("User-Agent");
    str_view_t user_value = STRING_VIEW("curl/8.6.0");
    str_view_t acc = STRING_VIEW("Accept");
    str_view_t acc_value = STRING_VIEW("*/*");

    assert_int_equal(header.elements.size, 3);
    http_header_element_t first = darray_at(&header.elements, 0);
    assert_int_equal(string_view_equal(host, first.key), true);
    assert_int_equal(string_view_equal(host_value, first.value), true);
    http_header_element_t sec = darray_at(&header.elements, 1);
    assert_int_equal(string_view_equal(user, sec.key), true);
    assert_int_equal(string_view_equal(user_value, sec.value), true);
    http_header_element_t third = darray_at(&header.elements, 2);
    assert_int_equal(string_view_equal(acc, third.key), true);
    assert_int_equal(string_view_equal(acc_value, third.value), true);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parser_http_get),
        cmocka_unit_test(test_parser_http_post),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
