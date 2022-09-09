#define SV_IMPLEMENTATION
#include "string_view.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

StringView svFromParts(const char* data, size_t count)
{
	StringView sv;
	sv.m_data  = data;
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
	if(sv.m_data == NULL) return sv;

	// trim leading whitespace
	while(sv.m_count > 0 && sv.m_data[0] == ' ')
	{
		sv.m_data++;
		sv.m_count--;
	}

	// trim trailing whitespace
	while(sv.m_count > 0 && sv.m_data[sv.m_count - 1] == ' ') sv.m_count--;

	return sv;
}

/*
	*   @brief: Another Possible Implementation of svTrim

	StringView svTrim(StringView sv)
	{
		size_t i = 0;
		while(i < sv.m_count && sv.m_data[i] == ' ')
			i++;
	return svFromParts(sv.m_data + i, sv.m_count - i);
	}

	* @brief: This implementation is less efficient and creates a copy of sv , but it is more
   readable.
*/

StringView svTrimLeft(StringView sv)
{
	// returns a view of the string, or an empty view if the string is NULL
	if(sv.m_data == NULL) return sv;

	// trim leading whitespace
	while(sv.m_count > 0 && sv.m_data[0] == ' ')
	{
		sv.m_data++;
		sv.m_count--;
	}

	return sv;
}

/*
	*   @brief: This is another possible implementation for svTrimLeft,
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

StringView svTrimRight(StringView sv)
{
	// returns a view of the string, or an empty view if the string is NULL
	if(sv.m_data == NULL) return sv;

	// trim trailing whitespace
	while(sv.m_count > 0 && sv.m_data[sv.m_count - 1] == ' ') sv.m_count--;

	return sv;
}

/*
	*   @brief: This is another possible implementation for svTrimRight,
			but is less efficient and creates a copy of sv ,
			but it is more readable.

	StringView svTrimRight(StringView sv)
	{
		size_t i = 0;
		while(i < sv.m_count && sv.m_data[i] == ' ')
			i++;

		return svFromParts(sv.m_data, sv.m_count - i);
	}
*/

StringView svTakeLeftWhile(StringView sv, bool (*predicate)(char x))
{
	size_t i = 0;
	while(i < sv.m_count && predicate(sv.m_data[i]))
	{
		i += 1;
	}
	return svFromParts(sv.m_data, i);
}

/*
	*   @brief: This is another possible implementation for svTakeLeftWhile,
			but is less efficient and creates a copy of sv,
			but it is more readable.

	StringView svTakeLeftWhile(StringView sv, bool (*predicate)(char x))
	{
		size_t i = 0;
		while(i < sv.m_count && predicate(sv.m_data[i]))
			i++;

		return svFromParts(sv.m_data, i);
	}
*/

bool try_svChopByDelim(StringView* sv, char delim, StringView* chunk)
{
	size_t i = 0;
	while(i < sv->m_count && sv->m_data[i] != delim) i += 1;

	StringView result = svFromParts(sv->m_data, i);

	if(i < sv->m_count)
	{
		sv->m_count -= i + 1;
		sv->m_data += i + 1;
		if(chunk) *chunk = result;

		return true;
	}

	return false;
}

StringView svChopByDelim(StringView* sv, char delim)
{
	size_t i = 0;
	while(i < sv->m_count && sv->m_data[i] != delim)
	{
		i += 1;
	}

	StringView result = svFromParts(sv->m_data, i);

	if(i < sv->m_count)
	{
		sv->m_count -= i + 1;
		sv->m_data += i + 1;
	} else {
		sv->m_count -= i;
		sv->m_data += i;
	}

	return result;
}

StringView svChopBySV(StringView* sv, StringView thicc_delim)
{
	StringView window = svFromParts(sv->m_data, thicc_delim.m_count);
	size_t i		  = 0;
	while(i + thicc_delim.m_count < sv->m_count && !(svIsEqual(window, thicc_delim)))
	{
		i++;
		window.m_data++;
	}

	StringView result = svFromParts(sv->m_data, i);

	if(i + thicc_delim.m_count == sv->m_count)
	{
		// include last <thicc_delim.m_count> characters if they aren't
		//  equal to thicc_delim
		result.m_count += thicc_delim.m_count;
	}

	// Chop!
	sv->m_data += i + thicc_delim.m_count;
	sv->m_count -= i + thicc_delim.m_count;

	return result;
}

