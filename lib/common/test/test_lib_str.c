#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <str.h>
#include <err_codes.h>

static void test_str_cmp(void** state)
{
    (void) state;
    str_t string1 = string_create_from_str_literal("TEST_STR");
    str_t string2 = string_create_from_str_literal("TEST_STR");
    str_t string3 = string_create_from_str_literal("TEST_STe");

    assert_true(string_equal(string1, string2));
    assert_false(string_equal(string1, string3));
}

static void test_str_find(void** state)
{
    (void) state;
    str_t string1 = string_create_from_str_literal("TEST_STR");
    int32_t index = string_find(string1, '_');
    assert_int_equal(index, 4);
    index = string_find(string1, 'R');
    assert_int_equal(index, 7);
    index = string_find(string1, 'T');
    assert_int_equal(index, 0);
    index = string_find(string1, 'G');
    assert_int_equal(index, LIB_COMMON_NOT_FOUND);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_str_cmp),
        cmocka_unit_test(test_str_find),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}