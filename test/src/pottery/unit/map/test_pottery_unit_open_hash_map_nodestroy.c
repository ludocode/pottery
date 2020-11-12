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

static inline size_t fnv1a(const char* p) {
    uint32_t hash = 2166136261;
    for (; *p != 0; ++p)
        hash = (hash ^ pottery_cast(uint8_t, *p)) * 16777619;
    return hash;
}

// workarounds for MSVC C++/CLR not allowing different structs in different
// translation units with the same name
#ifdef __CLR_VER
#define ufo_map_t pottery_open_hash_map_nodestroy_ufo_t
#endif

// Instantiate a map of const char* to ufo_t with no destroy expression
#define POTTERY_OPEN_HASH_MAP_PREFIX ufo_map
#define POTTERY_OPEN_HASH_MAP_VALUE_TYPE ufo_t
#define POTTERY_OPEN_HASH_MAP_KEY_TYPE const char*
#define POTTERY_OPEN_HASH_MAP_VALUE_KEY(x) x->string
#define POTTERY_OPEN_HASH_MAP_KEY_HASH fnv1a
#define POTTERY_OPEN_HASH_MAP_KEY_EQUAL 0 == strcmp
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_MOVE ufo_move
#include "pottery/open_hash_map/pottery_open_hash_map_static.t.h"

// Instantiate ufo_map tests on our ufo map
// (NO_DESTROY will make the tests manually destroy and displace instead of remove)
#define POTTERY_TEST_MAP_UFO_PREFIX pottery_open_hash_map_ufo_nodestroy
#define POTTERY_TEST_MAP_UFO_HAS_CAPACITY 1
#define POTTERY_TEST_MAP_UFO_NO_DESTROY 1
#include "pottery/unit/map/test_pottery_unit_map_ufo.t.h"
