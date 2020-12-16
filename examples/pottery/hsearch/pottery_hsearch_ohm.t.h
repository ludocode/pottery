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

#define POTTERY_OPEN_HASH_MAP_PREFIX pottery_hsearch_impl
#define POTTERY_OPEN_HASH_MAP_VALUE_TYPE POTTERY_ENTRY
#define POTTERY_OPEN_HASH_MAP_KEY_TYPE const char*
#define POTTERY_OPEN_HASH_MAP_REF_KEY(x) x->key
#define POTTERY_OPEN_HASH_MAP_KEY_HASH pottery_hsearch_fnv1a
#define POTTERY_OPEN_HASH_MAP_KEY_EQUAL 0 == strcmp
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_MOVE_BY_VALUE 1

// We implement DESTROY_BY_VALUE since we don't own the values. This makes
// destroy the same as displace, but it allows us to destroy a non-empty map.
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_DESTROY_BY_VALUE 1

// NULL key represents an empty bucket.
#define POTTERY_OPEN_HASH_MAP_EMPTY_IS_ZERO 1
#define POTTERY_OPEN_HASH_MAP_SET_EMPTY(x) x->key = NULL
#define POTTERY_OPEN_HASH_MAP_IS_EMPTY(x) x->key == NULL
