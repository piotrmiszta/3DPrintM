#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include "darray.h"

static void test_darray(void** state)
{
    (void) state;
    darray(int) vector;

    darray_init(&vector);
    darray_push_back(&vector, 1);
    darray_push_back(&vector, 2);
    darray_push_back(&vector, 3);
    darray_push_back(&vector, 4);
    darray_push_back(&vector, 5);
    assert_int_equal(darray_at(&vector, 2), 3);
    assert_int_equal(vector.size, 5);
    assert_int_equal(vector.capacity, 5);
    darray_erase(&vector, 3);
    int expected = 1;
    for(uint32_t i = 0; i < vector.size; i++)
    {
        if(i == 3)
        {
            expected++;
        }
        assert_int_equal(darray_at(&vector, i), expected);
        expected++;
    }
    darray_deinit(&vector);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_darray),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
