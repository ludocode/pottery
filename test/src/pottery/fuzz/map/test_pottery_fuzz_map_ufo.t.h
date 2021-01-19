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

#ifndef TEST_POTTERY_FUZZ_MAP_UFO_PREFIX
#error "Misconfigured template"
#endif

#ifndef TEST_POTTERY_FUZZ_MAP_UFO_CAN_RESERVE
#define TEST_POTTERY_FUZZ_MAP_UFO_CAN_RESERVE 1
#endif
#ifndef TEST_POTTERY_FUZZ_MAP_UFO_CAN_SHRINK
#define TEST_POTTERY_FUZZ_MAP_UFO_CAN_SHRINK 1
#endif

#ifdef TEST_POTTERY_FUZZ_MAIN
    #include <stdio.h>
#else
    #include "pottery/unit/test_pottery_framework.h"
#endif

#include "pottery/fuzz/test_fuzz_common.h"

#define FUZZ_MAP_UFO_NAME_MAX_LENGTH 7
#define FUZZ_MAP_UFO_COUNT_LIMIT 1000000

// Rename ufo_map functions to the container being tested
//!!! AUTOGENERATED:container/rename.m.h SRC:ufo_map DEST:TEST_POTTERY_FUZZ_MAP_UFO_PREFIX
    // This block is auto-generated. Do not modify until END_AUTOGENERATED.
    // Renames array identifiers from ufo_map to TEST_POTTERY_FUZZ_MAP_UFO_PREFIX

    // types
    #define ufo_map_t POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _t)
    #define ufo_map_value_t POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _value_t)
    #define ufo_map_ref_t POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _ref_t)
    #define ufo_map_entry_t POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _entry_t)
    #define ufo_map_context_t POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _context_t)

    // lifecycle operations
    #define ufo_map_copy POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _copy)
    #define ufo_map_destroy POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _destroy)
    #define ufo_map_init POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _init)
    #define ufo_map_init_copy POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _init_copy)
    #define ufo_map_init_steal POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _init_steal)
    #define ufo_map_move POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _move)
    #define ufo_map_steal POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _steal)
    #define ufo_map_swap POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _swap)

    // entry operations
    #define ufo_map_entry_exists POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _entry_exists)
    #define ufo_map_entry_equal POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _entry_equal)
    #define ufo_map_entry_value POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _entry_value)

    // lookup
    #define ufo_map_count POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _count)
    #define ufo_map_is_empty POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _is_empty)
    #define ufo_map_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _at)
    #define ufo_map_find POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _find)
    #define ufo_map_contains_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _contains_key)
    #define ufo_map_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _first)
    #define ufo_map_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _last)
    #define ufo_map_any POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _any)

    // array access
    #define ufo_map_select POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _select)
    #define ufo_map_index POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _index)
    #define ufo_map_shift POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _shift)
    #define ufo_map_offset POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _offset)
    #define ufo_map_begin POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _begin)
    #define ufo_map_end POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _end)
    #define ufo_map_next POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _next)
    #define ufo_map_previous POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _previous)

    // misc
    #define ufo_map_capacity POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _capacity)
    #define ufo_map_reserve POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _reserve)
    #define ufo_map_shrink POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _shrink)

    // insert
    #define ufo_map_insert POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert)
    #define ufo_map_insert_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_at)
    #define ufo_map_insert_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_first)
    #define ufo_map_insert_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_last)
    #define ufo_map_insert_before POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_before)
    #define ufo_map_insert_after POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_after)
    #define ufo_map_insert_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_bulk)
    #define ufo_map_insert_at_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_at_bulk)
    #define ufo_map_insert_first_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_first_bulk)
    #define ufo_map_insert_last_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_last_bulk)
    #define ufo_map_insert_before_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_before_bulk)
    #define ufo_map_insert_after_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _insert_after_bulk)

    // emplace
    #define ufo_map_emplace POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace)
    #define ufo_map_emplace_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_at)
    #define ufo_map_emplace_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_key)
    #define ufo_map_emplace_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_first)
    #define ufo_map_emplace_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_last)
    #define ufo_map_emplace_before POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_before)
    #define ufo_map_emplace_after POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_after)
    #define ufo_map_emplace_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_bulk)
    #define ufo_map_emplace_at_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_at_bulk)
    #define ufo_map_emplace_first_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_first_bulk)
    #define ufo_map_emplace_last_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_last_bulk)
    #define ufo_map_emplace_before_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_before_bulk)
    #define ufo_map_emplace_after_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _emplace_after_bulk)

    // construct
    #define ufo_map_construct POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _construct)
    #define ufo_map_construct_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _construct_at)
    #define ufo_map_construct_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _construct_key)
    #define ufo_map_construct_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _construct_first)
    #define ufo_map_construct_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _construct_last)
    #define ufo_map_construct_before POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _construct_before)
    #define ufo_map_construct_after POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _construct_after)

    // extract
    #define ufo_map_extract POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _extract)
    #define ufo_map_extract_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _extract_at)
    #define ufo_map_extract_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _extract_key)
    #define ufo_map_extract_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _extract_first)
    #define ufo_map_extract_first_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _extract_first_bulk)
    #define ufo_map_extract_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _extract_last)
    #define ufo_map_extract_last_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _extract_last_bulk)
    #define ufo_map_extract_and_next POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _extract_and_next)
    #define ufo_map_extract_and_previous POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _extract_and_previous)

    // remove
    #define ufo_map_remove POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove)
    #define ufo_map_remove_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_at)
    #define ufo_map_remove_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_key)
    #define ufo_map_remove_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_first)
    #define ufo_map_remove_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_last)
    #define ufo_map_remove_and_next POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_and_next)
    #define ufo_map_remove_and_previous POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_and_previous)
    #define ufo_map_remove_all POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_all)
    #define ufo_map_remove_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_bulk)
    #define ufo_map_remove_at_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_at_bulk)
    #define ufo_map_remove_first_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_first_bulk)
    #define ufo_map_remove_last_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_last_bulk)
    #define ufo_map_remove_at_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_at_bulk)
    #define ufo_map_remove_and_next_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_and_next_bulk)
    #define ufo_map_remove_and_previous_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _remove_and_previous_bulk)

    // displace
    #define ufo_map_displace POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace)
    #define ufo_map_displace_at POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_at)
    #define ufo_map_displace_key POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_key)
    #define ufo_map_displace_first POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_first)
    #define ufo_map_displace_last POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_last)
    #define ufo_map_displace_and_next POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_and_next)
    #define ufo_map_displace_and_previous POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_and_previous)
    #define ufo_map_displace_all POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_all)
    #define ufo_map_displace_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_bulk)
    #define ufo_map_displace_at_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_at_bulk)
    #define ufo_map_displace_first_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_first_bulk)
    #define ufo_map_displace_last_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_last_bulk)
    #define ufo_map_displace_and_next_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_and_next_bulk)
    #define ufo_map_displace_and_previous_bulk POTTERY_CONCAT(TEST_POTTERY_FUZZ_MAP_UFO_PREFIX, _displace_and_previous_bulk)
