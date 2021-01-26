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

#ifndef POTTERY_DOUBLY_LINKED_LIST_IMPL
#error "This is an internal header. Do not include it."
#endif

// The entry type is the same as the ref type. This makes everything simple:
// values can be accessed and removed simply by ref.
typedef pottery_dll_ref_t pottery_dll_entry_t;

typedef struct pottery_dll_t {
    #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
    pottery_dll_context_t context;
    #endif
    pottery_dll_ref_t head;
    pottery_dll_ref_t tail;
    size_t count;
} pottery_dll_t;



#if 0&&POTTERY_DEBUG
POTTERY_DOUBLY_LINKED_LIST_EXTERN void pottery_dll_sanity_check(pottery_dll_t* dll);
#else
pottery_always_inline static
void pottery_dll_sanity_check(pottery_dll_t* dll) {(void)dll;}
#endif

static inline
bool pottery_dll_ref_is_null(pottery_dll_t* dll, pottery_dll_ref_t ref) {
    (void)dll;
    return pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll)
            ref, pottery_dll_null(dll));
}

static inline
bool pottery_dll_entry_exists(pottery_dll_t* dll, pottery_dll_entry_t entry) {
    (void)dll;
    return !pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll)
            entry, pottery_dll_null(dll));
}

static inline
pottery_dll_ref_t pottery_dll_entry_ref(pottery_dll_t* dll, pottery_dll_entry_t entry) {
    (void)dll;
    pottery_assert(pottery_dll_entry_exists(dll, entry));
    return entry;
}



/*
 * next/previous accessors
 */

static inline
pottery_dll_ref_t pottery_dll_next_ref(pottery_dll_t* dll, pottery_dll_ref_t ref) {
    pottery_assert(dll != NULL);
    pottery_assert(!pottery_dll_ref_is_null(dll, ref));

    #ifdef POTTERY_DOUBLY_LINKED_LIST_NEXT
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return POTTERY_DOUBLY_LINKED_LIST_NEXT((dll->context), ref);
        #else
            (void)dll;
            return POTTERY_DOUBLY_LINKED_LIST_NEXT(ref);
        #endif
    #else
        // If NEXT is not defined, the default is that ref is a pointer to a
        // struct in which the next pointer is stored in a field called "next"
        return ref->next;
    #endif
}

static inline
pottery_dll_ref_t pottery_dll_previous_ref(pottery_dll_t* dll, pottery_dll_ref_t ref) {
    pottery_assert(dll != NULL);
    pottery_assert(!pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll) ref, pottery_dll_null(dll)));

    #ifdef POTTERY_DOUBLY_LINKED_LIST_PREVIOUS
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            return POTTERY_DOUBLY_LINKED_LIST_PREVIOUS((dll->context), ref);
        #else
            (void)dll;
            return POTTERY_DOUBLY_LINKED_LIST_PREVIOUS(ref);
        #endif
    #else
        // If PREVIOUS is not defined, the default is that ref is a pointer to
        // a struct in which the previous pointer is stored in a field called
        // "previous"
        return ref->previous;
    #endif
}

static inline
void pottery_dll_set_next_ref(pottery_dll_t* dll,
        pottery_dll_ref_t ref, pottery_dll_ref_t next)
{
    (void)dll;
    pottery_assert(dll != NULL);
    pottery_assert(!pottery_dll_ref_is_null(dll, ref));

    #ifdef POTTERY_DOUBLY_LINKED_LIST_SET_NEXT
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            POTTERY_DOUBLY_LINKED_LIST_SET_NEXT((dll->context), (ref), (next));
        #else
            POTTERY_DOUBLY_LINKED_LIST_SET_NEXT((ref), (next));
        #endif
    #elif defined(POTTERY_DOUBLY_LINKED_LIST_NEXT)
        // If SET_NEXT is not defined but NEXT is, it must be an l-value.
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            POTTERY_DOUBLY_LINKED_LIST_NEXT((dll->context), (ref)) = next;
        #else
            POTTERY_DOUBLY_LINKED_LIST_NEXT((ref)) = next;
        #endif
    #else
        // Otherwise it's a "next" field.
        ref->next = next;
    #endif
}

