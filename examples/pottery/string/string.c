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

#include "string.h"

#define POTTERY_VECTOR_PREFIX string_storage
#define POTTERY_VECTOR_VALUE_TYPE char
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1
#define POTTERY_VECTOR_INTERNAL_CAPACITY 16
#include "pottery/vector/pottery_vector_define.t.h"

void string_init_blank(string_t* string) {
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

bool string_equal_bytes(const string_t* left, const char* right_bytes, size_t right_length) {
    const char* left_bytes = string_bytes(left);
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

    int cmp = memcmp(string_bytes(left), string_bytes(right), min_length);
    if (cmp != 0)
        return cmp;

    if (left_length < right_length)
        return -1;
    if (left_length > right_length)
        return 1;
    return 0;
}
