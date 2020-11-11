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

#ifndef POTTERY_LIFECYCLE_INTERNAL
#error "This is an internal header. Do not include it."
#endif

/*
 * types
 */

#if defined(POTTERY_LIFECYCLE_VALUE_TYPE)
typedef POTTERY_LIFECYCLE_VALUE_TYPE pottery_lifecycle_value_t;
#endif

#ifdef POTTERY_LIFECYCLE_REF_TYPE
#define POTTERY_LIFECYCLE_RESTRICT /*nothing*/
typedef POTTERY_LIFECYCLE_REF_TYPE pottery_lifecycle_ref_t;
typedef POTTERY_LIFECYCLE_REF_TYPE pottery_lifecycle_const_ref_t;
#else
#define POTTERY_LIFECYCLE_RESTRICT pottery_restrict
typedef pottery_lifecycle_value_t* pottery_lifecycle_ref_t;
typedef const pottery_lifecycle_value_t* pottery_lifecycle_const_ref_t;
#endif

#ifdef POTTERY_LIFECYCLE_CONTEXT_TYPE
typedef POTTERY_LIFECYCLE_CONTEXT_TYPE pottery_lifecycle_context_t;
#endif



/*
 * fundamental lifecycle operations
 */

/*
 * destroy()
 */
#if POTTERY_LIFECYCLE_CAN_DESTROY
static inline
void pottery_lifecycle_destroy(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t element) pottery_noexcept
{
    POTTERY_LIFECYCLE_CONTEXT_MAYBE_UNUSED;

    #if defined(POTTERY_LIFECYCLE_DESTROY)
        // destroy by given expression
        #ifdef POTTERY_LIFECYCLE_CONTEXT_TYPE
            (POTTERY_LIFECYCLE_DESTROY((context), (element)));
        #else
            (POTTERY_LIFECYCLE_DESTROY((element)));
        #endif

    #elif POTTERY_LIFECYCLE_DESTROY_BY_VALUE
        #ifdef __cplusplus
            // run destructor
            element->~pottery_lifecycle_value_t();
        #else
            // nothing to do!
            (void)element;
        #endif

    #else
        #error "Lifecycle template bug! No destroy() implementation but CAN_DESTROY is 1"
    #endif
}
#endif

#if POTTERY_LIFECYCLE_CAN_MOVE
/*
 * move()
 *
 * Moves a value to a different storage location.
 *
 * In C++, this performs a reconstructing move (also called a non-trivial
 * relocation): it constructs a new value in the unconstructed destination
 * space by move construction from the source (terminating if any exception is
 * thrown), then destructs the source.
 */
static inline
void pottery_lifecycle_move(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t to, pottery_lifecycle_ref_t from) pottery_noexcept
{
    POTTERY_LIFECYCLE_CONTEXT_MAYBE_UNUSED;

    #if defined(POTTERY_LIFECYCLE_MOVE)
        // move by given expression
        #ifdef POTTERY_LIFECYCLE_CONTEXT_TYPE
            (POTTERY_LIFECYCLE_MOVE((context), (to), (from)));
        #else
            (POTTERY_LIFECYCLE_MOVE((to), (from)));
        #endif

    #elif POTTERY_LIFECYCLE_MOVE_BY_VALUE
        #if defined(__cplusplus)
            if (pottery::is_bitwise_movable<pottery_lifecycle_value_t>::value) {
                // move bitwise
                // cast to void* to prevent -Wclass-memaccess warnings
                pottery_memcpy(pottery_cast(void*, to), pottery_cast(const void*, from), sizeof(*to));
            } else {
                // move by move construction then destruction
                // This isn't allowed to fail so we don't bother with
                // try/catch. We don't assert for it though (with e.g.
                // is_nothrow_move_constructible) since most people don't
                // bother to mark their move constructors noexcept. An
                // exception here should terminate because this function is
                // noexcept.
                new (to) pottery_lifecycle_value_t(std::move(*from));
                from->~pottery_lifecycle_value_t();
            }
        #else
            // move by simple assignment
            *to = *from;
        #endif

    #else
        #error "Lifecycle template bug! No move() implementation but CAN_MOVE is 1"
    #endif
}
#endif

