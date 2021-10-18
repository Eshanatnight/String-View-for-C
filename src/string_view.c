#define SV_IMPLEMENTATION
#include "string_view.h"

#include <stdio.h>
#include <string.h>


StringView svFromParts(const char* data, size_t count)
{
    StringView sv;
    sv.m_data = data;
    sv.m_count = count;
    return sv;
}

StringView svFromCStr(const char* cstr)
{
    // returns a view of the string, or an empty view if the string is NULL
    return svFromParts(cstr, cstr ? strlen(cstr) : 0);
}

StringView svTrim(StringView sv)
{
    // returns a view of the string, or an empty view if the string is NULL
    if (sv.m_data == NULL)
        return sv;

    // trim leading whitespace
    while (sv.m_count > 0 && sv.m_data[0] == ' ')
    {
        sv.m_data++;
        sv.m_count--;
    }

    // trim trailing whitespace
    while (sv.m_count > 0 && sv.m_data[sv.m_count - 1] == ' ')
        sv.m_count--;

    return sv;
}

/*
    * @brief: Another Possible Implementation of svTrim

    StringView svTrim(StringView sv)
    {
        size_t i = 0;
        while(i < sv.m_count && sv.m_data[i] == ' ')
            i++;
    return svFromParts(sv.m_data + i, sv.m_count - i);
    }

    * @brief: This implementation is less efficient and creates a copy of sv , but it is more readable.
*/

StringView svTrimLeft(StringView sv)
{
    // returns a view of the string, or an empty view if the string is NULL
    if (sv.m_data == NULL)
        return sv;

    // trim leading whitespace
    while (sv.m_count > 0 && sv.m_data[0] == ' ')
    {
        sv.m_data++;
        sv.m_count--;
    }

    return sv;
}

/*
    @brief: This is another possible implementation for svTrimLeft,
            but is less efficient and creates a copy of sv ,
            but it is more readable.

    StringView svTrimLeft(StringView sv)
    {
        size_t i = 0;
        while(i < sv.m_count && sv.m_data[i] == ' ')
            i++;

        return svFromParts(sv.m_data + i, sv.m_count - i);
    }

*/