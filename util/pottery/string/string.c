/*
 * MIT License
 *
 * Copyright (c) 2020-2021 11037650 Canada Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// We need stdio.h for vsnprintf()
#include <stdio.h>

#include "string.h"

#define POTTERY_VECTOR_PREFIX string_storage
#define POTTERY_VECTOR_VALUE_TYPE char
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1
#define POTTERY_VECTOR_INTERNAL_CAPACITY POTTERY_STRING_INTERNAL_CAPACITY
#include "pottery/vector/pottery_vector_define.t.h"

void string_init(string_t* string) {
    string_storage_init(&string->storage);
    string_abort_on_error(string_storage_insert_last(&string->storage, '\0'));
}

void string_init_cstr(string_t* string, const char* cstr) {
    string_storage_init(&string->storage);
    string_set_cstr(string, cstr);
}

void string_init_chars(string_t* string, const char* data, size_t count) {
    string_storage_init(&string->storage);
    string_abort_on_error(string_storage_reserve(&string->storage, count + 1));
    string_abort_on_error(string_storage_insert_last_bulk(&string->storage, data, count));
    string_abort_on_error(string_storage_insert_last(&string->storage, '\0'));
}

void string_set_chars(string_t* string, const char* chars, size_t count) {
    string_storage_remove_all(&string->storage);
    char* emplace;
    string_abort_on_error(string_storage_emplace_last_bulk(&string->storage, count + 1, &emplace));
    memcpy(emplace, chars, count);
    emplace[count] = '\0';
}

void string_set_cstr(string_t* string, const char* cstr) {
    string_storage_remove_all(&string->storage);
    size_t size = strlen(cstr) + 1;
    char* emplace;
    string_abort_on_error(string_storage_emplace_last_bulk(&string->storage, size, &emplace));
    memcpy(emplace, cstr, size);
}

void string_clear(string_t* string) {
    string_storage_remove_all(&string->storage);
    string_abort_on_error(string_storage_insert_last(&string->storage, '\0'));
}

void string_set_length(string_t* string, size_t length) {
    if (length > string_length(string)) {
        char* space;
        string_abort_on_error(string_storage_emplace_last_bulk(&string->storage, length - string_length(string), &space));
        (void)space;
    } else {
        string_storage_remove_last_bulk(&string->storage, string_length(string) - length);
    }
    *string_storage_last(&string->storage) = '\0';
}

bool string_equal_bytes(const string_t* left, const char* right_bytes, size_t right_length) {
    const char* left_bytes = string_cstr(left);
    if (left_bytes == right_bytes)
        return true;

    size_t left_length = string_length(left);
    if (left_length != right_length)
        return false;

    return 0 == memcmp(left_bytes, right_bytes, left_length);
}

int string_compare(const string_t* left, const string_t* right) {
    if (left == right)
        return 0;

    size_t left_length = string_length(left);
    size_t right_length = string_length(right);
    size_t min_length = (left_length < right_length) ? left_length : right_length;

    int cmp = memcmp(string_cstr(left), string_cstr(right), min_length);
    if (cmp != 0)
        return cmp;

    if (left_length < right_length)
        return -1;
    if (left_length > right_length)
        return 1;
    return 0;
}

void string_lstrip_chars(string_t* string, const char* chars, size_t char_count) {
    size_t strip_count = 0;

    while (string_length(string) > strip_count) {
        char candidate = string_cstr(string)[strip_count];
        bool found = false;
        size_t i;
        for (i = 0; i < char_count; ++i) {
            if (chars[i] == candidate) {
                found = true;
                break;
            }
        }
        if (!found)
            break;
        ++strip_count;
    }

    string_remove_first(string, strip_count);
}

void string_rstrip_chars(string_t* string, const char* chars, size_t char_count) {
    size_t strip_count = 0;

    while (string_length(string) > strip_count) {
        char candidate = string_bytes(string)[string_length(string) - 1 - strip_count];
        bool found = false;
        size_t i;
        for (i = 0; i < char_count; ++i) {
            if (chars[i] == candidate) {
                found = true;
                break;
            }
        }
        if (!found)
            break;
        ++strip_count;
    }

    string_remove_last(string, strip_count);
}

size_t string_find_char(string_t* string, size_t index, char c) {
    for (; index < string_length(string); ++index)
        if (string_bytes(string)[index] == c)
            break;
    return index;
}

size_t string_find_not_char(string_t* string, size_t index, char c) {
    for (; index < string_length(string); ++index)
        if (string_bytes(string)[index] != c)
            break;
    return index;
}

size_t string_find_chars(string_t* string, size_t index, const char* chars, size_t char_count) {
    for (; index < string_length(string); ++index) {
        char candidate = string_bytes(string)[index];
        bool found = false;
        size_t i;
        for (i = 0; i < char_count; ++i) {
            if (chars[i] == candidate) {
                found = true;
                break;
            }
        }
        if (!found)
            break;
    }
    return index;
}



/*
 * String formatting
 *
 * The core implementation is based around append(). insert() wraps it using a
 * temporary to avoid having to repeatedly move around string data after the
 * insertion point.
 */

