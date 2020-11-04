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

#ifndef TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX
#error "Misconfigured template"
#endif

#ifdef TEST_POTTERY_FUZZ_MAIN
    #include <stdio.h>
#else
    #include "pottery/unit/test_pottery_framework.h"
#endif

#include "pottery/fuzz/test_fuzz_common.h"

#define FUZZ_ARRAY_UFO_NAME_MAX_LENGTH 7
#define FUZZ_ARRAY_UFO_COUNT_LIMIT 1000000

// The shadow array. The contents of the real array much match this after every
// command.
typedef struct shadow_t {
    ufo_t* array;
    size_t count;
} shadow_t;

static void ufo_move_bulk_up(ufo_t* to, ufo_t* from, size_t count) {
    while (count != 0) {
        --count;
        ufo_move(to + count, from + count);
    }
}

static void ufo_move_bulk_down(ufo_t* to, ufo_t* from, size_t count) {
    for (; count > 0; --count) {
        ufo_move(to++, from++);
    }
}

// Performs sanity checks after every command and ensures that the real array
// matches the shadow array.
static void fuzz_check(array_ufo_t* array, shadow_t* shadow) {

    // TODO enable this (should exist in unit tests even in release builds)
    //array_ufo_check(array);

    pottery_test_assert(array_ufo_count(array) == shadow->count);

    ufo_t* expected = shadow->array;
    // TODO use entry! vector doesn't have entry yet
    ufo_t* entry = array_ufo_begin(array);
    for (; array_ufo_entry_exists(array, &entry); array_ufo_next(array, &entry)) {
        ufo_t* ufo = entry;
        pottery_test_assert(ufo_equal(ufo, expected++));
    }
}

typedef enum command_t {

    // these are our first two commands so that we can easily generate an afl
    // test case that grows a ton and then shrinks back down to nothing
    // (see test/tools/generate-corpus.sh)
    command_emplace_at,
    command_remove_at,

    command_shrink,
    command_reserve,

    #if 0
    command_emplace_first,
    command_emplace_last,
    command_emplace_before,
    command_emplace_after,
    #endif

    // remove

    // alloc failure
    //command_alloc_failure, // TODO alloc fail after n more allocations

    command_count,
} command_t;

static void fuzz_shrink(array_ufo_t* array) {
    array_ufo_shrink(array);
}

static void fuzz_reserve(array_ufo_t* array, fuzz_input_t* input) {
    // Reserve any number between 256 and (roughly) four times what the array
    // currently holds. Often we'll be reserving less (which should do
    // nothing), but usually we'll be reserving more, sometimes much more.
    size_t count = fuzz_load_u24(input) % (256 + 4 * array_ufo_count(array));
    array_ufo_reserve(array, count);
}

static void fuzz_emplace_at(array_ufo_t* array, fuzz_input_t* input, shadow_t* shadow) {
    if (shadow->count == FUZZ_ARRAY_UFO_COUNT_LIMIT)
        return;
    size_t pos = fuzz_load_u24(input) % (array_ufo_count(array) + 1);

    // create the ufo
    ufo_t ufo;
    if (!fuzz_ufo_init(&ufo, input))
        return;

    // emplace into real array
    //printf("emplacing %s at %zu of %zu\n", ufo.string, pos, array_ufo_count(array));
    array_ufo_entry_t entry;
    if (POTTERY_OK != array_ufo_emplace_at(array, pos, &entry)) {
        ufo_destroy(&ufo);
        return;
    }
    ufo_move(entry, &ufo);

    // insert into shadow array
    ufo_move_bulk_up(shadow->array + pos + 1, shadow->array + pos, shadow->count - pos);
    ufo_init_copy(shadow->array + pos, entry);
    ++shadow->count;
}

static void fuzz_remove_at(array_ufo_t* array, fuzz_input_t* input, shadow_t* shadow) {
    if (array_ufo_is_empty(array)) {
        pottery_test_assert(shadow->count == 0);
        //printf("array is empty, not removing\n");
        return;
    }

    size_t pos = fuzz_load_u24(input) % array_ufo_count(array);

    // remove from real array
    //printf("removing at %zu of %zu\n", pos, array_ufo_count(array));
    array_ufo_remove_at(array, pos);

    // remove from shadow array
    pottery_test_assert(pos < shadow->count);
    --shadow->count;
    ufo_destroy(shadow->array + pos);
    ufo_move_bulk_down(shadow->array + pos, shadow->array + pos + 1, shadow->count - pos);
}

static void fuzz(fuzz_input_t* input) {
    shadow_t shadow = {
        pottery_cast(ufo_t*, malloc(sizeof(ufo_t) * FUZZ_ARRAY_UFO_COUNT_LIMIT)),
        0
    };

    array_ufo_t array;
    array_ufo_init(&array);

    int commands = 0;
    while (input->pos != input->end) {
        ++commands;
        int command = *(input->pos++) % pottery_cast(int, command_count);
        switch (pottery_cast(command_t, command)) {

            case command_emplace_at:
                fuzz_emplace_at(&array, input, &shadow);
                break;

            case command_remove_at:
                fuzz_remove_at(&array, input, &shadow);
                break;

            case command_shrink:
                fuzz_shrink(&array);
                break;

            case command_reserve:
                fuzz_reserve(&array, input);
                break;

            default:
                pottery_test_assert(false);
                break;
        }

        fuzz_check(&array, &shadow);
    }

    (void)commands;
    //printf("%i commands run\n", commands);
    //printf("destroying array with %zu elements\n", array_ufo_count(&array));
    array_ufo_destroy(&array);

    size_t i;
    for (i = 0; i < shadow.count; ++i)
        ufo_destroy(shadow.array + i);
    free(shadow.array);
}

#define TEST_POTTERY_FUZZ_TEST_NAME TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX
#include "pottery/fuzz/test_pottery_fuzz_harness.t.h"

#undef TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX
