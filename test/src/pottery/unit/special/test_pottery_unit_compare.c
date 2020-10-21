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

#include "pottery/common/test_pottery_ufo.h"
#include "pottery/unit/test_pottery_framework.h"

// Instantiate string comparison functions
#define POTTERY_COMPARE_PREFIX string
#define POTTERY_COMPARE_REF_TYPE const char*
#define POTTERY_COMPARE_THREE_WAY strcmp
#include "pottery/compare/pottery_compare_static.t.h"
#include "pottery/compare/pottery_compare_cleanup.t.h"

POTTERY_TEST(pottery_compare_strcmp) {
    pottery_test_assert(string_equal("hello", "hello"));
    pottery_test_assert(string_not_equal("hello", "world"));

    pottery_test_assert(string_less("AAAA", "AAAZ"));
    pottery_test_assert(string_less_or_equal("AAAA", "AAAA"));
    pottery_test_assert(string_less_or_equal("AAAA", "AAAZ"));

    pottery_test_assert(string_greater("ZZZZ", "ZZZA"));
    pottery_test_assert(string_greater_or_equal("ZZZZ", "ZZZZ"));
    pottery_test_assert(string_greater_or_equal("ZZZZ", "ZZZA"));

    pottery_test_assert(0 > string_three_way("a", "z"));
    pottery_test_assert(0 < string_three_way("y", "b"));

    // Min/max
    pottery_test_assert(0 == strcmp("apple", string_min("apple", "banana")));
    pottery_test_assert(0 == strcmp("apple", string_min("banana", "apple")));
    pottery_test_assert(0 == strcmp("banana", string_max("apple", "banana")));
    pottery_test_assert(0 == strcmp("banana", string_max("banana", "apple")));

    // Clamp
    pottery_test_assert(0 == strcmp("carl", string_clamp("carl", "bob", "dave")));
    pottery_test_assert(0 == strcmp("bob", string_clamp("alice", "bob", "dave")));
    pottery_test_assert(0 == strcmp("dave", string_clamp("eve", "bob", "dave")));

    // Median
    pottery_test_assert(0 == strcmp("banana", string_median("apple", "banana", "carrot")));
    pottery_test_assert(0 == strcmp("banana", string_median("apple", "carrot", "banana")));
    pottery_test_assert(0 == strcmp("banana", string_median("banana", "apple", "carrot")));
    pottery_test_assert(0 == strcmp("banana", string_median("banana", "carrot", "apple")));
    pottery_test_assert(0 == strcmp("banana", string_median("carrot", "apple", "banana")));
    pottery_test_assert(0 == strcmp("banana", string_median("carrot", "banana", "apple")));
    pottery_test_assert(0 == strcmp("true", string_median("false", "true", "true")));
    pottery_test_assert(0 == strcmp("true", string_median("true", "false", "true")));
    pottery_test_assert(0 == strcmp("true", string_median("true", "true", "false")));
    pottery_test_assert(0 == strcmp("ok", string_median("ok", "ok", "ok")));
}

// Instantiate float comparison functions
#define POTTERY_COMPARE_PREFIX float
#define POTTERY_COMPARE_VALUE_TYPE float
#define POTTERY_COMPARE_BY_VALUE 1
#include "pottery/compare/pottery_compare_static.t.h"
#include "pottery/compare/pottery_compare_cleanup.t.h"

POTTERY_TEST(pottery_compare_floats) {
    // Note that because float is the value type, the generated compare
    // functions take pointers to floats. This is how a container of floats
    // would expect to call these functions.
    float one = 1.f;
    float pi = 3.14f;
    float twopi = 6.28f;
    pottery_test_assert(float_less(&pi, &twopi));
    pottery_test_assert(float_greater(&twopi, &pi));
    pottery_test_assert(float_equal(&one, &one));
    pottery_test_assert(float_greater_or_equal(&one, &one));
}

// Instantiate comparison functions for custom context-based comparison
// expressions. The context is a UFO array and the compare type is an index
// into it.
#define POTTERY_COMPARE_PREFIX ufo_array
#define POTTERY_COMPARE_CONTEXT_TYPE ufo_t*
#define POTTERY_COMPARE_REF_TYPE size_t
#define POTTERY_COMPARE_LESS(array, left, right) 0 > ufo_compare(array + left, array + right)
#define POTTERY_COMPARE_LESS_OR_EQUAL(array, left, right) 0 >= ufo_compare(array + left, array + right)
#include "pottery/compare/pottery_compare_static.t.h"
#include "pottery/compare/pottery_compare_cleanup.t.h"

// Obviously this would have been more efficient, but the above tests out one
// particular combination of two non-opposite expressions that generates all
// comparison functions.
//#define POTTERY_COMPARE_THREE_WAY(array, left, right) ufo_compare(array + left, array + right)

// Although, by defining just one more non-opposite operator we could have made
// it essentially as efficient as the three-way compare since every comparison
// function could be implemented by a single call to ufo_compare(). e.g.:
//#define POTTERY_COMPARE_NOT_EQUAL(array, left, right) 0 != ufo_compare(array + left, array + right)

POTTERY_TEST(pottery_compare_ufo_array) {
    ufo_t ufos[4];
    ufo_init(ufos + 0, "alice", 0);
    ufo_init(ufos + 1, "bob", 1);
    ufo_init(ufos + 2, "eve", 5);
    ufo_init(ufos + 3, "eve", 5); // duplicate!

    // We should have a full set of comparison functions available.
    pottery_test_assert(ufo_array_equal(ufos, 0, 0));
    pottery_test_assert(ufo_array_equal(ufos, 2, 3));
    pottery_test_assert(ufo_array_not_equal(ufos, 0, 1));
    pottery_test_assert(ufo_array_less(ufos, 0, 1));
    pottery_test_assert(ufo_array_less(ufos, 0, 2));
    pottery_test_assert(ufo_array_less_or_equal(ufos, 0, 3));
    pottery_test_assert(ufo_array_less_or_equal(ufos, 2, 3));
    pottery_test_assert(ufo_array_greater(ufos, 2, 1));
    pottery_test_assert(ufo_array_greater_or_equal(ufos, 3, 0));
    pottery_test_assert(ufo_array_greater_or_equal(ufos, 3, 2));
    pottery_test_assert(ufo_array_greater_or_equal(ufos, 2, 3));
    pottery_test_assert(0 == ufo_array_three_way(ufos, 2, 2));
    pottery_test_assert(0 == ufo_array_three_way(ufos, 2, 3));
    pottery_test_assert(0 > ufo_array_three_way(ufos, 1, 3));
    pottery_test_assert(0 < ufo_array_three_way(ufos, 2, 0));

    // min/max/median/etc should work with context
    pottery_test_assert(0 == strcmp("alice", ufos[ufo_array_min(ufos, 0, 1)].string));
    pottery_test_assert(0 == strcmp("bob", ufos[ufo_array_median(ufos, 0, 1, 3)].string));
    pottery_test_assert(0 == strcmp("bob", ufos[ufo_array_clamp(ufos, 2, 0, 1)].string));
    pottery_test_assert(0 == strcmp("eve", ufos[ufo_array_max(ufos, 3, 1)].string));

    size_t i;
    for (i = 0; i < sizeof(ufos) / sizeof(*ufos); ++i)
        ufo_destroy(ufos + i);
}
