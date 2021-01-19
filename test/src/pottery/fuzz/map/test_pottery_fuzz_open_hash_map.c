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

#include "pottery/common/test_pottery_ufo.h"

static inline size_t fnv1a(const char* p) {
    uint32_t hash = 2166136261;
    for (; *p != 0; ++p)
        hash = (hash ^ pottery_cast(uint8_t, *p)) * 16777619;
    return hash;
}

#define POTTERY_OPEN_HASH_MAP_PREFIX pottery_open_hash_map_fuzz_ufo
#define POTTERY_OPEN_HASH_MAP_VALUE_TYPE ufo_t
#define POTTERY_OPEN_HASH_MAP_KEY_TYPE const char*
#define POTTERY_OPEN_HASH_MAP_REF_KEY(ufo) ufo->string
#define POTTERY_OPEN_HASH_MAP_KEY_EQUAL(x, y) 0 == strcmp(x, y)
#define POTTERY_OPEN_HASH_MAP_KEY_HASH fnv1a
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_INIT_COPY ufo_init_copy
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_MOVE ufo_move
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_DESTROY ufo_destroy
#include "pottery/open_hash_map/pottery_open_hash_map_static.t.h"

#define TEST_POTTERY_FUZZ_MAP_UFO_PREFIX pottery_open_hash_map_fuzz_ufo
#define TEST_POTTERY_FUZZ_MAP_UFO_CAN_RESERVE 0
#define TEST_POTTERY_FUZZ_MAP_UFO_CAN_SHRINK 0
#include "pottery/fuzz/map/test_pottery_fuzz_map_ufo.t.h"
