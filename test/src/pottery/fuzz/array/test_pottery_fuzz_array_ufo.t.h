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

#ifndef TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX
#error "Misconfigured template"
#endif
#ifndef TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_AT
#define TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_AT 1
#endif
#ifndef TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_RESERVE
#define TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_RESERVE 1
#endif
#ifndef TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_SHRINK
#define TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_SHRINK 1
#endif

#ifdef TEST_POTTERY_FUZZ_MAIN
    #include <stdio.h>
#else
    #include "pottery/unit/test_pottery_framework.h"
#endif

#include "pottery/fuzz/test_fuzz_common.h"

#define FUZZ_ARRAY_UFO_NAME_MAX_LENGTH 7
#define FUZZ_ARRAY_UFO_COUNT_LIMIT 1000000

// Rename ufo_array functions to the container being tested
//!!! AUTOGENERATED:container/rename.m.h SRC:ufo_array DEST:TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from ufo_array to TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX

    // types
    #define ufo_array_t POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _t)
    #define ufo_array_value_t POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _value_t)
    #define ufo_array_ref_t POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _ref_t)
    #define ufo_array_entry_t POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _entry_t)
    #define ufo_array_context_t POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _context_t)

    // lifecycle operations
    #define ufo_array_copy POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _copy)
    #define ufo_array_destroy POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _destroy)
    #define ufo_array_init POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _init)
    #define ufo_array_init_copy POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _init_copy)
    #define ufo_array_init_steal POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _init_steal)
    #define ufo_array_move POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _move)
    #define ufo_array_steal POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _steal)
    #define ufo_array_swap POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _swap)

    // entry operations
    #define ufo_array_entry_exists POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _entry_exists)
    #define ufo_array_entry_equal POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _entry_equal)
    #define ufo_array_entry_value POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _entry_value)

    // lookup
    #define ufo_array_count POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _count)
    #define ufo_array_is_empty POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _is_empty)
    #define ufo_array_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _at)
    #define ufo_array_find POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _find)
    #define ufo_array_contains_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _contains_key)
    #define ufo_array_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _first)
    #define ufo_array_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _last)
    #define ufo_array_any POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _any)

    // array access
    #define ufo_array_select POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _select)
    #define ufo_array_index POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _index)
    #define ufo_array_shift POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _shift)
    #define ufo_array_offset POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _offset)
    #define ufo_array_begin POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _begin)
    #define ufo_array_end POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _end)
    #define ufo_array_next POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _next)
    #define ufo_array_previous POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _previous)

    // misc
    #define ufo_array_capacity POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _capacity)
    #define ufo_array_reserve POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _reserve)
    #define ufo_array_shrink POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _shrink)

    // insert
    #define ufo_array_insert POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert)
    #define ufo_array_insert_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_at)
    #define ufo_array_insert_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_first)
    #define ufo_array_insert_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_last)
    #define ufo_array_insert_before POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_before)
    #define ufo_array_insert_after POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_after)
    #define ufo_array_insert_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_bulk)
    #define ufo_array_insert_at_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_at_bulk)
    #define ufo_array_insert_first_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_first_bulk)
    #define ufo_array_insert_last_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_last_bulk)
    #define ufo_array_insert_before_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_before_bulk)
    #define ufo_array_insert_after_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _insert_after_bulk)

    // emplace
    #define ufo_array_emplace POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace)
    #define ufo_array_emplace_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_at)
    #define ufo_array_emplace_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_key)
    #define ufo_array_emplace_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_first)
    #define ufo_array_emplace_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_last)
    #define ufo_array_emplace_before POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_before)
    #define ufo_array_emplace_after POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_after)
    #define ufo_array_emplace_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_bulk)
    #define ufo_array_emplace_at_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_at_bulk)
    #define ufo_array_emplace_first_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_first_bulk)
    #define ufo_array_emplace_last_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_last_bulk)
    #define ufo_array_emplace_before_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_before_bulk)
    #define ufo_array_emplace_after_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _emplace_after_bulk)

    // construct
    #define ufo_array_construct POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _construct)
    #define ufo_array_construct_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _construct_at)
    #define ufo_array_construct_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _construct_key)
    #define ufo_array_construct_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _construct_first)
    #define ufo_array_construct_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _construct_last)
    #define ufo_array_construct_before POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _construct_before)
    #define ufo_array_construct_after POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _construct_after)

    // extract
    #define ufo_array_extract POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _extract)
    #define ufo_array_extract_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _extract_at)
    #define ufo_array_extract_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _extract_key)
    #define ufo_array_extract_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _extract_first)
    #define ufo_array_extract_first_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _extract_first_bulk)
    #define ufo_array_extract_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _extract_last)
    #define ufo_array_extract_last_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _extract_last_bulk)
    #define ufo_array_extract_and_next POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _extract_and_next)
    #define ufo_array_extract_and_previous POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _extract_and_previous)

    // remove
    #define ufo_array_remove POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove)
    #define ufo_array_remove_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_at)
    #define ufo_array_remove_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_key)
    #define ufo_array_remove_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_first)
    #define ufo_array_remove_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_last)
    #define ufo_array_remove_and_next POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_and_next)
    #define ufo_array_remove_and_previous POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_and_previous)
    #define ufo_array_remove_all POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_all)
    #define ufo_array_remove_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_bulk)
    #define ufo_array_remove_at_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_at_bulk)
    #define ufo_array_remove_first_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_first_bulk)
    #define ufo_array_remove_last_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_last_bulk)
    #define ufo_array_remove_at_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_at_bulk)
    #define ufo_array_remove_and_next_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_and_next_bulk)
    #define ufo_array_remove_and_previous_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _remove_and_previous_bulk)

    // displace
    #define ufo_array_displace POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace)
    #define ufo_array_displace_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_at)
    #define ufo_array_displace_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_key)
    #define ufo_array_displace_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_first)
    #define ufo_array_displace_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_last)
    #define ufo_array_displace_and_next POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_and_next)
    #define ufo_array_displace_and_previous POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_and_previous)
    #define ufo_array_displace_all POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_all)
    #define ufo_array_displace_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_bulk)
    #define ufo_array_displace_at_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_at_bulk)
    #define ufo_array_displace_first_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_first_bulk)
    #define ufo_array_displace_last_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_last_bulk)
    #define ufo_array_displace_and_next_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_and_next_bulk)
    #define ufo_array_displace_and_previous_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX, _displace_and_previous_bulk)
