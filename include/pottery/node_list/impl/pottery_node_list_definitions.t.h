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

// Allocate a node
static inline
pottery_nl_node_t* pottery_nl_alloc_node(pottery_nl_t* list) {
    (void)list;
    void* ret = pottery_nl_alloc_malloc(
            POTTERY_NODE_LIST_CONTEXT_VAL(list)
                pottery_alignof(pottery_nl_node_t),
                sizeof(pottery_nl_node_t));
    return pottery_cast(pottery_nl_node_t*, ret);
}

// Free a node
static inline
void pottery_nl_free_node(pottery_nl_t* list, pottery_nl_node_t* node) {
    (void)list;
    pottery_nl_alloc_free(
            POTTERY_NODE_LIST_CONTEXT_VAL(list)
            pottery_alignof(pottery_nl_node_t),
            node);
}

POTTERY_NODE_LIST_EXTERN
void pottery_nl_destroy(pottery_nl_t* list) {

    #if POTTERY_LIFECYCLE_CAN_DESTROY
    pottery_nl_remove_all(list);
    #else
    // If we don't have a destroy expression, you must manually empty the list
    // before destroying it.
    pottery_assert(pottery_nl_is_empty(list));
    #endif
}


// displace

POTTERY_NODE_LIST_EXTERN
void pottery_nl_displace(pottery_nl_t* list, pottery_nl_entry_t entry) {
    pottery_nl_node_t* node = pottery_nl_entry_to_node(entry);
    pottery_nl_dll_unlink(&list->dll, node);
    pottery_nl_free_node(list, node);
}

POTTERY_NODE_LIST_EXTERN
void pottery_nl_displace_all(pottery_nl_t* list) {
    pottery_nl_node_t* node = pottery_nl_dll_begin(&list->dll);
    while (node != pottery_null) {
        pottery_nl_node_t* next = pottery_nl_dll_next(&list->dll, node);
        pottery_nl_free_node(list, node);
        node = next;
    }
    pottery_nl_dll_unlink_all(&list->dll);
}

POTTERY_NODE_LIST_EXTERN
void pottery_nl_displace_first(pottery_nl_t* list) {
    pottery_nl_free_node(list, pottery_nl_dll_unlink_first(&list->dll));
}

POTTERY_NODE_LIST_EXTERN
void pottery_nl_displace_last(pottery_nl_t* list) {
    pottery_nl_free_node(list, pottery_nl_dll_unlink_last(&list->dll));
}


// remove

#if POTTERY_LIFECYCLE_CAN_DESTROY
POTTERY_NODE_LIST_EXTERN
void pottery_nl_remove(pottery_nl_t* list, pottery_nl_entry_t entry) {
    pottery_nl_node_t* node = pottery_nl_entry_to_node(entry);
    pottery_nl_dll_unlink(&list->dll, node);
    pottery_nl_lifecycle_destroy(POTTERY_NODE_LIST_CONTEXT_VAL(list) &node->value);
    pottery_nl_free_node(list, node);
}

POTTERY_NODE_LIST_EXTERN
void pottery_nl_remove_all(pottery_nl_t* list) {
    pottery_nl_node_t* node = pottery_nl_dll_begin(&list->dll);
    while (node != pottery_null) {
        pottery_nl_node_t* next = pottery_nl_dll_next(&list->dll, node);
        pottery_nl_lifecycle_destroy(POTTERY_NODE_LIST_CONTEXT_VAL(list) &node->value);
        pottery_nl_free_node(list, node);
        node = next;
    }
    pottery_nl_dll_unlink_all(&list->dll);
}

POTTERY_NODE_LIST_EXTERN
void pottery_nl_remove_first(pottery_nl_t* list) {
    pottery_nl_node_t* node = pottery_nl_dll_unlink_first(&list->dll);
    pottery_nl_lifecycle_destroy(POTTERY_NODE_LIST_CONTEXT_VAL(list) &node->value);
    pottery_nl_free_node(list, node);
}

POTTERY_NODE_LIST_EXTERN
void pottery_nl_remove_last(pottery_nl_t* list) {
    pottery_nl_node_t* node = pottery_nl_dll_unlink_last(&list->dll);
    pottery_nl_lifecycle_destroy(POTTERY_NODE_LIST_CONTEXT_VAL(list) &node->value);
    pottery_nl_free_node(list, node);
}
#endif


// extract

#if POTTERY_LIFECYCLE_CAN_PASS
POTTERY_NODE_LIST_EXTERN
pottery_nl_value_t pottery_nl_extract(pottery_nl_t* list, pottery_nl_entry_t entry) {
    pottery_nl_node_t* node = pottery_nl_entry_to_node(entry);
    pottery_nl_dll_unlink(&list->dll, node);
    pottery_nl_value_t ret = pottery_move_if_cxx(node->value);
    #ifdef __cplusplus
    // We need to run the C++ destructor of the entry. See
    // pottery_vector_extract() for details.
    node->value.~pottery_nl_value_t();
    #endif
    pottery_nl_free_node(list, node);
    return ret;
}

