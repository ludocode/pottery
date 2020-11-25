/*
 * MIT License
 *
 * Copyright (c) 2020 Nicholas Fraser
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

#ifndef POTTERY_EXAMPLES_STRING_STRING_H
#define POTTERY_EXAMPLES_STRING_STRING_H 1

#include <stdarg.h>
#include "pottery/pottery_dependencies.h"

#ifdef __cplusplus
extern "C" {
#endif

// To store the string data we use a Pottery vector of char with some internal
// storage space. This gives us a small string optimization similar to most
// implementations of std::string. It also makes our string not bitwise
// movable so you can only move it with string_move()!

// If you override this, make sure it's the same in all translation units!
#ifndef POTTERY_STRING_INTERNAL_CAPACITY
#define POTTERY_STRING_INTERNAL_CAPACITY 16
#endif

#define POTTERY_VECTOR_PREFIX string_storage
#define POTTERY_VECTOR_VALUE_TYPE char
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1
#define POTTERY_VECTOR_INTERNAL_CAPACITY POTTERY_STRING_INTERNAL_CAPACITY
#include "pottery/vector/pottery_vector_declare.t.h"

typedef struct string_t {
    string_storage_t storage;
} string_t;

// internal helper function to abort on error
static pottery_always_inline
void string_abort_on_error(pottery_error_t error) {
    if (pottery_unlikely(error != POTTERY_OK))
        pottery_abort();
}

/*
 * Accessor functions
 */

/**
 * Returns a mutable pointer to the bytes contained in the string.
 *
 * The returned data is valid until the string is modified.
 *
 * For a const pointer, use string_cstr() instead.
 */
static inline char* string_bytes(string_t* string) {
    return string_storage_data(&string->storage);
}

/**
 * Returns the length of the string (not counting the null-terminator.)
 */
static inline size_t string_length(const string_t* string) {
    size_t count = string_storage_count(&string->storage);
    pottery_assert(count > 0);
    return count - 1;
}

/**
 * Sets the length of the string to the given length.
 *
 * If this is shorter than the current length, the string is truncated. If this
 * is longer than the current length, the string is extended with uninitialized
 * space (but a null-terminator will be preserved as usual after the
 * uninitialized space, so you only need to initialize the added space.)
 */
void string_set_length(string_t* string, size_t length);

/**
 * Returns a pointer to the contents of the string as a null-terminated
 * C string.
 *
 * The returned C string is valid until the string is modified.
 */
static inline const char* string_cstr(const string_t* string) {
    return string_storage_const_data(&string->storage);
}

bool string_equal_bytes(const string_t* left, const char* right, size_t right_length);

static inline bool string_equal_cstr(const string_t* left, const char* right) {
    return string_equal_bytes(left, right, strlen(right));
}

static inline bool string_equal(const string_t* left, const string_t* right) {
    return string_equal_bytes(left, string_cstr(right), string_length(right));
}

static inline bool string_equal_char(const string_t* string, char c) {
    return string_length(string) == 1 && string_cstr(string)[0] == c;
}

int string_compare(const string_t* left, const string_t* right);

/**
 * Strips chars from the start of the string that match any char in the given
 * array.
 */
void string_lstrip_chars(string_t* string, const char* chars, size_t char_count);

static inline void string_lstrip_chars_cstr(string_t* string, const char* chars_cstr) {
    string_lstrip_chars(string, chars_cstr, strlen(chars_cstr));
}

/**
 * Strips chars from the end of the string that match any char in the given
 * array.
 */
void string_rstrip_chars(string_t* string, const char* chars, size_t char_count);

static inline void string_rstrip_chars_cstr(string_t* string, const char* chars_cstr) {
    string_rstrip_chars(string, chars_cstr, strlen(chars_cstr));
}

/**
 * Strips chars from the start and end of the string that match any char in the
 * given array.
 */
static inline void string_strip_chars(string_t* string, const char* chars, size_t char_count) {
    string_lstrip_chars(string, chars, char_count);
    string_rstrip_chars(string, chars, char_count);
}

static inline void string_strip_chars_cstr(string_t* string, const char* chars_cstr) {
    string_strip_chars(string, chars_cstr, strlen(chars_cstr));
}

/*
 * Mutation functions
 */

void string_clear(string_t* string);

static inline void string_insert_char(string_t* string, size_t index, char c) {
    string_abort_on_error(string_storage_insert_at(&string->storage, index, c));
}

static inline void string_insert_chars(string_t* string, size_t index, const char* chars, size_t count) {
    string_abort_on_error(string_storage_insert_at_bulk(&string->storage, index, chars, count));
}

static inline void string_append_char(string_t* string, char c) {
    string_insert_char(string, string_length(string), c);
}

static inline void string_append_chars(string_t* string, const char* chars, size_t count) {
    string_insert_chars(string, string_length(string), chars, count);
}

static inline void string_insert_cstr(string_t* string, size_t index, const char* cstr) {
    string_insert_chars(string, index, cstr, strlen(cstr));
}

static inline void string_append_cstr(string_t* string, const char* cstr) {
    string_append_chars(string, cstr, strlen(cstr));
}

static inline void string_insert_string(string_t* string, size_t index, const string_t* other) {
    string_insert_chars(string, index, string_cstr(other), string_length(other));
}

static inline void string_append_string(string_t* string, const string_t* other) {
    string_insert_string(string, string_length(string), other);
}

/**
 * Inserts a substring of another string into this one.
 */