//!!! END_AUTOGENERATED

// The shadow array. The contents of the real array much match this after every
// command.
typedef struct shadow_t {
    ufo_t* array;
    size_t count;
    size_t salt;
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
static void fuzz_check(ufo_array_t* array, shadow_t* shadow) {

    // TODO enable this (should exist in unit tests even in release builds)
    //ufo_array_check(array);

    pottery_test_assert(ufo_array_count(array) == shadow->count);

    ufo_t* expected = shadow->array;
    ufo_array_entry_t entry = ufo_array_begin(array);
    for (; ufo_array_entry_exists(array, entry); entry = ufo_array_next(array, entry)) {
        ufo_t* ufo = ufo_array_entry_value(array, entry);
        pottery_test_assert(ufo_equal(ufo, expected++));
    }
}

typedef enum command_t {

    #if TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_AT
    // these are our first two commands so that we can easily generate an afl
    // test case that grows a ton and then shrinks back down to nothing
    // (see test/tools/generate-corpus.sh)
    command_emplace_at,
    command_remove_at,
    #endif

    // these are our next two commands for the same reasons as above, in case
    // at isn't supported by the container
    command_emplace_last,
    command_remove_first,

    // additional commands
    command_emplace_first,
    command_remove_last,
    #if TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_SHRINK
    command_shrink,
    #endif
    #if TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_RESERVE
    command_reserve,
    #endif

    // alloc failure
    //command_alloc_failure, // TODO alloc fail after n more allocations

    command_count,
} command_t;

#if TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_SHRINK
static void fuzz_shrink(ufo_array_t* array) {
    ufo_array_shrink(array);
}
#endif

#if TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_RESERVE
static void fuzz_reserve(ufo_array_t* array, fuzz_input_t* input) {
    // Reserve any number between 256 and (roughly) four times what the array
    // currently holds. Often we'll be reserving less (which should do
    // nothing), but usually we'll be reserving more, sometimes much more.
    size_t count = fuzz_load_u24(input) % (256 + 4 * ufo_array_count(array));
    ufo_array_reserve(array, count);
}
#endif

#if TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_AT
static void fuzz_emplace_at(ufo_array_t* array, fuzz_input_t* input, shadow_t* shadow) {
    if (shadow->count == FUZZ_ARRAY_UFO_COUNT_LIMIT)
        return;
    size_t pos = fuzz_load_u24(input) % (ufo_array_count(array) + 1);

    // create the ufo
    ufo_t ufo;
    if (!fuzz_ufo_init(&ufo, input, shadow->salt++))
        return;

    // emplace into real array
    //printf("emplacing %s at %zu of %zu\n", ufo.string, pos, ufo_array_count(array));
    ufo_array_entry_t entry;
    if (POTTERY_OK != ufo_array_emplace_at(array, pos, &entry)) {
        ufo_destroy(&ufo);
        return;
    }
    ufo_move(ufo_array_entry_value(array, entry), &ufo);

    // insert into shadow array
    ufo_move_bulk_up(shadow->array + pos + 1, shadow->array + pos, shadow->count - pos);
    ufo_init_copy(shadow->array + pos, ufo_array_entry_value(array, entry));
    ++shadow->count;
}
#endif

static void fuzz_emplace_first(ufo_array_t* array, fuzz_input_t* input, shadow_t* shadow) {
    if (shadow->count == FUZZ_ARRAY_UFO_COUNT_LIMIT)
        return;

    // create the ufo
    ufo_t ufo;
    if (!fuzz_ufo_init(&ufo, input, shadow->salt++))
        return;

    // emplace into real array
    //printf("emplacing %s as first of %zu\n", ufo.string, ufo_array_count(array));
    ufo_array_entry_t entry;
    if (POTTERY_OK != ufo_array_emplace_first(array, &entry)) {
        ufo_destroy(&ufo);
        return;
    }
    ufo_move(ufo_array_entry_value(array, entry), &ufo);

    // insert into shadow array
    ufo_move_bulk_up(shadow->array + 1, shadow->array, shadow->count);
    ufo_init_copy(shadow->array, ufo_array_entry_value(array, entry));
    ++shadow->count;
}

static void fuzz_emplace_last(ufo_array_t* array, fuzz_input_t* input, shadow_t* shadow) {
    if (shadow->count == FUZZ_ARRAY_UFO_COUNT_LIMIT)
        return;

    // create the ufo
    ufo_t ufo;
    if (!fuzz_ufo_init(&ufo, input, shadow->salt++))
        return;

    // emplace into real array
    //printf("emplacing %s as last of %zu\n", ufo.string, ufo_array_count(array));
    ufo_array_entry_t entry;
    if (POTTERY_OK != ufo_array_emplace_last(array, &entry)) {
        ufo_destroy(&ufo);
        return;
    }
    ufo_move(ufo_array_entry_value(array, entry), &ufo);

    // insert into shadow array
    ufo_init_copy(shadow->array + shadow->count, ufo_array_entry_value(array, entry));
    ++shadow->count;
}

#if TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_AT
static void fuzz_remove_at(ufo_array_t* array, fuzz_input_t* input, shadow_t* shadow) {
    if (ufo_array_is_empty(array)) {
        pottery_test_assert(shadow->count == 0);
        //printf("array is empty, not removing\n");
        return;
    }

    size_t pos = fuzz_load_u24(input) % ufo_array_count(array);

    // remove from real array
    //printf("removing at %zu of %zu\n", pos, ufo_array_count(array));
    ufo_array_remove_at(array, pos);

    // remove from shadow array
    pottery_test_assert(pos < shadow->count);
    --shadow->count;
    ufo_destroy(shadow->array + pos);
    ufo_move_bulk_down(shadow->array + pos, shadow->array + pos + 1, shadow->count - pos);
}
#endif

static void fuzz_remove_first(ufo_array_t* array, fuzz_input_t* input, shadow_t* shadow) {
    (void)input;

    if (ufo_array_is_empty(array)) {
        pottery_test_assert(shadow->count == 0);
        //printf("array is empty, not removing\n");
        return;
    }

    // remove from real array
    //printf("removing first of %zu\n", ufo_array_count(array));
    ufo_array_remove_first(array);

    // remove from shadow array
    --shadow->count;
    ufo_destroy(shadow->array);
    ufo_move_bulk_down(shadow->array, shadow->array + 1, shadow->count);
}

static void fuzz_remove_last(ufo_array_t* array, fuzz_input_t* input, shadow_t* shadow) {
    (void)input;

    if (ufo_array_is_empty(array)) {
        pottery_test_assert(shadow->count == 0);
        //printf("array is empty, not removing\n");
        return;
    }

    // remove from real array
    //printf("removing last of %zu\n", ufo_array_count(array));
    ufo_array_remove_last(array);

    // remove from shadow array
    --shadow->count;
    ufo_destroy(shadow->array + shadow->count);
}

static void fuzz(fuzz_input_t* input) {
    shadow_t shadow = {
        pottery_cast(ufo_t*, malloc(sizeof(ufo_t) * FUZZ_ARRAY_UFO_COUNT_LIMIT)),
        0,
        0,
    };
    pottery_test_assert(shadow.array != pottery_null);

    ufo_array_t array;
    ufo_array_init(&array);

    int commands = 0;
    while (input->pos != input->end) {
        ++commands;
        int command = *(input->pos++) % pottery_cast(int, command_count);
        switch (pottery_cast(command_t, command)) {

            #if TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_AT
            case command_emplace_at:
                fuzz_emplace_at(&array, input, &shadow);
                break;
            case command_remove_at:
                fuzz_remove_at(&array, input, &shadow);
                break;
            #endif

            case command_emplace_first:
                fuzz_emplace_first(&array, input, &shadow);
                break;
            case command_remove_first:
                fuzz_remove_first(&array, input, &shadow);
                break;

            case command_emplace_last:
                fuzz_emplace_last(&array, input, &shadow);
                break;
            case command_remove_last:
                fuzz_remove_last(&array, input, &shadow);
                break;

            #if TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_SHRINK
            case command_shrink:
                fuzz_shrink(&array);
                break;
            #endif

            #if TEST_POTTERY_FUZZ_ARRAY_UFO_CAN_RESERVE
            case command_reserve:
                fuzz_reserve(&array, input);
                break;
            #endif

            default:
                pottery_test_assert(false);
                break;
        }

        fuzz_check(&array, &shadow);
    }

    (void)commands;
    //printf("%i commands run\n", commands);
    //printf("destroying array with %zu elements\n", ufo_array_count(&array));
    ufo_array_destroy(&array);

    size_t i;
    for (i = 0; i < shadow.count; ++i)
        ufo_destroy(shadow.array + i);
    free(shadow.array);
}

#define TEST_POTTERY_FUZZ_TEST_NAME TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX
#include "pottery/fuzz/test_pottery_fuzz_harness.t.h"

#undef TEST_POTTERY_FUZZ_ARRAY_UFO_PREFIX
