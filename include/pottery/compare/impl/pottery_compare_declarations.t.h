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

#ifndef POTTERY_COMPARE_INTERNAL
#error "This is an internal header. Do not include it."
#endif

#if defined(POTTERY_COMPARE_VALUE_TYPE)
typedef POTTERY_COMPARE_VALUE_TYPE pottery_compare_value_t;
#endif

#ifdef POTTERY_COMPARE_REF_TYPE
typedef POTTERY_COMPARE_REF_TYPE pottery_compare_ref_t;
#else
typedef pottery_compare_value_t* pottery_compare_ref_t;
#endif

#ifdef POTTERY_COMPARE_ENTRY_TYPE
typedef POTTERY_COMPARE_ENTRY_TYPE pottery_compare_entry_t;
#else
typedef pottery_compare_ref_t pottery_compare_entry_t;
#endif

#ifdef POTTERY_COMPARE_CONTEXT_TYPE
typedef POTTERY_COMPARE_CONTEXT_TYPE pottery_compare_context_t;
#endif



/**
 * Returns a ref for an entry.
 *
 * The entry must exist.
 */
static inline
pottery_compare_ref_t pottery_compare_value(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_entry_t entry)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #ifdef POTTERY_COMPARE_ENTRY_VALUE
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_ENTRY_VALUE((context), (entry)));
        #else
            return (POTTERY_COMPARE_ENTRY_VALUE((entry)));
        #endif
    #else
        // Without an ENTRY_VALUE expression, the entry type must implicitly
        // convertible to the ref type (usually it's the same type.)
        return entry;
    #endif
}



