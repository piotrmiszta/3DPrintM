#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

static void test_my_function(void **state) {
    (void) state;
    assert_int_equal(42, 42);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_my_function),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}