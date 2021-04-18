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

#include "pottery/string/string.h"

#include "pottery/unit/test_pottery_framework.h"

POTTERY_TEST(pottery_string) {

    // Initialize a blank string
    string_t homer;
    string_init(&homer);

    // Append a C string
    string_append_cstr(&homer, "Homer");
    printf("%s\n", string_cstr(&homer));

    // Append more data
    string_append_char(&homer, ' ');
    string_append_cstr(&homer, "Simpson");
    pottery_test_assert(string_equal_cstr(&homer, "Homer Simpson"));
    printf("%s\n", string_cstr(&homer));

    // Insert data (enough to cause it to allocate)
    string_insert_cstr(&homer, 5, " J.");
    pottery_test_assert(string_equal_cstr(&homer, "Homer J. Simpson"));
    printf("%s\n", string_cstr(&homer));

    // Replace data
    string_remove(&homer, 6, 2);
    string_insert_cstr(&homer, 6, "Jay");
    pottery_test_assert(string_equal_cstr(&homer, "Homer Jay Simpson"));
    printf("%s\n", string_cstr(&homer));

    #ifndef __CLR_VER
    // Append lots more data via format strings, causing it to grow
    string_append_format(&homer, " works at the %s %s.", "Springfield", "Nuclear Power Plant");
    printf("%s\n", string_cstr(&homer));
    string_insert_format(&homer, 23, " in sector %01d%c", 7, 'G');
    printf("%s\n", string_cstr(&homer));
    #endif

    // Clean up
    string_destroy(&homer);
}
