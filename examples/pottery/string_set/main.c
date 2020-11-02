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

#include <stdlib.h>
#include <stdio.h>

#include "string_set.h"

int main(void) {
    string_set_t* string_set = string_set_new();

    string_set_add(string_set, "alice");
    string_set_add(string_set, "bob");

    string_set_add(string_set, "eve");
    string_set_remove(string_set, "eve");

    printf("%s: %s\n", "alice", string_set_query(string_set, "alice") ? "exists" : "does not exist");
    printf("%s: %s\n", "bob",   string_set_query(string_set, "bob")   ? "exists" : "does not exist");
    printf("%s: %s\n", "eve",   string_set_query(string_set, "eve")   ? "exists" : "does not exist");

    string_set_delete(string_set);
    return EXIT_SUCCESS;
}