#if POTTERY_COMPARE_CAN_EQUAL
static inline
bool pottery_compare_equal(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_entry_t left, pottery_compare_entry_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL left)
                ==
                *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL right);

    #elif defined(POTTERY_COMPARE_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_EQUAL((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_NOT_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_NOT_EQUAL((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_NOT_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return 0 == (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 == (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_LESS((context), (left), (right))) &&
                    !(POTTERY_COMPARE_LESS((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_LESS((left), (right))) &&
                    !(POTTERY_COMPARE_LESS((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_GREATER((context), (left), (right))) &&
                    !(POTTERY_COMPARE_GREATER((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_GREATER((left), (right))) &&
                    !(POTTERY_COMPARE_GREATER((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_LESS_OR_EQUAL((context), (left), (right))) &&
                    (POTTERY_COMPARE_LESS_OR_EQUAL((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_LESS_OR_EQUAL((left), (right))) &&
                    (POTTERY_COMPARE_LESS_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
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
        pottery_compare_entry_t left, pottery_compare_entry_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL left)
                !=
                *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL right);

    #elif defined(POTTERY_COMPARE_NOT_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_NOT_EQUAL((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_NOT_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_EQUAL((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return 0 != (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 != (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_LESS((context), (left), (right))) ||
                    (POTTERY_COMPARE_LESS((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_LESS((left), (right))) ||
                    (POTTERY_COMPARE_LESS((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_GREATER((context), (left), (right))) ||
                    (POTTERY_COMPARE_GREATER((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_GREATER((left), (right))) ||
                    (POTTERY_COMPARE_GREATER((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((context), (left), (right))) ||
                    !(POTTERY_COMPARE_LESS_OR_EQUAL((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((left), (right))) ||
                    !(POTTERY_COMPARE_LESS_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
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
        pottery_compare_entry_t left, pottery_compare_entry_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL left)
                <
                *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL right);

    #elif defined(POTTERY_COMPARE_LESS)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_LESS((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_LESS((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_GREATER((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_GREATER((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_GREATER_OR_EQUAL((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_GREATER_OR_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return 0 > (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 > (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #endif
}

static inline
bool pottery_compare_greater(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_entry_t left, pottery_compare_entry_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL left)
                >
                *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL right);

    #elif defined(POTTERY_COMPARE_GREATER)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_GREATER((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_GREATER((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_LESS((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_LESS((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_LESS_OR_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_GREATER_OR_EQUAL((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_GREATER_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return 0 < (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 < (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #endif
}

static inline
bool pottery_compare_less_or_equal(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_entry_t left, pottery_compare_entry_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL left)
                <=
                *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL right);

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_LESS_OR_EQUAL((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_LESS_OR_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_GREATER_OR_EQUAL((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_GREATER_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_GREATER((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_GREATER((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_LESS((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_LESS((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return 0 >= (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 >= (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #endif
}

static inline
bool pottery_compare_greater_or_equal(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_entry_t left, pottery_compare_entry_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL left)
                >=
                *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL right);

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_GREATER_OR_EQUAL((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_GREATER_OR_EQUAL((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_LESS_OR_EQUAL((context), (right), (left)));
        #else
            return (POTTERY_COMPARE_LESS_OR_EQUAL((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_LESS((context), (left), (right)));
        #else
            return !(POTTERY_COMPARE_LESS((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return !(POTTERY_COMPARE_GREATER((context), (right), (left)));
        #else
            return !(POTTERY_COMPARE_GREATER((right), (left)));
        #endif

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return 0 <= (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return 0 <= (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #endif
}

static inline
int pottery_compare_three_way(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_entry_t left, pottery_compare_entry_t right)
{
    POTTERY_COMPARE_CONTEXT_MAYBE_UNUSED;

    #if POTTERY_COMPARE_BY_VALUE
        return (*pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL left)
                    <
                    *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL right)) ?
                -1 :
                    (*pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL left)
                    >
                    *pottery_compare_value(POTTERY_COMPARE_CONTEXT_VAL right) ?
                1 : 0);

    #elif defined(POTTERY_COMPARE_THREE_WAY)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_THREE_WAY((context), (left), (right)));
        #else
            return (POTTERY_COMPARE_THREE_WAY((left), (right)));
        #endif

    #elif defined(POTTERY_COMPARE_LESS)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_LESS((context), (left), (right))) ? -1 :
                    ((POTTERY_COMPARE_LESS((context), (right), (left))) ? 1 : 0);
        #else
            return (POTTERY_COMPARE_LESS((left), (right))) ? -1 :
                    ((POTTERY_COMPARE_LESS((right), (left))) ? 1 : 0);
        #endif

    #elif defined(POTTERY_COMPARE_GREATER)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (POTTERY_COMPARE_GREATER((context), (left), (right))) ? 1 :
                    ((POTTERY_COMPARE_GREATER((context), (right), (left))) ? -1 : 0);
        #else
            return (POTTERY_COMPARE_GREATER((left), (right))) ? 1 :
                    ((POTTERY_COMPARE_GREATER((right), (left))) ? -1 : 0);
        #endif

    #elif defined(POTTERY_COMPARE_LESS_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
            return (!POTTERY_COMPARE_LESS_OR_EQUAL((context), (left), (right))) ? 1 :
                    (!(POTTERY_COMPARE_LESS_OR_EQUAL((context), (right), (left))) ? -1 : 0);
        #else
            return (!POTTERY_COMPARE_LESS_OR_EQUAL((left), (right))) ? 1 :
                    (!(POTTERY_COMPARE_LESS_OR_EQUAL((right), (left))) ? -1 : 0);
        #endif

    #elif defined(POTTERY_COMPARE_GREATER_OR_EQUAL)
        #ifdef POTTERY_COMPARE_CONTEXT_TYPE
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
pottery_compare_entry_t pottery_compare_min(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_entry_t left, pottery_compare_entry_t right)
{
    return pottery_compare_less(POTTERY_COMPARE_CONTEXT_VAL
            left, right) ? left : right;
}

static inline
pottery_compare_entry_t pottery_compare_max(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_entry_t left, pottery_compare_entry_t right)
{
    return pottery_compare_greater(POTTERY_COMPARE_CONTEXT_VAL
            left, right) ? left : right;
}

static inline
pottery_compare_entry_t pottery_compare_clamp(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_entry_t value, pottery_compare_entry_t min, pottery_compare_entry_t max)
{
    return pottery_compare_less(POTTERY_COMPARE_CONTEXT_VAL
                value, min) ? min :
            pottery_compare_greater(POTTERY_COMPARE_CONTEXT_VAL
                value, max) ? max : value;
}

static inline
pottery_compare_entry_t pottery_compare_median(POTTERY_COMPARE_CONTEXT_ARG
        pottery_compare_entry_t a, pottery_compare_entry_t b, pottery_compare_entry_t c)
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