//!!! END_AUTOGENERATED

// The shadow map. The contents of the real map much match this after every
// command. The elements in this map are unordered (i.e. in arbitrary order);
// we do a linear search to find elements, we add by append, and we remove by
// swapping the last element in place and truncating (similar to array_map in
// unordered mode.)
typedef struct shadow_t {
    ufo_t* map;
    size_t count;
    size_t salt;
} shadow_t;

// Performs sanity checks after every command and ensures that the real map
// matches the shadow map.
static void fuzz_check(ufo_map_t* map, shadow_t* shadow) {

    // TODO enable this (should exist in unit tests even in release builds)
    //ufo_map_check(map);

    // make sure the map and shadow have the same number of elements
    //printf("comparing sizes actual %zi shadow %zi\n", ufo_map_count(map) , shadow->count);
    pottery_test_assert(ufo_map_count(map) == shadow->count);

    // make sure every element in the shadow can be found in the map
    size_t i;
    for (i = 0; i < shadow->count; ++i) {
        ufo_t* ufo = shadow->map + i;
        ufo_map_entry_t entry = ufo_map_find(map, ufo->string);
        pottery_test_assert(ufo_map_entry_exists(map, entry));
        pottery_test_assert(ufo_equal(ufo, ufo_map_entry_value(map, entry)));
    }
}

typedef enum command_t {

    // these are our first two commands so that we can easily generate an afl
    // test case that grows a ton and then shrinks back down to nothing
    // (see test/tools/generate-corpus.sh)
    command_emplace_key,
    command_remove_key,

    #if TEST_POTTERY_FUZZ_MAP_UFO_CAN_SHRINK
    command_shrink,
    #endif
    #if TEST_POTTERY_FUZZ_MAP_UFO_CAN_RESERVE
    command_reserve,
    #endif

    // alloc failure
    //command_alloc_failure, // TODO alloc fail after n more allocations

    command_count,
} command_t;

#if TEST_POTTERY_FUZZ_MAP_UFO_CAN_SHRINK
static void fuzz_shrink(ufo_map_t* map) {
    ufo_map_shrink(map);
}
#endif

