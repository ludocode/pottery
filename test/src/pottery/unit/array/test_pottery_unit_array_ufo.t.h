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

/*
 * Tests an array of ufo_t called ufo_array.
 *
 * This is used to test various array implementations.
 */

#ifndef POTTERY_TEST_ARRAY_UFO_PREFIX
#error "Misconfigured template"
#endif

#define POTTERY_TEST_ARRAY_UFO(x) \
    POTTERY_TEST(POTTERY_CONCAT(POTTERY_CONCAT(POTTERY_TEST_ARRAY_UFO_PREFIX, _), x))

POTTERY_TEST_ARRAY_UFO(init_destroy) {
    ufo_array_t array;
    ufo_array_init(&array);
    pottery_test_assert(ufo_array_count(&array) == 0);
    ufo_array_destroy(&array);
}

static inline void check_ufo_array(ufo_array_t* array) {
    size_t i;
    for (i = 0; i < ufo_array_count(array); ++i)
        ufo_check(ufo_array_at(array, i));
}

POTTERY_TEST_ARRAY_UFO(remove) {
    ufo_array_t array;
    ufo_array_init(&array);

    ufo_array_entry_t entry;

    pottery_test_assert(ufo_array_emplace_last(&array, &entry) == POTTERY_OK);
    ufo_init(ufo_array_entry_value(&array, entry), "hello", 1);
    pottery_test_assert(ufo_array_emplace_last(&array, &entry) == POTTERY_OK);
    ufo_init(ufo_array_entry_value(&array, entry), "world", 2);
    check_ufo_array(&array);

    pottery_test_assert(0 == strcmp(ufo_array_first(&array)->string, "hello"));
    pottery_test_assert(0 == strcmp(ufo_array_last(&array)->string, "world"));

    // Removing the first element should call its destroy function, then shift
    // the second one down, calling its move function.
    ufo_array_remove_at(&array, 0);
    check_ufo_array(&array);
    pottery_test_assert(0 == strcmp(ufo_array_first(&array)->string, "world"));

    // Destroying with contained elements should be allowed because we should
    // be instantiated with a destroy function. This should clean up the
    // remaining ufo.
    ufo_array_destroy(&array);
}

POTTERY_TEST_ARRAY_UFO(grow_and_shrink) {
    ufo_array_t array;
    ufo_array_init(&array);

    #ifdef POTTERY_TEST_ARRAY_UFO_HAS_CAPACITY
    size_t capacity = ufo_array_capacity(&array);
    int capacity_changes = 0;
    #endif

    // Add 1000 elements
    int16_t i;
    for (i = 0; i < 1000; ++i) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%i", i);

        ufo_array_entry_t entry;
        pottery_test_assert(ufo_array_emplace_last(&array, &entry) == POTTERY_OK);
        ufo_init(ufo_array_entry_value(&array, entry), buf, i);
        check_ufo_array(&array);

        // Track capacity changes
        #ifdef POTTERY_TEST_ARRAY_UFO_HAS_CAPACITY
        if (capacity != ufo_array_capacity(&array)) {
            capacity = ufo_array_capacity(&array);
            ++capacity_changes;
        }
        #endif
    }

    #ifdef POTTERY_TEST_ARRAY_UFO_HAS_CAPACITY
    // Make sure we grew a reasonable number of times
    pottery_test_assert(capacity_changes >= 3);
    pottery_test_assert(capacity_changes <= 20);
    capacity_changes = 0;
    #endif

    // Remove the middle element until the array is empty. This will shift all
    // remaining elements down, or will move the before and after chunks
    // separately when shrinking.
    while (!ufo_array_is_empty(&array)) {
        ufo_array_remove_at(&array, ufo_array_count(&array) / 2);
        check_ufo_array(&array);

        // Track capacity changes
        #ifdef POTTERY_TEST_ARRAY_UFO_HAS_CAPACITY
        if (capacity != ufo_array_capacity(&array)) {
            capacity = ufo_array_capacity(&array);
            ++capacity_changes;
        }
        #endif
    }

    #ifdef POTTERY_TEST_ARRAY_UFO_HAS_CAPACITY
    // Make sure we shrunk a reasonable number of times
    pottery_test_assert(capacity_changes >= 3);
    pottery_test_assert(capacity_changes <= 20);
    #endif

    ufo_array_destroy(&array);
}

#undef POTTERY_TEST_ARRAY_UFO

#undef POTTERY_TEST_ARRAY_UFO_HAS_CAPACITY
#undef POTTERY_TEST_ARRAY_UFO_PREFIX