#ifndef __CLR_VER

#ifdef __GNUC__
__attribute__((__format__(__printf__, 2, 0)))
#endif
void string_append_vformat(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        va_list args)
{
    size_t original_length = string_length(string);

    // We start by guessing how much space we'll need based on the length of
    // the format string. (We keep it small enough that very short format
    // strings like "%d" can fit within our internal space so we can format
    // short strings without allocation.)
    size_t buffer_length = strlen(format);
    buffer_length += 6 + buffer_length / 8;
    size_t total_length = original_length + buffer_length;
    if (total_length < original_length)
        abort();
    if (total_length < POTTERY_STRING_INTERNAL_CAPACITY - 1)
        total_length = POTTERY_STRING_INTERNAL_CAPACITY - 1;
    string_set_length(string, total_length);

    // Try to format in the existing space
    va_list temp_args;
    va_copy(temp_args, args);
    int ret = vsnprintf(string_bytes(string) + original_length, buffer_length, format, temp_args);
    if (ret < 0)
        abort();
    va_end(temp_args);

    // If it fit, we're done.
    if (pottery_cast(size_t, ret) < buffer_length) {
        string_set_length(string, original_length + pottery_cast(size_t, ret));
        return;
    }

    // Otherwise grow to the necessary size and format again.
    buffer_length = pottery_cast(size_t, ret) + 1;
    total_length = original_length + buffer_length;
    if (total_length < original_length)
        abort();
    string_set_length(string, total_length);
    ret = vsnprintf(string_bytes(string) + original_length, buffer_length, format, args);
    if (ret < 0 || pottery_cast(size_t, ret) != buffer_length - 1)
        abort();
}

#ifdef __GNUC__
__attribute__((__format__(__printf__, 2, 3)))
#endif
void string_append_format(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        ...)
{
    va_list args;
    va_start(args, format);
    string_append_vformat(string, format, args);
    va_end(args);
}

#ifdef __GNUC__
__attribute__((__format__(__printf__, 2, 0)))
#endif
void string_set_vformat(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        va_list args)
{
    string_clear(string);
    string_append_vformat(string, format, args);
}

#ifdef __GNUC__
__attribute__((__format__(__printf__, 2, 3)))
#endif
void string_set_format(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        ...)
{
    va_list args;
    va_start(args, format);
    string_set_vformat(string, format, args);
    va_end(args);
}

#ifdef __GNUC__
__attribute__((__format__(__printf__, 2, 0)))
#endif
void string_init_vformat(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        va_list args)
{
    string_init(string);
    string_append_vformat(string, format, args);
}

#ifdef __GNUC__
__attribute__((__format__(__printf__, 2, 3)))
#endif
void string_init_format(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        ...)
{
    va_list args;
    va_start(args, format);
    string_init_vformat(string, format, args);
    va_end(args);
}

#ifdef __GNUC__
__attribute__((__format__(__printf__, 3, 0)))
#endif
void string_insert_vformat(string_t* string,
        size_t index,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        va_list args)
{
    string_t temp;
    string_init(&temp);
    string_append_vformat(&temp, format, args);
    string_insert_string(string, index, &temp);
    string_destroy(&temp);
}

#ifdef __GNUC__
__attribute__((__format__(__printf__, 3, 4)))
#endif
void string_insert_format(string_t* string,
        size_t index,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        ...)
{
    va_list args;
    va_start(args, format);
    string_insert_vformat(string, index, format, args);
    va_end(args);
}

#endif // !defined(__CLR_VER)
