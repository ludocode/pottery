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

#ifndef POTTERY_TREE_MAP_IMPL
#error "This is header internal to Pottery. Do not include it."
#endif

#define POTTERY_RED_BLACK_TREE_PREFIX POTTERY_CONCAT(POTTERY_TREE_MAP_PREFIX, _tree)

// The tree will define its own container_types because we want our map as its
// context.
#define POTTERY_RED_BLACK_TREE_CONTEXT_TYPE pottery_tree_map_t*
#define POTTERY_RED_BLACK_TREE_VALUE_TYPE pottery_tree_map_node_t
#define POTTERY_RED_BLACK_TREE_KEY_TYPE pottery_tree_map_key_t
#define POTTERY_RED_BLACK_TREE_REF_KEY pottery_tree_map_node_key

// right_child and is_red are collapsed together; is_red is the low bit
#define POTTERY_RED_BLACK_TREE_RIGHT_CHILD(map, foo) \
        pottery_reinterpret_cast(pottery_tree_map_node_t*, foo->right_child_and_is_red & ~pottery_cast(uintptr_t, 1))
#define POTTERY_RED_BLACK_TREE_SET_RIGHT_CHILD(map, foo, v) \
        foo->right_child_and_is_red = pottery_reinterpret_cast(uintptr_t, v) | (foo->right_child_and_is_red & pottery_cast(uintptr_t, 1))
#define POTTERY_RED_BLACK_TREE_IS_RED(map, foo) \
        pottery_cast(bool, foo->right_child_and_is_red & pottery_cast(uintptr_t, 1))
#define POTTERY_RED_BLACK_TREE_SET_IS_RED(map, foo, r) \
        foo->right_child_and_is_red = pottery_cast(uintptr_t, !!r) | (foo->right_child_and_is_red & ~pottery_cast(uintptr_t, 1))


// Compare forwarding
// Trying a new cleaner way to remap context here. Might pull this out into a
// meta-template if it works out and use it instead of some of the other
// context remapping we're doing by loads of inline functions
#ifdef POTTERY_TREE_MAP_COMPARE_BY_VALUE
    #define POTTERY_RED_BLACK_TREE_COMPARE_BY_VALUE POTTERY_TREE_MAP_COMPARE_BY_VALUE
#endif
#ifdef POTTERY_TREE_MAP_COMPARE_EQUAL
    #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
        #define POTTERY_RED_BLACK_TREE_COMPARE_EQUAL(map, left, right) POTTERY_TREE_MAP_COMPARE_EQUAL(pottery_tree_map_context(map), left, right)
    #else
        #define POTTERY_RED_BLACK_TREE_COMPARE_EQUAL(map, left, right) POTTERY_TREE_MAP_COMPARE_EQUAL(left, right)
    #endif
#endif
#ifdef POTTERY_TREE_MAP_COMPARE_NOT_EQUAL
    #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
        #define POTTERY_RED_BLACK_TREE_COMPARE_NOT_EQUAL(map, left, right) POTTERY_TREE_MAP_COMPARE_NOT_EQUAL(pottery_tree_map_context(map), left, right)
    #else
        #define POTTERY_RED_BLACK_TREE_COMPARE_NOT_EQUAL(map, left, right) POTTERY_TREE_MAP_COMPARE_NOT_EQUAL(left, right)
    #endif
#endif
#ifdef POTTERY_TREE_MAP_COMPARE_LESS
    #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
        #define POTTERY_RED_BLACK_TREE_COMPARE_LESS(map, left, right) POTTERY_TREE_MAP_COMPARE_LESS(pottery_tree_map_context(map), left, right)
    #else
        #define POTTERY_RED_BLACK_TREE_COMPARE_LESS(map, left, right) POTTERY_TREE_MAP_COMPARE_LESS(left, right)
    #endif
#endif
#ifdef POTTERY_TREE_MAP_COMPARE_LESS_OR_EQUAL
    #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
        #define POTTERY_RED_BLACK_TREE_COMPARE_LESS_OR_EQUAL(map, left, right) POTTERY_TREE_MAP_COMPARE_LESS_OR_EQUAL(pottery_tree_map_context(map), left, right)
    #else
        #define POTTERY_RED_BLACK_TREE_COMPARE_LESS_OR_EQUAL(map, left, right) POTTERY_TREE_MAP_COMPARE_LESS_OR_EQUAL(left, right)
    #endif
#endif
#ifdef POTTERY_TREE_MAP_COMPARE_GREATER
    #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
        #define POTTERY_RED_BLACK_TREE_COMPARE_GREATER(map, left, right) POTTERY_TREE_MAP_COMPARE_GREATER(pottery_tree_map_context(map), left, right)
    #else
        #define POTTERY_RED_BLACK_TREE_COMPARE_GREATER(map, left, right) POTTERY_TREE_MAP_COMPARE_GREATER(left, right)
    #endif
#endif
#ifdef POTTERY_TREE_MAP_COMPARE_GREATER_OR_EQUAL
    #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
        #define POTTERY_RED_BLACK_TREE_COMPARE_GREATER_OR_EQUAL(map, left, right) POTTERY_TREE_MAP_COMPARE_GREATER_OR_EQUAL(pottery_tree_map_context(map), left, right)
    #else
        #define POTTERY_RED_BLACK_TREE_COMPARE_GREATER_OR_EQUAL(map, left, right) POTTERY_TREE_MAP_COMPARE_GREATER_OR_EQUAL(left, right)
    #endif
#endif
#ifdef POTTERY_TREE_MAP_COMPARE_THREE_WAY
    #ifdef POTTERY_TREE_MAP_CONTEXT_TYPE
        #define POTTERY_RED_BLACK_TREE_COMPARE_THREE_WAY(map, left, right) POTTERY_TREE_MAP_COMPARE_THREE_WAY(pottery_tree_map_context(map), left, right)
    #else
        #define POTTERY_RED_BLACK_TREE_COMPARE_THREE_WAY(map, left, right) POTTERY_TREE_MAP_COMPARE_THREE_WAY(left, right)
    #endif
#endif

// TODO make right_child and color shared
