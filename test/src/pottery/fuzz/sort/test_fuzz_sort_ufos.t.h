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

#ifndef TEST_POTTERY_FUZZ_SORT_UFO_PREFIX
    #error "Misconfigured template"
#endif

#include <stdio.h>
#include "pottery/fuzz/test_fuzz_common.h"

#ifndef TEST_POTTERY_FUZZ_MAIN
    #include "pottery/unit/test_pottery_framework.h"
#endif

#ifndef FUZZ_SORT_UFO_COUNT_LIMIT
    #ifdef TEST_POTTERY_FUZZ_MAIN
        #define FUZZ_SORT_UFO_COUNT_LIMIT 20000
    #else
        #define FUZZ_SORT_UFO_COUNT_LIMIT 1000
    #endif
#endif

#ifndef FUZZ_SORT_IS_STABLE
    #define FUZZ_SORT_IS_STABLE 0
#endif

// The array to be sorted.
typedef struct source_t {
    ufo_t* array;
    size_t count;
} source_t;

// Checks that the ufo array is sorted.
static void fuzz_check(ufo_t* ufos, size_t count) {
    size_t i;
    for (i = 1; i < count; ++i) {
        pottery_test_assert(0 <= ufo_compare_by_string(&ufos[i], &ufos[i - 1]));

        #if FUZZ_SORT_IS_STABLE
        if (0 == strcmp(ufos[i].string, ufos[i - 1].string)) {
            //printf("matching ufos %i %i\n", ufos[i].integer, ufos[i - 1].integer); fflush(stdout);
            pottery_test_assert(ufos[i].integer > ufos[i - 1].integer);
        }
        #endif
    }
}

typedef enum command_t {
    command_shuffle, // shuffle a range of elements
    command_reverse, // reverse a range of elements
    #if 0
    command_swap, // swap two ranges of elements
    command_rotate, // rotate a range of elements
    #endif
    command_count,
} command_t;

static void fuzz_shuffle(fuzz_input_t* input, source_t* source) {
    size_t count = fuzz_load_u16(input) % source->count;
    if (count <= 1)
        return;
    size_t start = fuzz_load_u16(input) % (source->count - count);
    srand(fuzz_load_u16(input));

    size_t i;
    for (i = 0; i < count; ++i) {
        size_t j = pottery_cast(size_t, rand()) % (count - i);
        ufo_swap_strings(source->array + start + i, source->array + start + j);
    }
}

static void fuzz_reverse(fuzz_input_t* input, source_t* source) {
    size_t count = fuzz_load_u16(input) % source->count;
    if (count <= 1)
        return;
    size_t start = fuzz_load_u16(input) % (source->count - count);

    size_t i;
    for (i = 0; i < count / 2; ++i) {
        ufo_swap_strings(source->array + start + i, source->array + start + count - i - 1);
    }
}

#if 0
static void fuzz_swap(fuzz_input_t* input, source_t* source) {
    fuzz_load_u8(input);
    (void)input;
    (void)source;
}

static void fuzz_rotate(fuzz_input_t* input, source_t* source) {
    fuzz_load_u8(input);
    (void)input;
    (void)source;
}
#endif

static void fuzz(fuzz_input_t* input) {

    // The first bytes are the array count. (One of our initial test cases is
    // all zeroes, and these load little-endian, so permuting the first byte
    // should give lots of small array sizes to sort to test corner cases.)
    size_t count = fuzz_load_u16(input) % FUZZ_SORT_UFO_COUNT_LIMIT;
    source_t source = {
        pottery_cast(ufo_t*, malloc(sizeof(ufo_t) * count)),
        count,
    };

    if (count > 0) {

        // The next byte is the max number of duplicates we can have. We rotate
        // through this with a large prime to have different numbers of duplicates
        // of each element.
        size_t max_duplicates = 1 + fuzz_load_u8(input) % 64;
        size_t prime = 3643;

        size_t duplicates = 1 + (prime % max_duplicates);
        //printf("starting with %zi duplicates\n", duplicates);
        size_t duplicate_index = 0;
        size_t i;
        unsigned int ufo_name = 0;
        for (i = 0; i < count; ++i) {
            char buf[16];
            snprintf(buf, sizeof(buf), "%06u", ufo_name);

            // The index is used as the ufo integer and we only swap ufo
            // strings. This way we can verify that stable sorts preserved
            // order.
            ufo_init(source.array + i, buf, pottery_cast(int32_t, i));

            //printf("%u ufo %s %i\n", (unsigned int)i, buf, pottery_cast(int32_t, i));
            if (++duplicate_index == duplicates) {
                ++ufo_name;
                duplicate_index = 0;
                duplicates = 1 + (duplicates - 1 + prime) % max_duplicates;
                //printf("now %zi duplicates\n", duplicates);
            }
        }

        // Commands perform various motions on the source array.
        int commands = 0;
        while (input->pos != input->end) {
            ++commands;
            int command = *(input->pos++) % pottery_cast(int, command_count);
            switch (pottery_cast(command_t, command)) {
                case command_shuffle:
                    fuzz_shuffle(input, &source);
                    break;
                case command_reverse:
                    fuzz_reverse(input, &source);
                    break;
                #if 0
                case command_swap:
                    fuzz_swap(input, &source);
                    break;
                case command_rotate:
                    fuzz_rotate(input, &source);
                    break;
                #endif
                default:
                    pottery_test_assert(false);
                    break;
            }
        }

        (void)commands;
        //printf("%i commands run\n", commands);

    }

    // Sort!
    sort_ufos_by_string(source.array, source.count);

    // Make sure that it sorted!
    fuzz_check(source.array, source.count);

    size_t i;
    for (i = 0; i < source.count; ++i)
        ufo_destroy(source.array + i);
    free(source.array);
}

#define TEST_POTTERY_FUZZ_TEST_NAME TEST_POTTERY_FUZZ_SORT_UFO_PREFIX
#include "pottery/fuzz/test_pottery_fuzz_harness.t.h"

#undef TEST_POTTERY_FUZZ_SORT_UFO_PREFIX
