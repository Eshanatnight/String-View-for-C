#include "./src/string_view.h"
#include <stdio.h>

#include <stdio.h>
#include <ctype.h>
#include <inttypes.h>

void sv_assert_eq_StringView(const char *file, size_t line,
                              const char *expected_expr, StringView expected,
                              const char *actual_expr, StringView actual)
{
    if (!sv_eq(expected, actual)) {
        fprintf(stderr, "%s:%zu: FAILED: %s == %s\n",
                file, line, expected_expr, actual_expr);
        fprintf(stderr, "  EXPECTED: " SV_FMT "\n", SV_Arg(expected));
        fprintf(stderr, "  ACTUAL:   " SV_FMT "\n", SV_Arg(actual));
        exit(1);
    }
}

void sv_assert_eq_size_t(const char *file, size_t line,
                         const char *expected_expr, size_t expected,
                         const char *actual_expr, size_t actual)
{
    if (expected != actual) {
        fprintf(stderr, "%s:%zu: FAILED: %s == %s\n",
                file, line, expected_expr, actual_expr);
        fprintf(stderr, "  EXPECTED: %zu\n", expected);
        fprintf(stderr, "  ACTUAL:   %zu\n", actual);
        exit(1);
    }
}

void sv_assert_eq_uint64_t(const char *file, size_t line,
                           const char *expected_expr, uint64_t expected,
                           const char *actual_expr, uint64_t actual)
{
    if (expected != actual) {
        fprintf(stderr, "%s:%zu: FAILED: %s == %s\n",
                file, line, expected_expr, actual_expr);
        fprintf(stderr, "  EXPECTED: %" PRIu64 "\n", expected);
        fprintf(stderr, "  ACTUAL:   %" PRIu64 "\n", actual);
        exit(1);
    }
}

#define ASSERT_EQ(type, expected, actual) sv_assert_eq_##type(__FILE__, __LINE__, #expected, expected, #actual, actual)

void sv_assert_true(const char *file, size_t line,
                    const char *expression_cstr,
                    bool expression)
{
    if (!expression) {
        fprintf(stderr, "%s:%zu: FAILED: %s\n", file, line, expression_cstr);
        exit(1);
    }
}

#define ASSERT_TRUE(expression) sv_assert_true(__FILE__, __LINE__, #expression, expression);

bool is_alpha(char x)
{
    return isalpha(x);
}

int main(void)
{
    // Construct
    {
        ASSERT_EQ(StringView, SV("Foo"), svFromCStr("Foo"));
    }

    // Trimming Whitespace
    {
        ASSERT_EQ(StringView, SV("hello    "), svTrimLeft(SV("    hello    ")));
        ASSERT_EQ(StringView, SV("    hello"), svTrimRight(SV("    hello    ")));
        ASSERT_EQ(StringView, SV("hello"), svTrim(SV("    hello    ")));
    }

    // Chop by delimiter
    {
        // Existing
        {
            StringView input = SV_STATIC("hello\nworld");
            StringView line = svChopByDelim(&input, '\n');
            ASSERT_EQ(StringView, SV("hello"), line);
            ASSERT_EQ(StringView, SV("world"), input);
        }

        // Non-Existing
        {
            StringView input = SV_STATIC("hello\nworld");
            StringView line = svChopByDelim(&input, ' ');
            ASSERT_EQ(StringView, SV("hello\nworld"), line);
            ASSERT_EQ(StringView, SV(""), input);
        }
    }

    // Try to chop by delimiter
    {
        // Existing
        {
            StringView input = SV_STATIC("hello\nworld");
            StringView line = SV_NULL;
            bool result = try_svChopByDelim(&input, '\n', &line);
            ASSERT_TRUE(result);
            ASSERT_EQ(StringView, SV("hello"), line);
            ASSERT_EQ(StringView, SV("world"), input);
        }

        // Non-Existing
        {
            StringView input = SV_STATIC("hello\nworld");
            StringView line = SV_NULL;
            bool result = try_svChopByDelim(&input, ' ', &line);
            ASSERT_TRUE(!result);
            ASSERT_EQ(StringView, SV(""), line);
            ASSERT_EQ(StringView, SV("hello\nworld"), input);
        }
    }

    // Chop N characters
    {
        // Chop left
        {
            StringView input = SV_STATIC("hello");
            StringView hell = svChopLeft(&input, 4);
            ASSERT_EQ(StringView, SV("o"), input);
            ASSERT_EQ(StringView, SV("hell"), hell);
        }

        // Overchop left
        {
            StringView input = SV_STATIC("hello");
            StringView hell = svChopLeft(&input, 10);
            ASSERT_EQ(StringView, SV(""), input);
            ASSERT_EQ(StringView, SV("hello"), hell);
        }

        // Chop right
        {
            StringView input = SV_STATIC("hello");
            StringView hell = svChopRight(&input, 4);
            ASSERT_EQ(StringView, SV("h"), input);
            ASSERT_EQ(StringView, SV("ello"), hell);
        }

        // Overchop right
        {
            StringView input = SV_STATIC("hello");
            StringView hell = svChopRight(&input, 10);
            ASSERT_EQ(StringView, SV(""), input);
            ASSERT_EQ(StringView, SV("hello"), hell);
        }
    }

    // Take while
    {
        // Take while is_alpha
        {
            StringView input = SV_STATIC("hello1234");
            StringView hello = svTakeLeftWhile(input, is_alpha);
            ASSERT_EQ(StringView, SV("hello1234"), input);
            ASSERT_EQ(StringView, SV("hello"), hello);
        }

        // Overtake while
        {
            StringView input = SV_STATIC("helloworld");
            StringView hello = svTakeLeftWhile(input, is_alpha);
            ASSERT_EQ(StringView, SV("helloworld"), input);
            ASSERT_EQ(StringView, SV("helloworld"), hello);
        }
    }

    // Chop while
    {
        // Chop while is_alpha
        {
            StringView input = SV_STATIC("hello1234");
            StringView hello = svChopLeftWhile(&input, is_alpha);
            ASSERT_EQ(StringView, SV("1234"), input);
            ASSERT_EQ(StringView, SV("hello"), hello);
        }

        // Overchop while
        {
            StringView input = SV_STATIC("helloworld");
            StringView hello = svChopLeftWhile(&input, is_alpha);
            ASSERT_EQ(StringView, SV(""), input);
            ASSERT_EQ(StringView, SV("helloworld"), hello);
        }
    }

    // Index of
    {
        size_t index = 0;
        ASSERT_TRUE(sv_index_of(SV("hello world"), ' ', &index));
        ASSERT_EQ(size_t, 5, index);
    }

    // Prefix/suffix check
    {
        ASSERT_TRUE(sv_starts_with(SV("Hello, World"), SV("Hello")));
        ASSERT_TRUE(sv_ends_with(SV("Hello, World"), SV("World")));
    }

    // To Integer
    {
        ASSERT_EQ(uint64_t, 1234567890, sv_to_u64(SV("1234567890")));
    }

    printf("OK\n");

    return 0;
}
