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



#if POTTERY_DEBUG
POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_sanity_check(pottery_dll_t* dll) {

    // Head and tail should be null if and only if the list is empty.
    pottery_assert((dll->count == 0) == pottery_dll_ref_is_null(dll, dll->head));
    pottery_assert((dll->count == 0) == pottery_dll_ref_is_null(dll, dll->tail));
    if (dll->count == 0)
        return;

    // Previous of first and next of last should always be null.
    pottery_assert(pottery_dll_ref_is_null(dll, pottery_dll_previous_ref(dll, dll->head)));
    pottery_assert(pottery_dll_ref_is_null(dll, pottery_dll_next_ref(dll, dll->tail)));

    // If we have only one element, head and tail should point to it.
    if (dll->count == 1) {
        pottery_assert(pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll)
                    dll->head, dll->tail));
        return;
    }

    // If we have only two elements, they should point to each other.
    if (dll->count == 2) {
        pottery_assert(pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll)
                    pottery_dll_next_ref(dll, dll->head), dll->tail));
        pottery_assert(pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll)
                    pottery_dll_previous_ref(dll, dll->tail), dll->head));
        return;
    }


    // Make sure the list doesn't have a loop

    pottery_dll_ref_t previous = pottery_dll_null(dll);
    pottery_dll_ref_t full = dll->head;
    pottery_dll_ref_t half = dll->head;
    bool skip = true;
    size_t count = 2;

    while (!pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll)
                pottery_dll_next_ref(dll, full), dll->tail))
    {
        pottery_assert(pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll)
                    pottery_dll_previous_ref(dll, full), previous));
        previous = full;
        ++count;

        if (!skip)
            half = pottery_dll_next_ref(dll, half);
        skip = !skip;
        full = pottery_dll_next_ref(dll, full);

        pottery_assert(!pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll) full, half));
    }
    pottery_assert(pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll)
                pottery_dll_previous_ref(dll, dll->tail), full));
    pottery_assert(count == dll->count);
}
#endif



POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_swap(pottery_dll_t* left, pottery_dll_t* right) {
    pottery_dll_sanity_check(left);
    pottery_dll_sanity_check(right);

    pottery_dll_t temp = *left;
    *left = *right;
    *right = temp;
}

POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_link_first(pottery_dll_t* dll, pottery_dll_ref_t ref) {
    pottery_dll_sanity_check(dll);
    pottery_assert(!pottery_dll_ref_is_null(dll, ref));

    ++dll->count;

    pottery_dll_set_previous_ref(dll, ref, pottery_dll_null(dll));

    if (pottery_dll_ref_is_null(dll, dll->head)) {
        dll->tail = ref;
    } else {
        pottery_dll_set_previous_ref(dll, dll->head, ref);
    }

    pottery_dll_set_next_ref(dll, ref, dll->head);
    dll->head = ref;

    pottery_dll_sanity_check(dll);
}

POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_link_last(pottery_dll_t* dll, pottery_dll_ref_t ref) {
    pottery_dll_sanity_check(dll);
    pottery_assert(!pottery_dll_ref_is_null(dll, ref));

    ++dll->count;

    pottery_dll_set_previous_ref(dll, ref, dll->tail);

    if (pottery_dll_ref_is_null(dll, dll->tail)) {
        dll->head = ref;
    } else {
        pottery_dll_set_next_ref(dll, dll->tail, ref);
    }

    pottery_dll_set_next_ref(dll, ref, pottery_dll_null(dll));
    dll->tail = ref;

    pottery_dll_sanity_check(dll);
}

POTTERY_DOUBLY_LINKED_LIST_EXTERN
pottery_dll_ref_t pottery_dll_unlink_first(pottery_dll_t* dll) {
    pottery_dll_sanity_check(dll);
    pottery_dll_ref_t ref = dll->head;
    pottery_assert(!pottery_dll_ref_is_null(dll, ref));
    pottery_dll_unlink(dll, ref);
    return ref;
}

POTTERY_DOUBLY_LINKED_LIST_EXTERN
pottery_dll_ref_t pottery_dll_unlink_last(pottery_dll_t* dll) {
    pottery_dll_sanity_check(dll);
    pottery_dll_ref_t ref = dll->tail;
    pottery_assert(!pottery_dll_ref_is_null(dll, ref));
    pottery_dll_unlink(dll, ref);
    return ref;
}

POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_link_before(pottery_dll_t* dll, pottery_dll_entry_t relative_entry,
        pottery_dll_ref_t ref_to_link)
{
    pottery_dll_sanity_check(dll);
    pottery_assert(!pottery_dll_ref_is_null(dll, ref_to_link));

    ++dll->count;

    if (pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll) dll->head, relative_entry)) {
        dll->head = ref_to_link;
    } else {
        pottery_dll_set_next_ref(dll, pottery_dll_previous_ref(dll, relative_entry), ref_to_link);
    }

    pottery_dll_set_previous_ref(dll, ref_to_link, pottery_dll_previous_ref(dll, relative_entry));
    pottery_dll_set_next_ref(dll, ref_to_link, relative_entry);

    pottery_dll_set_previous_ref(dll, relative_entry, ref_to_link);

    pottery_dll_sanity_check(dll);
}

POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_link_after(pottery_dll_t* dll, pottery_dll_entry_t relative_entry,
        pottery_dll_ref_t ref_to_link)
{
    pottery_dll_sanity_check(dll);
    pottery_assert(!pottery_dll_ref_is_null(dll, ref_to_link));
    pottery_assert(pottery_dll_entry_exists(dll, relative_entry));

    ++dll->count;

    if (pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll) dll->tail, relative_entry)) {
        dll->tail = ref_to_link;
    } else {
        pottery_dll_set_previous_ref(dll, pottery_dll_next_ref(dll, relative_entry), ref_to_link);
    }

    pottery_dll_set_next_ref(dll, ref_to_link, pottery_dll_next_ref(dll, relative_entry));
    pottery_dll_set_previous_ref(dll, ref_to_link, relative_entry);

    pottery_dll_set_next_ref(dll, relative_entry, ref_to_link);

    pottery_dll_sanity_check(dll);
}

POTTERY_DOUBLY_LINKED_LIST_EXTERN
void pottery_dll_unlink(pottery_dll_t* dll, pottery_dll_ref_t entry) {
    pottery_dll_sanity_check(dll);
    pottery_assert(!pottery_dll_is_empty(dll));
    pottery_assert(pottery_dll_entry_exists(dll, entry));

    --dll->count;

    pottery_dll_ref_t next = pottery_dll_next_ref(dll, entry);
    pottery_dll_ref_t previous = pottery_dll_previous_ref(dll, entry);

    if (pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll) entry, dll->head)) {
        dll->head = next;
    } else {
        pottery_dll_set_next_ref(dll, previous, next);
    }

    if (pottery_dll_ref_equal(POTTERY_DLL_CONTEXT_VAL(dll) entry, dll->tail)) {
        dll->tail = previous;
    } else {
        pottery_dll_set_previous_ref(dll, next, previous);
    }

    pottery_dll_sanity_check(dll);
}
