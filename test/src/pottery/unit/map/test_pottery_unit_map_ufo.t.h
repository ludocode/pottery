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

/*
 * Tests a map of const char* to ufo_t called ufo_map.
 *
 * This is used to test various map implementations.
 */

#ifndef POTTERY_TEST_MAP_UFO_PREFIX
#error "Misconfigured template"
#endif

#ifndef POTTERY_TEST_MAP_UFO_HAS_CAPACITY
    #define POTTERY_TEST_MAP_UFO_HAS_CAPACITY 0
#endif

#ifndef POTTERY_TEST_MAP_UFO_NO_DESTROY
    #define POTTERY_TEST_MAP_UFO_NO_DESTROY 0
#endif

#define POTTERY_TEST_MAP_UFO(x) \
    POTTERY_TEST(POTTERY_CONCAT(POTTERY_CONCAT(POTTERY_TEST_MAP_UFO_PREFIX, _), x))

POTTERY_TEST_MAP_UFO(init_destroy) {
    ufo_map_t map;
    pottery_test_assert(POTTERY_OK == ufo_map_init(&map));
    pottery_test_assert(ufo_map_count(&map) == 0);
    ufo_map_destroy(&map);
}

static inline void check_ufo_map(ufo_map_t* map) {
    size_t count = 0;
    ufo_map_ref_t ref = ufo_map_begin(map);
    for (; ufo_map_ref_exists(map, ref); ufo_map_next(map, &ref)) {
        ufo_check(ref);
        ++count;
    }
    pottery_test_assert(count == ufo_map_count(map));
}

POTTERY_TEST_MAP_UFO(remove) {
    ufo_map_t map;
    pottery_test_assert(POTTERY_OK == ufo_map_init(&map));

    ufo_map_ref_t ref;
    bool created;

    created = false;
    pottery_test_assert(ufo_map_emplace(&map, "hello", &ref, &created) == POTTERY_OK);
    pottery_test_assert(created);
    ufo_init(ref, "hello", 1);
    check_ufo_map(&map);

    created = false;
    pottery_test_assert(ufo_map_emplace(&map, "world", &ref, &created) == POTTERY_OK);
    pottery_test_assert(created);
    ufo_init(ref, "world", 2);
    check_ufo_map(&map);

    pottery_test_assert(0 == strcmp(ufo_map_find(&map, "hello")->string, "hello"));
    pottery_test_assert(0 == strcmp(ufo_map_find(&map, "world")->string, "world"));

    #if POTTERY_TEST_MAP_UFO_NO_DESTROY
    // Destroy and displace
    ref = ufo_map_find(&map, "hello");
    ufo_destroy(ref);
    ufo_map_displace(&map, ref);
    #else
    // Removing an element should call its destroy function
    ufo_map_remove_key(&map, "hello");
    #endif

    check_ufo_map(&map);

    // There's only one ufo left in the map, so first() should return it
    pottery_test_assert(0 == strcmp(ufo_map_first(&map)->string, "world"));

    // If we have a destroy expression, destroying with contained elements
    // is allowed because the map can destroy its contents. This cleans up the
    // remaining ufo. If we don't have a destroy expression, we have to clear
    // it manually (the map verifies that it's empty when destroyed to ensure
    // that there are no leaks.)

    #if POTTERY_TEST_MAP_UFO_NO_DESTROY
    ufo_destroy(ufo_map_find(&map, "world"));
    ufo_map_displace_all(&map);
    #endif

    ufo_map_destroy(&map);
}

POTTERY_TEST_MAP_UFO(remove_all) {
    ufo_map_t map;
    pottery_test_assert(POTTERY_OK == ufo_map_init(&map));

    ufo_map_ref_t ref;

    pottery_test_assert(ufo_map_emplace(&map, "apple", &ref, pottery_null) == POTTERY_OK);
    pottery_test_assert(ufo_init(ref, "apple", 1) == POTTERY_OK);

    pottery_test_assert(ufo_map_emplace(&map, "banana", &ref, pottery_null) == POTTERY_OK);
    pottery_test_assert(ufo_init(ref, "banana", 1) == POTTERY_OK);

    pottery_test_assert(ufo_map_emplace(&map, "cherry", &ref, pottery_null) == POTTERY_OK);
    pottery_test_assert(ufo_init(ref, "cherry", 1) == POTTERY_OK);

    check_ufo_map(&map);

    #if POTTERY_TEST_MAP_UFO_NO_DESTROY
    // Destroy and displace all
    for (ref = ufo_map_begin(&map); ufo_map_ref_exists(&map, ref); ufo_map_next(&map, &ref))
        ufo_destroy(ref);
    ufo_map_displace_all(&map);
    #else
    // Remove all destroys all elements.
    ufo_map_remove_all(&map);
    #endif

    check_ufo_map(&map);

    ufo_map_destroy(&map);
}

POTTERY_TEST_MAP_UFO(grow_and_shrink) {
    ufo_map_t map;
    pottery_test_assert(POTTERY_OK == ufo_map_init(&map));

    #if POTTERY_TEST_MAP_UFO_HAS_CAPACITY
    size_t capacity = ufo_map_capacity(&map);
    //printf("initial capacity %zi\n", capacity);
    int capacity_changes = 0;
    #endif

    // Add 1000 elements
    int16_t i;
    for (i = 0; i < 1000; ++i) {
        char key[16];
        snprintf(key, sizeof(key), "%i", i);

        bool created = false;
        ufo_map_ref_t ref;
        //printf("inserting %i\n", i);
        pottery_test_assert(ufo_map_emplace(&map, key, &ref, &created) == POTTERY_OK);
        pottery_test_assert(created);
        ufo_init(ref, key, i);
        check_ufo_map(&map);

        // Track capacity changes
        #if POTTERY_TEST_MAP_UFO_HAS_CAPACITY
        if (capacity != ufo_map_capacity(&map)) {
            capacity = ufo_map_capacity(&map);
            ++capacity_changes;
        }
        #endif
    }

    #if POTTERY_TEST_MAP_UFO_HAS_CAPACITY
    // Make sure we grew a reasonable number of times
    pottery_test_assert(capacity_changes >= 3);
    pottery_test_assert(capacity_changes <= 20);
    capacity_changes = 0;
    #endif

    // Remove elements until the map is empty.
    // (This scans from the front of the map forward every iteration. This is
    // not the ideal way to empty a hash table.)
    while (!ufo_map_is_empty(&map)) {

        //printf("removing %i\n", ufo_map_first(&map)->integer);
        #if POTTERY_TEST_MAP_UFO_NO_DESTROY
        ufo_map_ref_t ref = ufo_map_first(&map);
        ufo_destroy(ref);
        ufo_map_displace(&map, ref);
        #else
        ufo_map_remove(&map, ufo_map_first(&map));
        #endif

        check_ufo_map(&map);

        // Track capacity changes
        #if POTTERY_TEST_MAP_UFO_HAS_CAPACITY
        if (capacity != ufo_map_capacity(&map)) {
            capacity = ufo_map_capacity(&map);
            ++capacity_changes;
        }
        #endif
    }

    #if POTTERY_TEST_MAP_UFO_HAS_CAPACITY
    // Make sure we shrunk a reasonable number of times
    pottery_test_assert(capacity_changes >= 3);
    pottery_test_assert(capacity_changes <= 20);
    #endif

    ufo_map_destroy(&map);
}

#undef POTTERY_TEST_MAP_UFO

#undef POTTERY_TEST_MAP_UFO_HAS_CAPACITY
#undef POTTERY_TEST_MAP_UFO_PREFIX
