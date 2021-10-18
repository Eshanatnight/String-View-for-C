#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct StringView
{
    size_t m_count;
    const char* m_data;
}StringView;

#define SV(cstr_lit) svFromParts(cstr_lit, sizeof(cstr_lit) - 1)

#define SV_STATIC(cstr_lit)     \
    {                           \
        sizeof(cstr_lit) - 1,   \
        cstr_lit                \
    }

#define SV_NULL svFromParts(NULL, 0)

// Need a macro for SV for printf
#define SV_FMT "%.*s"

#define SV_AVERAGE(sv) (int) (sv).m_count, (sv).m_data
/*
    * @brief: Usage
    * StringView sv = ...;
    * printf("Name: SV_FMT", SV_AVERAGE(name));
*/

// Function Signatures

StringView svFromParts(const char* data, size_t count);
StringView svFromCStr(const char* cstr);

StringView svTrim(StringView sv);
StringView svTrimLeft(StringView sv);
StringView svTrimRight(StringView sv);

StringView svTakeLeftWhile(StringView sv, bool (*predicate)(char x));

bool try_svChopByDelim(StringView* sv, char delim, StringView* chunk);
StringView svChopByDelim(StringView* sv, char delim);

StringView svChopLeft(StringView* sv, size_t count);
StringView svChopRight(StringView* sv, size_t count);
StringView svChopLeftWhile(StringView* sv, bool (*predicate)(char x));
bool svIndexOf(StringView sv, char c, size_t* index);
bool svIsEqual(StringView a, StringView b);
bool svStartsWith(StringView sv, StringView prefix);
bool svEndsWith(StringView sv, StringView suffix);
uint64_t svTou64(StringView sv);



#endif //STRING_VIEW_H
