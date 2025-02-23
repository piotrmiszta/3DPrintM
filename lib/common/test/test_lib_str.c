#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <str.h>
#include <str_view.h>
#include <err_codes.h>

static void test_str_cmp(void** state)
{
    (void) state;
    str_t string1 = string_create_from_str_literal("TEST_STR");
    str_t string2 = string_create_from_str_literal("TEST_STR");
    str_t string3 = string_create_from_str_literal("TEST_STe");

    assert_true(string_equal(string1, string2));
    assert_false(string_equal(string1, string3));

    string_free(string1);
    string_free(string2);
    string_free(string3);
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
    string_free(string1);
}

static void test_str_substring(void **state)
{
    (void)state;
    str_t string1 = string_create_from_str_literal("TEST_STR_SUBSTR");
    int32_t size = string_get_size(string1);
    int32_t index = string_find(string1, '_');
    str_t substr = string_substr(string1, index, size - 1);
    str_t expected = string_create_from_str_literal("_STR_SUBSTR");
    assert_true(string_equal(substr, expected));
    string_free(expected);
    string_free(substr);
    string_free(string1);
}

static void test_str_tokenizer(void** state)
{
    (void)state;
    str_view_t long_string = STRING_VIEW("This is example:with multiple words");
    str_view_t delims = STRING_VIEW(" :");
    str_view_tokenizer_t tokenizer = string_view_tokenizer_init(long_string, delims);
    str_view_t word = string_view_tokenizer_next(&tokenizer);
    assert_true(string_view_equal(word, STRING_VIEW("This")));
    word = string_view_tokenizer_next(&tokenizer);
    assert_true(string_view_equal(word, STRING_VIEW("is")));
    word = string_view_tokenizer_next(&tokenizer);
    assert_true(string_view_equal(word, STRING_VIEW("example")));
    word = string_view_tokenizer_next(&tokenizer);
    assert_true(string_view_equal(word, STRING_VIEW("with")));
    word = string_view_tokenizer_next(&tokenizer);
    assert_true(string_view_equal(word, STRING_VIEW("multiple")));
    word = string_view_tokenizer_next(&tokenizer);
    assert_true(string_view_equal(word, STRING_VIEW("words")));
    word = string_view_tokenizer_next(&tokenizer);
    assert_int_equal(word.size, 0);
    assert_null(word.data);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_str_cmp),
        cmocka_unit_test(test_str_find),
        cmocka_unit_test(test_str_substring),
        cmocka_unit_test(test_str_tokenizer),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
