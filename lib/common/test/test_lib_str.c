#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <str.h>

static void test_str_cmp(void** state) {
    (void) state;
    str_t string1 = string_create_from_str_literal("TEST_STR");
    str_t string2 = string_create_from_str_literal("TEST_STR");
    str_t string3 = string_create_from_str_literal("TEST_STe");

    assert_true(string_equal(string1, string2));
    assert_false(string_equal(string1, string3));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_str_cmp),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}