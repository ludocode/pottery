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

// workarounds for MSVC C++/CLR not allowing different structs in different
// translation units with the same name
#ifdef __CLR_VER
#define ufo_map_t pottery_tree_map_ufo_t
#endif

// Instantiate a map of const char* to ufo_t with NULL as empty
#define POTTERY_TREE_MAP_PREFIX ufo_map
#define POTTERY_TREE_MAP_VALUE_TYPE ufo_t
#define POTTERY_TREE_MAP_KEY_TYPE const char*
#define POTTERY_TREE_MAP_REF_KEY(x) x->string
#define POTTERY_TREE_MAP_COMPARE_THREE_WAY strcmp
#define POTTERY_TREE_MAP_LIFECYCLE_MOVE ufo_move
#define POTTERY_TREE_MAP_LIFECYCLE_DESTROY ufo_destroy
#include "pottery/tree_map/pottery_tree_map_static.t.h"

#include "pottery/unit/test_pottery_framework.h"

// Instantiate ufo_map tests on our ufo map
#define POTTERY_TEST_MAP_UFO_PREFIX pottery_tree_map_ufo
#define POTTERY_TEST_MAP_UFO_INIT_CAN_FAIL 0
#include "pottery/unit/map/test_pottery_unit_map_ufo.t.h"