/*
 * swap()
 */
#if POTTERY_LIFECYCLE_CAN_SWAP
static inline
void pottery_lifecycle_swap(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t left, pottery_lifecycle_ref_t right) pottery_noexcept
{
    POTTERY_LIFECYCLE_CONTEXT_MAYBE_UNUSED;

    #ifdef POTTERY_LIFECYCLE_SWAP

        // swap by given expression
        #ifdef POTTERY_LIFECYCLE_CONTEXT_TYPE
            (POTTERY_LIFECYCLE_SWAP((context), (left), (right)));
        #else
            (POTTERY_LIFECYCLE_SWAP((left), (right)));
        #endif

    #elif POTTERY_LIFECYCLE_SWAP_BY_VALUE

        #if defined(__cplusplus)
        if (!pottery::is_bitwise_movable<pottery_lifecycle_value_t>::value) {
            // swap by ADL-swap() or std::swap()
            // We use using here to default to std::swap, but we call it undecorated to
            // allow another swap to be found by ADL. This means you can define a swap
            // function for your class in your own namespace and it will be used here.
            // If swap() throws we terminate (since we're noexcept.)
            using std::swap;
            swap(*left, *right);
            return;
        }
        #endif

        // swap bitwise through a temporary buffer
        // cast to void* to prevent -Wclass-memaccess warnings
        POTTERY_DECLARE_UNCONSTRUCTED(pottery_lifecycle_value_t, temp);
        pottery_memcpy(pottery_cast(void*, temp), pottery_cast(const void*, left),
                sizeof(pottery_lifecycle_value_t));
        pottery_memcpy(pottery_cast(void*, left), pottery_cast(const void*, right),
                sizeof(pottery_lifecycle_value_t));
        pottery_memcpy(pottery_cast(void*, right), pottery_cast(const void*, temp),
                sizeof(pottery_lifecycle_value_t));

    #elif (defined(POTTERY_LIFECYCLE_MOVE) || POTTERY_LIFECYCLE_MOVE_BY_VALUE) && \
            defined(POTTERY_LIFECYCLE_VALUE_TYPE)

        // swap by move through a temporary buffer
        POTTERY_DECLARE_UNCONSTRUCTED(pottery_lifecycle_value_t, temp);
        pottery_lifecycle_move(POTTERY_LIFECYCLE_CONTEXT_VAL temp, left);
        pottery_lifecycle_move(POTTERY_LIFECYCLE_CONTEXT_VAL left, right);
        pottery_lifecycle_move(POTTERY_LIFECYCLE_CONTEXT_VAL right, temp);

    #else
        #error "Lifecycle template bug! No swap() implementation but CAN_SWAP is 1"
    #endif
}
#endif

/*
 * steal()
 */
#if POTTERY_LIFECYCLE_CAN_STEAL
static inline
void pottery_lifecycle_steal(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t to, pottery_lifecycle_ref_t from) pottery_noexcept
{
    POTTERY_LIFECYCLE_CONTEXT_MAYBE_UNUSED;

    #if defined(POTTERY_LIFECYCLE_STEAL)
        // steal by given expression
        #ifdef POTTERY_LIFECYCLE_CONTEXT_TYPE
            (POTTERY_LIFECYCLE_STEAL((context), (to), (from)));
        #else
            (POTTERY_LIFECYCLE_STEAL((to), (from)));
        #endif

    #elif POTTERY_LIFECYCLE_STEAL_BY_VALUE
        #if defined(__cplusplus)
            if (!pottery::is_bitwise_movable<pottery_lifecycle_value_t>::value) {
                // steal by move assignment
                // Note that we don't use pottery::assign() here because we
                // don't want to catch any exceptions. steal() can't fail; if
                // this throws we terminate since we're noexcept.
                *to = std::move(*from);
                return;
            }

            // steal bitwise
            // cast to void* to prevent -Wclass-memaccess warnings
            pottery_memcpy(pottery_cast(void*, to), pottery_cast(const void*, from), sizeof(*to));
        #else
            // steal by simple assignment
            *to = *from;
        #endif

    #elif POTTERY_LIFECYCLE_CAN_SWAP
        // synthesize steal by swap
        pottery_lifecycle_swap(POTTERY_LIFECYCLE_CONTEXT_VAL to, from);

    #else
        #error "Lifecycle template bug! No steal() implementation but CAN_STEAL is 1"
    #endif
}
#endif

