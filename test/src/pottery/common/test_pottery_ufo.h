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

#ifndef TEST_POTTERY_MOVER_H
#define TEST_POTTERY_MOVER_H 1

#include "pottery/pottery_dependencies.h"
#include "pottery/unit/test_pottery_framework.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * An unidentified flying object.
 *
 * UFO is our standard test object for unit tests and fuzz tests. It contains a
 * string and an integer, either of which can be used as the key in a map or
 * for sorting purposes.
 *
 * The UFO performs allocations, detects memory leaks, and detects improper
 * (bitwise) moves. Call ufo_check() to make sure the ufo is valid.
 *
 * All lifecycle functions are defined. Test containers can be instantiated
 * with only a subset of these functions.
 */
typedef struct ufo_t {
    struct ufo_t* self;
    char* string;
    int32_t integer;
} ufo_t;

static inline void ufo_check(const ufo_t* ufo) {
    pottery_test_assert(ufo != pottery_null);
    pottery_test_assert(ufo->self == ufo);
    pottery_test_assert(ufo->string != pottery_null);
    // force a read from our string that the compiler can't optimize away. it
    // must have at least a null-terminator.
    *pottery_reinterpret_cast(volatile char*, ufo->string);
}

static inline void ufo_destroy(ufo_t* ufo) {
    ufo_check(ufo);
    ufo->self = pottery_reinterpret_cast(ufo_t*, -1);
    pottery_test_free(ufo->string);
    // we don't clear out the allocated string pointer in order to let
    // instrumentation detect use-after-free.
}

static inline pottery_error_t ufo_init(ufo_t* ufo, const char* string, int32_t integer) {
    ufo->self = ufo;
    ufo->string = pottery_test_strdup(string);
    ufo->integer = integer;
    return POTTERY_OK;
}

static inline pottery_error_t ufo_init_copy(ufo_t* to, const ufo_t* from) {
    pottery_assert(to != from);
    ufo_check(from);
    to->self = to;
    to->string = pottery_test_strdup(from->string);
    to->integer = from->integer;
    return POTTERY_OK;
}

static inline pottery_error_t ufo_init_steal(ufo_t* to, ufo_t* from) {
    pottery_assert(to != from);
    ufo_check(from);

    to->self = to;
    to->string = from->string;
    from->string = pottery_test_strdup("");

    to->integer = from->integer;

    return POTTERY_OK;
}

static inline void ufo_move(ufo_t* to, ufo_t* from) {
    pottery_assert(to != from);
    ufo_check(from);

    to->self = to;
    to->string = from->string;
    to->integer = from->integer;

    from->self = pottery_reinterpret_cast(ufo_t*, -1);
    // we do clear out the allocated pointer here because the old pointer is
    // still alive, so we wouldn't detect use-after-free.
    from->string = pottery_reinterpret_cast(char*, -1);
}

static inline pottery_error_t ufo_copy(ufo_t* to, const ufo_t* from) {
    pottery_assert(to != from);
    ufo_check(to);
    ufo_check(from);

    char* string = pottery_test_strdup(from->string);
    if (string == pottery_null)
        return POTTERY_ERROR_ALLOC;
    pottery_test_free(to->string);
    to->string = string;

    to->integer = from->integer;

    return POTTERY_OK;
}

static inline void ufo_steal(ufo_t* to, ufo_t* from) {
    pottery_assert(to != from);
    ufo_check(to);
    ufo_check(from);

    // we can't allocate here so we swap.
    char* string = to->string;
    to->string = from->string;
    from->string = string;

    to->integer = from->integer;
}

static inline void ufo_swap(ufo_t* left, ufo_t* right) {
    pottery_assert(left != right);
    ufo_check(left);
    ufo_check(right);

    char* string = left->string;
    left->string = right->string;
    right->string = string;

    int32_t integer = left->integer;
    left->integer = right->integer;
    right->integer = integer;
}

static inline int ufo_compare(const ufo_t* left, const ufo_t* right) {
    ufo_check(left);
    ufo_check(right);
    int cmp = strcmp(left->string, right->string);
    if (cmp != 0)
        return cmp;
    return left->integer - right->integer;
}

static inline int ufo_compare_by_string(const ufo_t* left, const ufo_t* right) {
    ufo_check(left);
    ufo_check(right);
    return strcmp(left->string, right->string);
}

static inline int ufo_compare_by_integer(const ufo_t* left, const ufo_t* right) {
    ufo_check(left);
    ufo_check(right);
    return left->integer - right->integer;
}

static inline int ufo_equal(const ufo_t* left, const ufo_t* right) {
    return left->integer == right->integer && 0 == strcmp(left->string, right->string);
}

static inline void ufo_swap_strings(ufo_t* left, ufo_t* right) {
    ufo_check(left);
    ufo_check(right);

    char* string = left->string;
    left->string = right->string;
    right->string = string;
}

#ifdef __cplusplus
}
#endif

#endif
