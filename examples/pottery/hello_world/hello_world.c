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

#include <stdio.h>
#include <string.h>

#define POTTERY_VECTOR_PREFIX strings
#define POTTERY_VECTOR_VALUE_TYPE char*
#define POTTERY_VECTOR_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_VECTOR_LIFECYCLE_DESTROY(x) free(*x)
#include "pottery/vector/pottery_vector_static.t.h"

int main(void) {
    strings_t strings;
    strings_init(&strings);

    strings_insert_last(&strings, strdup("Hello"));
    strings_insert_last(&strings, strdup("world!")); // grows as needed

    printf("%s %s\n",
            *strings_at(&strings, 0),
            *strings_at(&strings, 1)); // prints "Hello world!"

    strings_destroy(&strings); // frees all strings
    return 0;
}