/*
 * init()
 */
#if POTTERY_LIFECYCLE_CAN_INIT
pottery_nodiscard
static inline
pottery_error_t pottery_lifecycle_init(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t element) pottery_noexcept
{
    POTTERY_LIFECYCLE_CONTEXT_MAYBE_UNUSED;

    #if defined(POTTERY_LIFECYCLE_INIT)
        // init by given expression
        #ifdef POTTERY_LIFECYCLE_CONTEXT_TYPE
            return (POTTERY_LIFECYCLE_INIT((context), (element)));
        #else
            return (POTTERY_LIFECYCLE_INIT((element)));
        #endif

    #elif POTTERY_LIFECYCLE_INIT_BY_VALUE
        #ifdef __cplusplus
            // run default constructor
            return pottery::construct(element);
        #else
            // nothing to do!
            (void)element;
            return POTTERY_OK;
        #endif

    #else
        #error "Lifecycle template bug! No init() implementation but CAN_INIT is 1"
    #endif
}
#endif

/*
 * copy()
 */
#if POTTERY_LIFECYCLE_CAN_COPY
pottery_nodiscard
static inline
pottery_error_t pottery_lifecycle_copy(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t to, pottery_lifecycle_const_ref_t from) pottery_noexcept
{
    POTTERY_LIFECYCLE_CONTEXT_MAYBE_UNUSED;

    #if defined(POTTERY_LIFECYCLE_COPY)
        // copy by given expression
        #ifdef POTTERY_LIFECYCLE_CONTEXT_TYPE
            return (POTTERY_LIFECYCLE_COPY((context), (to), (from)));
        #else
            return (POTTERY_LIFECYCLE_COPY((to), (from)));
        #endif

    #elif POTTERY_LIFECYCLE_COPY_BY_VALUE
        #if defined(__cplusplus)
            if (!pottery::is_bitwise_copyable<pottery_lifecycle_value_t>::value) {
                // copy by copy assignment
                return pottery::assign(to, *from);
            }

            // copy bitwise
            // cast to void* to prevent -Wclass-memaccess warnings
            pottery_memcpy(pottery_cast(void*, to), pottery_cast(const void*, from), sizeof(*to));
        #else
            // copy by simple assignment
            *to = *from;
        #endif
        return POTTERY_OK;

    #else
        #error "Lifecycle template bug! No copy() implementation but CAN_COPY is 1"
    #endif
}
#endif

/**
 * init_copy()
 */
#if POTTERY_LIFECYCLE_CAN_INIT_COPY
pottery_nodiscard
static inline
pottery_error_t pottery_lifecycle_init_copy(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t to, pottery_lifecycle_const_ref_t from) pottery_noexcept
{
    POTTERY_LIFECYCLE_CONTEXT_MAYBE_UNUSED;

    #if defined(POTTERY_LIFECYCLE_INIT_COPY)
        // init_copy by given expression
        #ifdef POTTERY_LIFECYCLE_CONTEXT_TYPE
            return (POTTERY_LIFECYCLE_INIT_COPY((context), (to), (from)));
        #else
            return (POTTERY_LIFECYCLE_INIT_COPY((to), (from)));
        #endif

    #elif POTTERY_LIFECYCLE_INIT_COPY_BY_VALUE
        #ifdef __cplusplus
            // init_copy by copy construction
            return pottery::construct(to, *from);
        #else
            // init_copy by simple assignment
            *to = *from;
            return POTTERY_OK;
        #endif

    #elif POTTERY_LIFECYCLE_CAN_INIT && POTTERY_LIFECYCLE_CAN_COPY
        // synthesize init_copy by init then copy
        pottery_error_t error = pottery_lifecycle_init(POTTERY_LIFECYCLE_CONTEXT_VAL to);
        if (error != POTTERY_OK)
            return error;
        error = pottery_lifecycle_copy(POTTERY_LIFECYCLE_CONTEXT_VAL to, from);
        if (error != POTTERY_OK)
            pottery_lifecycle_destroy(POTTERY_LIFECYCLE_CONTEXT_VAL to);
        return error;

    #else
        #error "Lifecycle template bug! No init_copy() implementation but CAN_INIT_COPY is 1"
    #endif
}
#endif