static inline
void pottery_dll_set_previous_ref(pottery_dll_t* dll,
        pottery_dll_ref_t ref, pottery_dll_ref_t previous)
{
    pottery_assert(dll != NULL);
    pottery_assert(!pottery_dll_ref_is_null(dll, ref));

    #ifdef POTTERY_DOUBLY_LINKED_LIST_SET_PREVIOUS
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            POTTERY_DOUBLY_LINKED_LIST_SET_PREVIOUS((dll->context), (ref), (previous));
        #else
            (void)dll;
            POTTERY_DOUBLY_LINKED_LIST_SET_PREVIOUS((ref), (previous));
        #endif
    #elif defined(POTTERY_DOUBLY_LINKED_LIST_PREVIOUS)
        // If SET_PREVIOUS is not defined but PREVIOUS is, it must be an l-value.
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
            POTTERY_DOUBLY_LINKED_LIST_PREVIOUS((dll->context), (ref)) = previous;
        #else
            (void)dll;
            POTTERY_DOUBLY_LINKED_LIST_PREVIOUS((ref)) = previous;
        #endif
    #else
        // Otherwise it's a "previous" field.
        ref->previous = previous;
    #endif
}



static inline
pottery_dll_entry_t pottery_dll_next(pottery_dll_t* dll, pottery_dll_entry_t entry) {
    pottery_assert(dll != NULL);
    pottery_dll_sanity_check(dll);
    pottery_assert(pottery_dll_entry_exists(dll, entry));
    return pottery_dll_next_ref(dll, entry);
}

static inline
pottery_dll_ref_t pottery_dll_previous(pottery_dll_t* dll, pottery_dll_entry_t entry) {
    pottery_assert(dll != NULL);
    pottery_dll_sanity_check(dll);
    pottery_assert(pottery_dll_entry_exists(dll, entry));
    return pottery_dll_previous_ref(dll, entry);
}

static inline
pottery_dll_entry_t pottery_dll_begin(pottery_dll_t* dll) {
    return dll->head;
}

static inline
pottery_dll_entry_t pottery_dll_end(pottery_dll_t* dll) {
    (void)dll;
    return pottery_dll_null(dll);
}



static inline
void pottery_dll_init(pottery_dll_t* dll
        #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
        , pottery_dll_context_t context
        #endif
) {
    pottery_assert(dll != NULL);
    #if POTTERY_CONTAINER_TYPES_HAS_CONTEXT
    dll->context = context;
    #endif
    dll->head = pottery_dll_null(dll);
    dll->tail = pottery_dll_null(dll);
    dll->count = 0;
    pottery_dll_sanity_check(dll);
}

static inline
void pottery_dll_unlink_all(pottery_dll_t* dll) {
    pottery_assert(dll != NULL);
    pottery_dll_sanity_check(dll);
    dll->head = pottery_dll_null(dll);
    dll->tail = pottery_dll_null(dll);
    dll->count = 0;
    pottery_dll_sanity_check(dll);
}

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_swap(pottery_dll_t* left, pottery_dll_t* right);

POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_link_first(pottery_dll_t* dll, pottery_dll_entry_t entry);

POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_link_last(pottery_dll_t* dll, pottery_dll_entry_t entry);

POTTERY_DOUBLY_LINKED_LIST_EXTERN
pottery_dll_ref_t pottery_dll_unlink_first(pottery_dll_t* dll);

POTTERY_DOUBLY_LINKED_LIST_EXTERN
pottery_dll_ref_t pottery_dll_unlink_last(pottery_dll_t* dll);

POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_link_before(pottery_dll_t* dll, pottery_dll_entry_t relative_entry,
        pottery_dll_ref_t ref_to_link);

POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_link_after(pottery_dll_t* dll, pottery_dll_entry_t relative_entry,
        pottery_dll_ref_t ref_to_link);

POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_unlink(pottery_dll_t* dll, pottery_dll_entry_t entry);
#endif

static inline
pottery_dll_entry_t pottery_dll_first(pottery_dll_t* dll) {
    pottery_dll_sanity_check(dll);
    return dll->head;
}

static inline
pottery_dll_entry_t pottery_dll_last(pottery_dll_t* dll) {
    pottery_dll_sanity_check(dll);
    return dll->tail;
}

static inline
void pottery_dll_destroy(pottery_dll_t* dll) {
    pottery_assert(dll != NULL);

    // It is an error to destroy a non-empty list. The list contents must be
    // properly disposed of.
    pottery_assert(dll->count == 0);
    pottery_dll_sanity_check(dll);
}

static inline
size_t pottery_dll_count(pottery_dll_t* dll) {
    pottery_dll_sanity_check(dll);
    return dll->count;
}

static inline
bool pottery_dll_is_empty(pottery_dll_t* dll) {
    pottery_dll_sanity_check(dll);
    return dll->count == 0;
}
