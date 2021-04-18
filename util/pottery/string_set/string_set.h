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

#ifndef POTTERY_EXAMPLE_STRING_SET_H
#define POTTERY_EXAMPLE_STRING_SET_H

#include <stdbool.h>

/**
 * A set of unique strings.
 */
typedef struct string_set_t string_set_t;

/**
 * Creates a new set of unique strings.
 */
string_set_t* string_set_new(void);

/**
 * Deletes a set of unique strings.
 */
void string_set_delete(string_set_t* set);

/**
 * Adds a string to the set.
 *
 * If a memory allocation error occurs, the program aborts.
 *
 * @return true if the string was added, false if it was already in the set.
 */
bool string_set_add(string_set_t* set, const char* string);

/**
 * Queries whether a string exists in the set.
 *
 * @return true if the string exists in the set, false if it does not.
 */
bool string_set_query(string_set_t* set, const char* string);

/**
 * Removes a string from the set.
 *
 * @return true if the string was removed, false if it was not in the set.
 */
bool string_set_remove(string_set_t* set, const char* string);

#endif