/**
 * init_steal()
 */
#if POTTERY_LIFECYCLE_CAN_INIT_STEAL
pottery_nodiscard
static inline
pottery_error_t pottery_lifecycle_init_steal(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t to, pottery_lifecycle_ref_t from) pottery_noexcept
{
    POTTERY_LIFECYCLE_CONTEXT_MAYBE_UNUSED;

    #if defined(POTTERY_LIFECYCLE_INIT_STEAL)

        // init_steal by given expression
        #ifdef POTTERY_LIFECYCLE_CONTEXT_TYPE
            return (POTTERY_LIFECYCLE_INIT_STEAL((context), (to), (from)));
        #else
            return (POTTERY_LIFECYCLE_INIT_STEAL((to), (from)));
        #endif
        return POTTERY_OK;

    #elif POTTERY_LIFECYCLE_INIT_STEAL_BY_VALUE
        #if defined(__cplusplus)
            // init_steal by move assignment
            return pottery::construct(to, std::move(*from));
        #else
            // init_steal by simple assignment
            *to = *from;
            return POTTERY_OK;
        #endif

    #elif POTTERY_LIFECYCLE_CAN_INIT && POTTERY_LIFECYCLE_CAN_SWAP
        // synthesize init_steal by init then swap
        pottery_error_t error = pottery_lifecycle_init(POTTERY_LIFECYCLE_CONTEXT_VAL to);
        if (error == POTTERY_OK)
            pottery_lifecycle_swap(POTTERY_LIFECYCLE_CONTEXT_VAL to, from);
        return error;

    #elif POTTERY_LIFECYCLE_CAN_INIT_COPY
        // synthesize init_steal by init_copy
        return pottery_lifecycle_init_copy(POTTERY_LIFECYCLE_CONTEXT_VAL to, from);

    #else
        #error "Lifecycle template bug! No init_steal() implementation but CAN_INIT_STEAL is 1"
    #endif
}
#endif



/*
 * bulk functions
 */

#if POTTERY_LIFECYCLE_CAN_MOVE && defined(POTTERY_LIFECYCLE_VALUE_TYPE)
static inline
void pottery_lifecycle_move_bulk_restrict(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t POTTERY_LIFECYCLE_RESTRICT to,
        pottery_lifecycle_ref_t POTTERY_LIFECYCLE_RESTRICT from,
        size_t count) pottery_noexcept
{
    pottery_assert(pottery_cast(size_t, from - to) >= count);
    pottery_assert(pottery_cast(size_t, to - from) >= count);

    #if POTTERY_LIFECYCLE_MOVE_BY_VALUE
    size_t bytes;
    if (true
            #ifdef __cplusplus
            && pottery::is_bitwise_movable<pottery_lifecycle_value_t>::value
            #endif
    ) {
        if (pottery_mul_overflow_s(count, sizeof(*to), &bytes))
            pottery_abort();
        // move bitwise with memcpy() since they don't overlap
        // cast to void* to prevent -Wclass-memaccess warnings
        pottery_memcpy(pottery_cast(void*, to), pottery_cast(const void*, from), bytes);
        return;
    }
    #endif

    // move by move()
    size_t i;
    for (i = 0; i < count; ++i)
        pottery_lifecycle_move(POTTERY_LIFECYCLE_CONTEXT_VAL
                to + i, from + i);
}