#if TEST_POTTERY_FUZZ_MAP_UFO_CAN_RESERVE
static void fuzz_reserve(ufo_map_t* map, fuzz_input_t* input) {
    // Reserve any number between 256 and (roughly) four times what the map
    // currently holds. Often we'll be reserving less (which should do
    // nothing), but usually we'll be reserving more, sometimes much more.
    size_t count = fuzz_load_u24(input) % (256 + 4 * ufo_map_count(map));
    ufo_map_reserve(map, count);
}
#endif

static void fuzz_emplace_key(ufo_map_t* map, fuzz_input_t* input, shadow_t* shadow) {
    if (shadow->count == FUZZ_MAP_UFO_COUNT_LIMIT)
        return;

    // create the ufo
    ufo_t ufo;
    if (!fuzz_ufo_init(&ufo, input, shadow->salt++))
        return;

    // emplace into real map
    //printf("emplacing %s in map of size %zu\n", ufo.string, ufo_map_count(map));
    bool created;
    ufo_map_entry_t entry;
    if (POTTERY_OK != ufo_map_emplace(map, ufo.string, &entry, &created)) {
        ufo_destroy(&ufo);
        return;
    }

    if (created) {
        //printf("created\n");
        // the ufo's key doesn't exist in the map. move it in
        ufo_move(ufo_map_entry_value(map, entry), &ufo);
        // add to shadow map
        ufo_init_copy(shadow->map + shadow->count++, ufo_map_entry_value(map, entry));

    } else {
        //printf("not created\n");

        // the ufo's key exists already in the map. find it in the shadow
        ufo_t* shadow_ufo = pottery_null;
        size_t i;
        for (i = 0; i < shadow->count; ++i) {
            if (0 == ufo_compare_by_string(shadow->map + i, &ufo)) {
                shadow_ufo = shadow->map + i;
                break;
            }
        }
        pottery_test_assert(shadow_ufo != pottery_null);

        // replace the ufo in the map and the ufo in the shadow
        ufo_steal(ufo_map_entry_value(map, entry), &ufo);
        ufo_steal(shadow_ufo, &ufo);
        ufo_destroy(&ufo);
    }
}

static void fuzz_remove_key(ufo_map_t* map, fuzz_input_t* input, shadow_t* shadow) {
    if (ufo_map_is_empty(map)) {
        pottery_test_assert(shadow->count == 0);
        //printf("map is empty, not removing\n");
        return;
    }
    pottery_test_assert(shadow->count > 0);

    size_t shadow_pos = fuzz_load_u24(input) % ufo_map_count(map);
    const char* key = shadow->map[shadow_pos].string;
    //printf("removing %s of %zi\n", key, ufo_map_count(map));

    // remove from real map
    ufo_map_remove_key(map, key);

    // remove from shadow map, moving the last element into its place and truncating
    pottery_test_assert(shadow_pos < shadow->count);
    ufo_destroy(shadow->map + shadow_pos);
    if (shadow_pos != shadow->count - 1)
        ufo_move(shadow->map + shadow_pos, shadow->map + shadow->count - 1);
    --shadow->count;
}

static void fuzz(fuzz_input_t* input) {
    shadow_t shadow = {
        pottery_cast(ufo_t*, malloc(sizeof(ufo_t) * FUZZ_MAP_UFO_COUNT_LIMIT)),
        0,
        0,
    };
    pottery_test_assert(shadow.map != pottery_null);

    ufo_map_t map;
    ufo_map_init(&map);

    //printf("=============================\n");
    //printf("starting to run commands\n");

    int commands = 0;
    while (input->pos != input->end) {
        ++commands;
        int command = *(input->pos++) % pottery_cast(int, command_count);
        switch (pottery_cast(command_t, command)) {

            case command_emplace_key:
                fuzz_emplace_key(&map, input, &shadow);
                break;
            case command_remove_key:
                fuzz_remove_key(&map, input, &shadow);
                break;

            #if TEST_POTTERY_FUZZ_MAP_UFO_CAN_SHRINK
            case command_shrink:
                fuzz_shrink(&map);
                break;
            #endif

            #if TEST_POTTERY_FUZZ_MAP_UFO_CAN_RESERVE
            case command_reserve:
                fuzz_reserve(&map, input);
                break;
            #endif

            default:
                pottery_test_assert(false);
                break;
        }

        fuzz_check(&map, &shadow);
    }

    (void)commands;
    //printf("%i commands run\n", commands);
    //printf("destroying map with %zu elements\n", ufo_map_count(&map));
    ufo_map_destroy(&map);

    size_t i;
    for (i = 0; i < shadow.count; ++i)
        ufo_destroy(shadow.map + i);
    free(shadow.map);
}

#define TEST_POTTERY_FUZZ_TEST_NAME TEST_POTTERY_FUZZ_MAP_UFO_PREFIX
#include "pottery/fuzz/test_pottery_fuzz_harness.t.h"

#undef TEST_POTTERY_FUZZ_MAP_UFO_PREFIX