static inline void string_insert_substr(string_t* string, size_t index,
        const string_t* other, size_t other_index, size_t count)
{
    string_insert_chars(string, index, string_cstr(other) + other_index, count);
}

static inline void string_append_substr(string_t* string,
        const string_t* other, size_t other_index, size_t count)
{
    string_insert_substr(string, string_length(string), other, other_index, count);
}

void string_set_chars(string_t* string, const char* chars, size_t count);

void string_set_cstr(string_t* string, const char* cstr);

/**
 * Removes a range of characters from the string.
 */
static inline void string_remove(string_t* string, size_t index, size_t count) {
    pottery_assert(index + count >= index); // don't overflow!
    pottery_assert(index + count <= string_length(string)); // don't remove the null-terminator!
    string_storage_remove_at_bulk(&string->storage, index, count);
}

static inline void string_remove_first(string_t* string, size_t count) {
    string_remove(string, 0, count);
}

static inline void string_remove_last(string_t* string, size_t count) {
    string_remove(string, string_length(string) - count, count);
}

/*
 * Lifecycle functions
 */

static inline void string_destroy(string_t* string) {
    string_storage_destroy(&string->storage);
}

/**
 * Initializes the string with no contents (i.e. the empty string.)
 */
void string_init_blank(string_t* string);

void string_init_cstr(string_t* string, const char* cstr);

void string_init_chars(string_t* string, const char* data, size_t length);

static inline void string_init_copy(string_t* string, const string_t* other) {
    string_abort_on_error(string_storage_init_copy(&string->storage, &other->storage));
}

static inline void string_init_steal(string_t* string, string_t* other) {
    string_storage_init_steal(&string->storage, &other->storage);
}

static inline void string_move(string_t* to, string_t* from) {
    string_storage_move(&to->storage, &from->storage);
}

static inline void string_copy(string_t* to, const string_t* from) {
    string_abort_on_error(string_storage_copy(&to->storage, &from->storage));
}

static inline void string_steal(string_t* to, string_t* from) {
    string_storage_steal(&to->storage, &from->storage);
}

static inline void string_swap(string_t* to, string_t* from) {
    string_storage_swap(&to->storage, &from->storage);
}

/**
 * Finds the given char starting at the given index.
 *
 * Returns the length of the string if not found.
 */
size_t string_find_char(string_t* string, size_t index, char c);

/**
 * Finds the first index that is not the given char starting at the given index.
 *
 * Returns the length of the string if the entire rest of the string consists
 * of the given char.
 *
 * This is the opposite of string_find_char().
 */
size_t string_find_not_char(string_t* string, size_t index, char c);

/**
 * Find any of the given chars in the string starting at the given index.
 *
 * Returns the length of the string if none are found.
 */
size_t string_find_chars(string_t* string, size_t index, const char* chars, size_t char_count);

static inline size_t string_find_chars_cstr(string_t* string, size_t index, const char* chars) {
    return string_find_chars(string, index, chars, strlen(chars));
}

#ifndef __CLR_VER

/**
 * Inserts a formatted string.
 *
 * @see string_append_vformat()
 */
void string_insert_vformat(string_t* string,
        size_t index,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        va_list args)
        #ifdef __GNUC__
        __attribute__((__format__(__printf__, 3, 0)))
        #endif
        ;

/**
 * Insert a formatted string.
 *
 * @see string_append_vformat()
 */
void string_insert_format(string_t* string,
        size_t index,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        ...)
        #ifdef __GNUC__
        __attribute__((__format__(__printf__, 3, 4)))
        #endif
        ;

/**
 * Appends a formatted string.
 *
 * This uses vsnprintf() to append the formatted string. Note that vsnprintf()
 * is not entirely portable. Be careful to avoid platform-specific format
 * extensions for maximum portability.
 */
void string_append_vformat(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        va_list args)
        #ifdef __GNUC__
        __attribute__((__format__(__printf__, 2, 0)))
        #endif
        ;

/**
 * Appends a formatted string.
 *
 * @see string_append_vformat()
 */
void string_append_format(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        ...)
        #ifdef __GNUC__
        __attribute__((__format__(__printf__, 2, 3)))
        #endif
        ;

/**
 * Sets a string to a formatted string, replacing whatever it contains.
 *
 * @see string_append_vformat()
 */
void string_set_vformat(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        va_list args)
        #ifdef __GNUC__
        __attribute__((__format__(__printf__, 2, 0)))
        #endif
        ;

/**
 * Sets a string to a formatted string, replacing whatever it contains.
 *
 * @see string_append_vformat()
 */
void string_set_format(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        ...)
        #ifdef __GNUC__
        __attribute__((__format__(__printf__, 2, 3)))
        #endif
        ;

/**
 * Initializes a string to a formatted string.
 *
 * @see string_append_vformat()
 */
void string_init_vformat(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        va_list args)
        #ifdef __GNUC__
        __attribute__((__format__(__printf__, 2, 0)))
        #endif
        ;

/**
 * Initializes a string to a formatted string.
 *
 * @see string_append_vformat()
 */
void string_init_format(string_t* string,
        #ifdef __MSC_VER
        _Printf_format_string_
        #endif
        const char* format,
        ...)
        #ifdef __GNUC__
        __attribute__((__format__(__printf__, 2, 3)))
        #endif
        ;

#endif // !defined(__CLR_VER)

#ifdef __cplusplus
}
#endif

#endif
