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

#include "string_set.h"

#include <stddef.h>
#include <stdint.h>

static inline size_t fnv1a(const char* p) {
    uint32_t hash = 2166136261;
    for (; *p != 0; ++p)
        hash = (hash ^ (uint8_t)*p) * 16777619;
    return hash;
}

// We use an internal hash map that uses const char* as the key type so we can
// compare it to const string arguments.

#define POTTERY_OPEN_HASH_MAP_PREFIX string_set_map
#define POTTERY_OPEN_HASH_MAP_VALUE_TYPE char*
#define POTTERY_OPEN_HASH_MAP_KEY_TYPE const char*
#define POTTERY_OPEN_HASH_MAP_KEY_FOR_VALUE(x) *x // TODO rename to key_for_ref or ref_key or something
#define POTTERY_OPEN_HASH_MAP_KEY_HASH fnv1a
#define POTTERY_OPEN_HASH_MAP_KEY_EQUAL 0 == strcmp
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_DESTROY(x) free(*x)
// These last two aren't strictly required, but they make it more efficient
// because we can use an in-band value to mark empty buckets.
#define POTTERY_OPEN_HASH_MAP_IS_EMPTY(x) *x == NULL
#define POTTERY_OPEN_HASH_MAP_SET_EMPTY(x) *x = NULL
#include "pottery/open_hash_map/pottery_open_hash_map_static.t.h"

struct string_set_t {
    string_set_map_t map;
};

string_set_t* string_set_new(void) {
    string_set_t* set = (string_set_t*) malloc(sizeof(string_set_t));
    string_set_map_init(&set->map);
    return set;
}

void string_set_delete(string_set_t* set) {
    string_set_map_destroy(&set->map);
    free(set);
}

bool string_set_add(string_set_t* set, const char* str) {
    bool created;
    char** entry;
    if (POTTERY_OK != string_set_map_emplace(&set->map, str, &entry, &created))
        abort();
    if (created)
        *entry = strdup(str);
    return created;
}

bool string_set_query(string_set_t* set, const char* str) {
    return string_set_map_ref_exists(&set->map, string_set_map_find(&set->map, str));
}

bool string_set_remove(string_set_t* set, const char* str) {
    return string_set_map_remove_key(&set->map, str);
}