POTTERY_NODE_LIST_EXTERN
pottery_nl_value_t pottery_nl_extract_first(pottery_nl_t* list) {
    return pottery_nl_extract(list, pottery_nl_node_to_entry(pottery_nl_dll_first(&list->dll)));
}

POTTERY_NODE_LIST_EXTERN
pottery_nl_value_t pottery_nl_extract_last(pottery_nl_t* list) {
    return pottery_nl_extract(list, pottery_nl_node_to_entry(pottery_nl_dll_last(&list->dll)));
}
#endif


// emplace

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_emplace_first(pottery_nl_t* list,
        pottery_nl_entry_t* emplaced_entry)
{
    pottery_nl_node_t* node = pottery_nl_alloc_node(list);
    *emplaced_entry = pottery_nl_node_to_entry(node);
    if (node == pottery_null)
        return POTTERY_ERROR_ALLOC;
    pottery_nl_dll_link_first(&list->dll, node);
    return POTTERY_OK;
}

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_emplace_last(pottery_nl_t* list,
        pottery_nl_entry_t* emplaced_entry)
{
    pottery_nl_node_t* node = pottery_nl_alloc_node(list);
    *emplaced_entry = pottery_nl_node_to_entry(node);
    if (node == pottery_null)
        return POTTERY_ERROR_ALLOC;
    pottery_nl_dll_link_last(&list->dll, node);
    return POTTERY_OK;
}

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_emplace_after(pottery_nl_t* list,
        pottery_nl_entry_t relative_entry, pottery_nl_entry_t* emplaced_entry)
{
    pottery_nl_node_t* node = pottery_nl_alloc_node(list);
    *emplaced_entry = pottery_nl_node_to_entry(node);
    if (node == pottery_null)
        return POTTERY_ERROR_ALLOC;
    pottery_nl_dll_link_after(&list->dll, pottery_nl_entry_to_node(relative_entry), node);
    return POTTERY_OK;
}

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_emplace_before(pottery_nl_t* list,
        pottery_nl_entry_t relative_entry, pottery_nl_entry_t* emplaced_entry)
{
    pottery_nl_node_t* node = pottery_nl_alloc_node(list);
    *emplaced_entry = pottery_nl_node_to_entry(node);
    if (node == pottery_null)
        return POTTERY_ERROR_ALLOC;
    pottery_nl_dll_link_before(&list->dll, pottery_nl_entry_to_node(relative_entry), node);
    return POTTERY_OK;
}


// insert

#if POTTERY_LIFECYCLE_CAN_PASS
POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_insert_first(pottery_nl_t* list, pottery_nl_value_t value) {
    pottery_nl_node_t* node = pottery_nl_alloc_node(list);
    if (node == pottery_null)
        return POTTERY_ERROR_ALLOC;
    pottery_move_construct(pottery_nl_value_t, node->value, value);
    pottery_nl_dll_link_first(&list->dll, node);
    return POTTERY_OK;
}

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_insert_last(pottery_nl_t* list, pottery_nl_value_t value) {
    pottery_nl_node_t* node = pottery_nl_alloc_node(list);
    if (node == pottery_null)
        return POTTERY_ERROR_ALLOC;
    pottery_move_construct(pottery_nl_value_t, node->value, value);
    pottery_nl_dll_link_last(&list->dll, node);
    return POTTERY_OK;
}

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_insert_before(pottery_nl_t* list,
        pottery_nl_entry_t relative_entry, pottery_nl_value_t value)
{
    pottery_nl_node_t* node = pottery_nl_alloc_node(list);
    if (node == pottery_null)
        return POTTERY_ERROR_ALLOC;
    pottery_move_construct(pottery_nl_value_t, node->value, value);
    pottery_nl_dll_link_before(&list->dll, pottery_nl_entry_to_node(relative_entry), node);
    return POTTERY_OK;
}

POTTERY_NODE_LIST_EXTERN
pottery_error_t pottery_nl_insert_after(pottery_nl_t* list,
        pottery_nl_entry_t relative_entry, pottery_nl_value_t value)
{
    pottery_nl_node_t* node = pottery_nl_alloc_node(list);
    if (node == pottery_null)
        return POTTERY_ERROR_ALLOC;
    pottery_move_construct(pottery_nl_value_t, node->value, value);
    pottery_nl_dll_link_after(&list->dll, pottery_nl_entry_to_node(relative_entry), node);
    return POTTERY_OK;
}
#endif