/*
	*   @brief: the following implementations is never tested,
		but it might be more efficient over readability.

	Todo: test the following implementation.
	StringView svChopByDelim_2(StringView* sv, char delim)
	{
	StringView chunk;
	if (try_svChopByDelim(&sv, delim, &chunk))
		return chunk;
	else
		return *sv;
	}
*/

StringView svChopLeft(StringView* sv, size_t count)
{
	if(sv->m_count < count) count = sv->m_count;

	StringView result = svFromParts(sv->m_data, count);

	sv->m_data += count;
	sv->m_count -= count;

	return result;
}

StringView svChopRight(StringView* sv, size_t count)
{
	if(sv->m_count < count) count = sv->m_count;

	StringView result = svFromParts(sv->m_data + sv->m_count - count, count);

	sv->m_count -= count;

	return result;
}

StringView svChopLeftWhile(StringView* sv, bool (*predicate)(char x))
{
	size_t i = 0;
	while(i < sv->m_count && predicate(sv->m_data[i])) i += 1;

	return svChopLeft(sv, i);
}

bool svIndexOf(StringView sv, char c, size_t* index)
{
	size_t i = 0;
	while(i < sv.m_count && sv.m_data[i] != c) i += 1;

	if(i < sv.m_count)
	{
		if(index) *index = i;
		return true;
	}

	return false;
}

bool svIsEqual(StringView a, StringView b)
{
	if(a.m_count != b.m_count) return false;

	if(a.m_count == 0) return true;

	return memcmp(a.m_data, b.m_data, a.m_count) == 0;
}

bool scIsEqualIgnoreCase(StringView a, StringView b)
{
	if(a.m_count != b.m_count)
	{
		return false;
	}

	char x, y;
	for(size_t i = 0; i < a.m_count; i++)
	{
		x = 'A' <= a.m_data[i] && a.m_data[i] <= 'Z' ? a.m_data[i] + 32 : a.m_data[i];

		y = 'A' <= b.m_data[i] && b.m_data[i] <= 'Z' ? b.m_data[i] + 32 : b.m_data[i];

		if(x != y) return false;
	}
	return true;
}

/*
	*   @brief: the above implementation has an extra safety check
		for the case when a.m_count == 0. which may be an extra comparison that is not needed.
bool svIsEqual(StringView a, StringView b)
{
	if (a.m_count != b.m_count)
		return false;

	return memcmp(a.m_data, b.m_data, a.m_count) == 0;
}
*/

bool svStartsWith(StringView sv, StringView expected_prefix)
{
	if(expected_prefix.m_count <= sv.m_count)
	{
		StringView actual_prefix = svFromParts(sv.m_data, expected_prefix.m_count);
		return svIsEqual(expected_prefix, actual_prefix);
	}

	return false;
}

bool svEndsWith(StringView sv, StringView suffix)
{
	if(sv.m_count >= suffix.m_count)
	{
		StringView sv_Suffix = svFromParts(sv.m_data + sv.m_count - suffix.m_count, suffix.m_count);
		return svIsEqual(sv_Suffix, suffix);
	}

	return false;
}

uint64_t StringViewToU64(StringView sv)
{
	uint64_t result = 0;
	for(size_t i = 0; i < sv.m_count && isdigit(sv.m_data[i]); ++i)
		result = result * 10 + (sv.m_data[i] - '0');

	return result;
}

/*
	*   @brief: the implementation belows should work logically
		but remains untested.

	Todo: test the following implementation.
uint64_t StringViewTouint64(StringView sv)
{
	uint64_t result = 0;
	for (size_t i = 0; i < sv.m_count; i++)
	{
		char c = sv.m_data[i];
		if (c < '0' || c > '9')
			return 0;

		result = result * 10 + (c - '0');
	}

	return result;
}
*/