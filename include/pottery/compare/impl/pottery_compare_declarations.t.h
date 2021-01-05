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

#ifndef POTTERY_COMPARE_INTERNAL
#error "This is an internal header. Do not include it."
#endif

#if POTTERY_COMPARE_CAN_EQUAL
static inline
bool pottery_compare_equal(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t left, pottery_compare_ref_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *left == *right;

    #elif defined(POTTERY_COMPARE_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_EQUAL((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_NOT_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_NOT_EQUAL((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_NOT_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return 0 == (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 == (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_LESS((context), (left), (right))) &&
                    !(POTTERY_COMPARE_LESS((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_LESS((left), (right))) &&
                    !(POTTERY_COMPARE_LESS((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_GREATER((context), (left), (right))) &&
                    !(POTTERY_COMPARE_GREATER((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_GREATER((left), (right))) &&
                    !(POTTERY_COMPARE_GREATER((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_LESS_OR_EQUAL((context), (left), (right))) &&
                    (POTTERY_COMPARE_LESS_OR_EQUAL((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_LESS_OR_EQUAL((left), (right))) &&
                    (POTTERY_COMPARE_LESS_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_GREATER_OR_EQUAL((context), (left), (right))) &&
                    (POTTERY_COMPARE_GREATER_OR_EQUAL((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_GREATER_OR_EQUAL((left), (right))) &&
                    (POTTERY_COMPARE_GREATER_OR_EQUAL((right), (left)));
        #endif

    #else
        #error "Pottery template error!"

    #endif
}

static inline
bool pottery_compare_not_equal(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t left, pottery_compare_ref_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *left != *right;

    #elif defined(POTTERY_COMPARE_NOT_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_NOT_EQUAL((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_NOT_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_EQUAL((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return 0 != (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 != (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_LESS((context), (left), (right))) ||
                    (POTTERY_COMPARE_LESS((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_LESS((left), (right))) ||
                    (POTTERY_COMPARE_LESS((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_GREATER((context), (left), (right))) ||
                    (POTTERY_COMPARE_GREATER((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_GREATER((left), (right))) ||
                    (POTTERY_COMPARE_GREATER((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((context), (left), (right))) ||
                    !(POTTERY_COMPARE_LESS_OR_EQUAL((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((left), (right))) ||
                    !(POTTERY_COMPARE_LESS_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_GREATER_OR_EQUAL((context), (left), (right))) ||
                    !(POTTERY_COMPARE_GREATER_OR_EQUAL((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_GREATER_OR_EQUAL((left), (right))) ||
                    !(POTTERY_COMPARE_GREATER_OR_EQUAL((right), (left)));
        #endif

    #else
        #error "Pottery template error!"

    #endif
}
#endif

#if POTTERY_COMPARE_CAN_ORDER
static inline
bool pottery_compare_less(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t left, pottery_compare_ref_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *left < *right;

    #elif defined(POTTERY_COMPARE_LESS)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_LESS((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_LESS((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_GREATER((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_GREATER((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_GREATER_OR_EQUAL((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_GREATER_OR_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return 0 > (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 > (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #endif
}

static inline
bool pottery_compare_greater(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t left, pottery_compare_ref_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *left > *right;

    #elif defined(POTTERY_COMPARE_GREATER)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_GREATER((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_GREATER((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_LESS((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_LESS((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_GREATER_OR_EQUAL((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_GREATER_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return 0 < (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 < (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #endif
}

static inline
bool pottery_compare_less_or_equal(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t left, pottery_compare_ref_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *left <= *right;

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_LESS_OR_EQUAL((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_LESS_OR_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_GREATER_OR_EQUAL((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_GREATER_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_GREATER((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_GREATER((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_LESS((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_LESS((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return 0 >= (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 >= (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #endif
}

static inline
bool pottery_compare_greater_or_equal(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t left, pottery_compare_ref_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *left >= *right;

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_GREATER_OR_EQUAL((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_GREATER_OR_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_LESS_OR_EQUAL((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_LESS_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_LESS((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_LESS((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return !(POTTERY_COMPARE_GREATER((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_GREATER((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return 0 <= (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 <= (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #endif
}

static inline
int pottery_compare_three_way(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t left, pottery_compare_ref_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        #ifdef __cplusplus
            #if __cplusplus > 202002L
                // This is not yet implemented. We should probably only use
                // this if it returns std::strong_ordering and we need to
                // figure out how to efficiently convert it to int.
                //if (pottery::has_spaceship_operator<pottery_compare_value_t>::value) {
                //    return *left <=> *right;
                //}
            #endif
        #endif

        return (*left < *right) ? -1 : ((*left > *right) ? 1 : 0);

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_LESS((context), (left), (right))) ? -1 :
                    ((POTTERY_COMPARE_LESS((context), (right), (left))) ? 1 : 0);
        #else
            return (POTTERY_COMPARE_LESS((left), (right))) ? -1 :
                    ((POTTERY_COMPARE_LESS((right), (left))) ? 1 : 0);
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (POTTERY_COMPARE_GREATER((context), (left), (right))) ? 1 :
                    ((POTTERY_COMPARE_GREATER((context), (right), (left))) ? -1 : 0);
        #else
            return (POTTERY_COMPARE_GREATER((left), (right))) ? 1 :
                    ((POTTERY_COMPARE_GREATER((right), (left))) ? -1 : 0);
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (!POTTERY_COMPARE_LESS_OR_EQUAL((context), (left), (right))) ? 1 :
                    (!(POTTERY_COMPARE_LESS_OR_EQUAL((context), (right), (left))) ? -1 : 0);
        #else
            return (!POTTERY_COMPARE_LESS_OR_EQUAL((left), (right))) ? 1 :
                    (!(POTTERY_COMPARE_LESS_OR_EQUAL((right), (left))) ? -1 : 0);
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return (!POTTERY_COMPARE_GREATER_OR_EQUAL((context), (left), (right))) ? -1 :
                    (!(POTTERY_COMPARE_GREATER_OR_EQUAL((context), (right), (left))) ? 1 : 0);
        #else
            return (!POTTERY_COMPARE_GREATER_OR_EQUAL((left), (right))) ? -1 :
                    (!(POTTERY_COMPARE_GREATER_OR_EQUAL((right), (left))) ? 1 : 0);
        #endif

    #else
        #error "Pottery template error!"

    #endif
}

static inline
pottery_compare_ref_t pottery_compare_min(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t left, pottery_compare_ref_t right)
{
    return pottery_compare_less(POTTERY_COMPARE_CONTEXT_VAL
            left, right) ? left : right;
}

static inline
pottery_compare_ref_t pottery_compare_max(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t left, pottery_compare_ref_t right)
{
    return pottery_compare_greater(POTTERY_COMPARE_CONTEXT_VAL
            left, right) ? left : right;
}

static inline
pottery_compare_ref_t pottery_compare_clamp(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t value, pottery_compare_ref_t min, pottery_compare_ref_t max)
{
    return pottery_compare_less(POTTERY_COMPARE_CONTEXT_VAL
                value, min) ? min :
            pottery_compare_greater(POTTERY_COMPARE_CONTEXT_VAL
                value, max) ? max : value;
}

static inline
pottery_compare_ref_t pottery_compare_median(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_ref_t a, pottery_compare_ref_t b, pottery_compare_ref_t c)
{
    if (pottery_compare_less(POTTERY_COMPARE_CONTEXT_VAL a, b)) {
        if (pottery_compare_less(POTTERY_COMPARE_CONTEXT_VAL a, c))
            return pottery_compare_min(POTTERY_COMPARE_CONTEXT_VAL b, c);
        return a;
    }
    if (pottery_compare_less(POTTERY_COMPARE_CONTEXT_VAL a, c))
        return a;
    return pottery_compare_max(POTTERY_COMPARE_CONTEXT_VAL b, c);
}
#endif
