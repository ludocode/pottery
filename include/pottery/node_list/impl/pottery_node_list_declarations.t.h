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

#ifndef POTTERY_NODE_LIST_IMPL
#error "This is an internal header. Do not include it."
#endif

// The entry type is the same as the ref type, which is also equivalent to the
// node type. This means entry->ref conversions can be done implicitly. This
// makes the node list easier to use since you don't need to call entry_ref(),
// entry_exists(), etc. (It is potentially more error-prone though since you
// can pass arbitrary value pointers directly to functions expecting entries
// even if they're not actually entries in the list.)
typedef pottery_nl_ref_t pottery_nl_entry_t;

pottery_always_inline static
pottery_nl_entry_t pottery_nl_node_to_entry(pottery_nl_node_t* node) {
    // The value is the first field in the node. We cast rather than
    // dereference since the node may be null. Note that a cast between a
    // pointer to struct and a pointer to its first field is allowed under
    // C aliasing rules.
    return pottery_reinterpret_cast(pottery_nl_entry_t, node);
}

pottery_always_inline static
pottery_nl_node_t* pottery_nl_entry_to_node(pottery_nl_entry_t entry) {
    // As above, we cast rather than use container_of.
    return pottery_reinterpret_cast(pottery_nl_node_t*, entry);
}

typedef struct pottery_nl_t {
    #if POTTERY_NODE_LIST_HAS_CONTEXT
    pottery_ohm_context_t context;
    #endif
    pottery_nl_dll_t dll;
} pottery_nl_t;

// TODO this is temporarily here for backwards compatibility
static inline
pottery_nl_value_t* pottery_nl_entry_value(pottery_nl_t* list, pottery_nl_entry_t entry) {
    (void)list;
    return entry;
}

static inline
pottery_nl_value_t* pottery_nl_entry_ref(pottery_nl_t* list, pottery_nl_entry_t entry) {
    (void)list;
    return entry;
}

static inline
bool pottery_nl_entry_exists(pottery_nl_t* list, pottery_nl_entry_t entry) {
    (void)list;
    return entry != pottery_null;
}

static inline
pottery_error_t pottery_nl_init(pottery_nl_t* list
        #if POTTERY_NODE_LIST_HAS_CONTEXT
        , pottery_nl_context_t context
        #endif
        )
{
    #if POTTERY_NODE_LIST_HAS_CONTEXT
    list->context = context;
    #endif
    pottery_nl_dll_init(&list->dll);
    return POTTERY_OK;
}

POTTERY_NODE_LIST_EXTERN
void pottery_nl_destroy(pottery_nl_t* list);

static inline
size_t pottery_nl_count(pottery_nl_t* list) {
    return pottery_nl_dll_count(&list->dll);
}

static inline
bool pottery_nl_is_empty(pottery_nl_t* list) {
    return pottery_nl_dll_is_empty(&list->dll);
}


// displace

POTTERY_NODE_LIST_EXTERN
void pottery_nl_displace(pottery_nl_t* list, pottery_nl_entry_t entry);

POTTERY_NODE_LIST_EXTERN
void pottery_nl_displace_all(pottery_nl_t* list);

POTTERY_NODE_LIST_EXTERN
void pottery_nl_displace_first(pottery_nl_t* list);

POTTERY_NODE_LIST_EXTERN
void pottery_nl_displace_last(pottery_nl_t* list);


// remove

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_NODE_LIST_EXTERN
void pottery_nl_remove(pottery_nl_t* list, pottery_nl_entry_t entry);

POTTERY_NODE_LIST_EXTERN
void pottery_nl_remove_all(pottery_nl_t* list);

POTTERY_NODE_LIST_EXTERN
void pottery_nl_remove_first(pottery_nl_t* list);

POTTERY_NODE_LIST_EXTERN
void pottery_nl_remove_last(pottery_nl_t* list);
#endif


// extract

#if POTTERY_LIFECYCLE_CAN_PASS
POTTERY_NODE_LIST_EXTERN
pottery_nl_value_t pottery_nl_extract(pottery_nl_t* list, pottery_nl_entry_t entry);

POTTERY_NODE_LIST_EXTERN
pottery_nl_value_t pottery_nl_extract_first(pottery_nl_t* list);

POTTERY_NODE_LIST_EXTERN
pottery_nl_value_t pottery_nl_extract_last(pottery_nl_t* list);
#endif


// emplace

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_emplace_first(pottery_nl_t* nl,
        pottery_nl_entry_t* emplaced_entry);

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_emplace_last(pottery_nl_t* nl,
        pottery_nl_entry_t* emplaced_entry);

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_emplace_after(pottery_nl_t* nl,
        pottery_nl_entry_t relative_entry, pottery_nl_entry_t* emplaced_entry);

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_emplace_before(pottery_nl_t* nl,
        pottery_nl_entry_t relative_entry, pottery_nl_entry_t* emplaced_entry);


// insert

#if POTTERY_LIFECYCLE_CAN_PASS
POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_insert_first(pottery_nl_t* nl, pottery_nl_value_t value);

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_insert_last(pottery_nl_t* nl, pottery_nl_value_t value);

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_insert_before(pottery_nl_t* nl,
        pottery_nl_entry_t relative_entry, pottery_nl_value_t value);

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_insert_after(pottery_nl_t* nl,
        pottery_nl_entry_t relative_entry, pottery_nl_value_t value);
#endif


// iteration

static inline
pottery_nl_entry_t pottery_nl_begin(pottery_nl_t* list) {
    return pottery_nl_node_to_entry(pottery_nl_dll_begin(&list->dll));
}

static inline
pottery_nl_entry_t pottery_nl_end(pottery_nl_t* list) {
    return pottery_nl_node_to_entry(pottery_nl_dll_end(&list->dll));
}

static inline
pottery_nl_entry_t pottery_nl_first(pottery_nl_t* list) {
    return pottery_nl_node_to_entry(pottery_nl_dll_first(&list->dll));
}

static inline
pottery_nl_entry_t pottery_nl_last(pottery_nl_t* list) {
    return pottery_nl_node_to_entry(pottery_nl_dll_last(&list->dll));
}

static inline
pottery_nl_entry_t pottery_nl_next(pottery_nl_t* list, pottery_nl_entry_t entry) {
    return pottery_nl_node_to_entry(pottery_nl_dll_next(&list->dll,
                pottery_nl_entry_to_node(entry)));
}

static inline
pottery_nl_entry_t pottery_nl_previous(pottery_nl_t* list, pottery_nl_entry_t entry) {
    return pottery_nl_node_to_entry(pottery_nl_dll_previous(&list->dll,
                pottery_nl_entry_to_node(entry)));
}
