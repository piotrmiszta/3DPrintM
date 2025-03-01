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

static void test_str_concatenation(void** state)
{
    str_t str1 = string_create_from_str_literal("STR1");
    str_t str2 = string_create_from_str_literal("STR2");
    str_t str3 = string_create_from_str_literal("STR3");
    str_t result = string_concatenate(str1, str2, str3, STR_EMPTY());
    assert_true(string_equal(result, string_create_from_str_literal("STR1STR2STR3")));
}

static void test_str_remove_whitespace(void** state)
{
    str_view_t str1 = STRING_VIEW("STR1  ");
    str_view_t str2 = STRING_VIEW("  STR2");
    str_view_t str3 = STRING_VIEW("  STR3  ");
    str_view_t str4 = STRING_VIEW("     ");
    str_view_t str1_expected = STRING_VIEW("STR1");
    str_view_t str2_expected = STRING_VIEW("STR2");
    str_view_t str3_expected = STRING_VIEW("STR3");
    str_view_t str4_expected = { 0 };

    assert_true(string_view_equal(string_view_remove_trailing_whitespace(str1),
                                  str1_expected));
    assert_true(string_view_equal(string_view_remove_leading_whitespace(str2),
                                  str2_expected));
    assert_true(string_view_equal(string_view_remove_whitespace(str3),
                                  str3_expected));
    assert_true(string_view_equal(string_view_remove_whitespace(str4),
                                  str4_expected));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_str_cmp),
        cmocka_unit_test(test_str_find),
        cmocka_unit_test(test_str_substring),
        cmocka_unit_test(test_str_tokenizer),
        cmocka_unit_test(test_str_concatenation),
        cmocka_unit_test(test_str_remove_whitespace),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