static inline
void pottery_lifecycle_move_bulk_down(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t to, pottery_lifecycle_ref_t from, size_t count) pottery_noexcept
{
    pottery_assert(to < from);

    #if POTTERY_LIFECYCLE_MOVE_BY_VALUE
    size_t bytes;
    if (true
            #ifdef __cplusplus
            && pottery::is_bitwise_movable<pottery_lifecycle_value_t>::value
            #endif
    ) {
        if (pottery_mul_overflow_s(count, sizeof(*to), &bytes))
            pottery_abort();
        // move bitwise with memmove() since they might overlap
        // cast to void* to prevent -Wmemaccess warnings
        pottery_memmove(pottery_cast(void*, to), pottery_cast(const void*, from), bytes);
        return;
    }
    #endif

    // move by move()
    size_t i;
    for (i = 0; i < count; ++i)
        pottery_lifecycle_move(POTTERY_LIFECYCLE_CONTEXT_VAL
                to + i, from + i);
}

static inline
void pottery_lifecycle_move_bulk_up(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t to, pottery_lifecycle_ref_t from, size_t count) pottery_noexcept
{
    pottery_assert(to > from);

    #if POTTERY_LIFECYCLE_MOVE_BY_VALUE
    size_t bytes;
    if (true
            #ifdef __cplusplus
            && pottery::is_bitwise_movable<pottery_lifecycle_value_t>::value
            #endif
    ) {
        if (pottery_mul_overflow_s(count, sizeof(*to), &bytes))
            pottery_abort();
        // move bitwise with memmove() since they might overlap
        // cast to void* to prevent -Wmemaccess warnings
        pottery_memmove(pottery_cast(void*, to), pottery_cast(const void*, from), bytes);
        return;
    }
    #endif

    // move by move(), starting at the end
    size_t i;
    for (i = count; i > 0;) {
        --i;
        pottery_lifecycle_move(POTTERY_LIFECYCLE_CONTEXT_VAL
                to + i, from + i);
    }
}

/**
 * Moves an array of values automatically. The arrays may or may not overlap,
 * and may have the same address (in which case nothing happens.)
 */
static inline
void pottery_lifecycle_move_bulk(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_ref_t to, pottery_lifecycle_ref_t from, size_t count) pottery_noexcept
{
    if (to == from)
        return;

    #if POTTERY_LIFECYCLE_MOVE_BY_VALUE
    size_t bytes;
    if (true
            #ifdef __cplusplus
            && pottery::is_bitwise_movable<pottery_lifecycle_value_t>::value
            #endif
    ) {
        if (pottery_mul_overflow_s(count, sizeof(*to), &bytes))
            pottery_abort();

        // Typical implementations of memmove() will do their own overlap
        // checks and switch to memcpy() if they don't so there's no point in
        // us testing for it here. We just defer to memmove().

        // cast to void* to prevent -Wmemaccess warnings
        pottery_memmove(pottery_cast(void*, to), pottery_cast(const void*, from), bytes);
        return;
    }
    #endif

    // there's no point in calling pottery_lifecycle_move_bulk_restrict() for
    // non-overlapping arrays here because it would do the same check as above
    // and the fallback is identical to move_bulk_down().
    if (to > from)
        pottery_lifecycle_move_bulk_up(POTTERY_LIFECYCLE_CONTEXT_VAL
                to, from, count);
    else
        pottery_lifecycle_move_bulk_down(POTTERY_LIFECYCLE_CONTEXT_VAL
                to, from, count);
}
#endif

#if POTTERY_LIFECYCLE_CAN_DESTROY && defined(POTTERY_LIFECYCLE_VALUE_TYPE)
static inline
void pottery_lifecycle_destroy_bulk(POTTERY_LIFECYCLE_CONTEXT_ARG
        pottery_lifecycle_value_t* element, size_t count) pottery_noexcept
{
    size_t i;
    for (i = 0; i < count; ++i)
        pottery_lifecycle_destroy(POTTERY_LIFECYCLE_CONTEXT_VAL element + i);
}
#endif
