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

#ifndef POTTERY_RED_BLACK_TREE_IMPL
#error "This is an internal header. Do not include it."
#endif

// The entry type is the same as the ref type. This makes everything simple:
// values can be accessed and removed simply by ref.
typedef pottery_rbt_ref_t pottery_rbt_entry_t;

typedef struct pottery_rbt_t {
    #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
    pottery_rbt_context_t context;
    #endif
    pottery_rbt_ref_t root;
    size_t count;
} pottery_rbt_t;

// Location struct use for find_location()/link_location()
typedef struct pottery_rbt_location_t {
    pottery_rbt_entry_t parent; // Null if new entry should be root
    int compare; // if <0, new entry should be left child; if >0, right child
} pottery_rbt_location_t;



#if 0&&POTTERY_DEBUG
POTTERY_RED_BLACK_TREE_EXTERN void pottery_rbt_sanity_check(pottery_rbt_t* rbt);
#else
pottery_always_inline static
void pottery_rbt_sanity_check(pottery_rbt_t* rbt) {(void)rbt;}
#endif

static inline
bool pottery_rbt_ref_is_null(pottery_rbt_t* rbt, pottery_rbt_ref_t ref) {
    (void)rbt;
    return pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
            ref, pottery_rbt_null(rbt));
}

static inline
bool pottery_rbt_entry_exists(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    (void)rbt;
    return !pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
            entry, pottery_rbt_null(rbt));
}

static inline
pottery_rbt_ref_t pottery_rbt_entry_ref(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    (void)rbt;
    pottery_assert(pottery_rbt_entry_exists(rbt, entry));
    return entry;
}



/*
 * node link and color accessors
 */

static inline
pottery_rbt_ref_t pottery_rbt_left_child(pottery_rbt_t* rbt, pottery_rbt_ref_t ref) {
    pottery_assert(rbt != NULL);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, ref));

    #ifdef POTTERY_RED_BLACK_TREE_LEFT_CHILD
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            return POTTERY_RED_BLACK_TREE_LEFT_CHILD((rbt->context), ref);
        #else
            (void)rbt;
            return POTTERY_RED_BLACK_TREE_LEFT_CHILD(ref);
        #endif
    #else
        // If LEFT_CHILD is not defined, the default is that ref is a pointer
        // to a struct in which the left_child pointer is stored in a field
        // called "left_child"
        (void)rbt;
        return ref->left_child;
    #endif
}

static inline
pottery_rbt_ref_t pottery_rbt_right_child(pottery_rbt_t* rbt, pottery_rbt_ref_t ref) {
    pottery_assert(rbt != NULL);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, ref));

    #ifdef POTTERY_RED_BLACK_TREE_RIGHT_CHILD
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            return POTTERY_RED_BLACK_TREE_RIGHT_CHILD((rbt->context), ref);
        #else
            (void)rbt;
            return POTTERY_RED_BLACK_TREE_RIGHT_CHILD(ref);
        #endif
    #else
        // If RIGHT_CHILD is not defined, the default is that ref is a pointer
        // to a struct in which the right_child pointer is stored in a field
        // called "right_child"
        (void)rbt;
        return ref->right_child;
    #endif
}

static inline
pottery_rbt_ref_t pottery_rbt_parent(pottery_rbt_t* rbt, pottery_rbt_ref_t ref) {
    pottery_assert(rbt != NULL);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, ref));

    #ifdef POTTERY_RED_BLACK_TREE_PARENT
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            return POTTERY_RED_BLACK_TREE_PARENT((rbt->context), ref);
        #else
            (void)rbt;
            return POTTERY_RED_BLACK_TREE_PARENT(ref);
        #endif
    #else
        // If PARENT is not defined, the default is that ref is a pointer
        // to a struct in which the parent pointer is stored in a field
        // called "parent"
        (void)rbt;
        return ref->parent;
    #endif
}

#if 0
static inline
pottery_rbt_ref_t pottery_rbt_other_child(pottery_rbt_t* rbt, pottery_rbt_ref_t parent,
        pottery_rbt_ref_t child)
{
    pottery_assert(!pottery_rbt_ref_is_null(rbt, parent));
    pottery_assert(!pottery_rbt_ref_is_null(rbt, child));

    pottery_rbt_ref_t left_child = pottery_rbt_left_child(rbt, parent);
    pottery_rbt_ref_t right_child = pottery_rbt_left_child(rbt, parent);

    if (pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) child, left_child))
        return right_child;
    pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) child, right_child));
    return left_child;
}
#endif

static inline
bool pottery_rbt_is_red(pottery_rbt_t* rbt, pottery_rbt_ref_t ref) {
    pottery_assert(rbt != NULL);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, ref));

    #ifdef POTTERY_RED_BLACK_TREE_IS_RED
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            return POTTERY_RED_BLACK_TREE_IS_RED((rbt->context), ref);
        #else
            (void)rbt;
            return POTTERY_RED_BLACK_TREE_IS_RED(ref);
        #endif
    #else
        // If IS_RED is not defined, the default is that ref is a pointer to a
        // struct in which the red/black flag is stored in a field called
        // "is_red"
        (void)rbt;
        return ref->is_red;
    #endif
}

static inline
void pottery_rbt_set_left_child(pottery_rbt_t* rbt,
        pottery_rbt_ref_t ref, pottery_rbt_ref_t left_child)
{
            //printf("%i SET %p RIGHT_CHILD %p\n",__LINE__,(void*)ref,(void*)left_child);
    (void)rbt;
    pottery_assert(rbt != NULL);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, ref));

    #ifdef POTTERY_RED_BLACK_TREE_SET_LEFT_CHILD
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            POTTERY_RED_BLACK_TREE_SET_LEFT_CHILD((rbt->context), (ref), (left_child));
        #else
            POTTERY_RED_BLACK_TREE_SET_LEFT_CHILD((ref), (left_child));
        #endif
    #elif defined(POTTERY_RED_BLACK_TREE_LEFT_CHILD)
        // If SET_LEFT_CHILD is not defined but LEFT_CHILD is, it must be an l-value.
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            POTTERY_RED_BLACK_TREE_LEFT_CHILD((rbt->context), (ref)) = left_child;
        #else
            POTTERY_RED_BLACK_TREE_LEFT_CHILD((ref)) = left_child;
        #endif
    #else
        // Otherwise it's a "left_child" field.
        ref->left_child = left_child;
    #endif
}

static inline
void pottery_rbt_set_right_child(pottery_rbt_t* rbt,
        pottery_rbt_ref_t ref, pottery_rbt_ref_t right_child)
{
            //printf("%i SET %p RIGHT_CHILD %p\n",__LINE__,(void*)ref,(void*)right_child);
    pottery_assert(rbt != NULL);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, ref));

    #ifdef POTTERY_RED_BLACK_TREE_SET_RIGHT_CHILD
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            POTTERY_RED_BLACK_TREE_SET_RIGHT_CHILD((rbt->context), (ref), (right_child));
        #else
            (void)rbt;
            POTTERY_RED_BLACK_TREE_SET_RIGHT_CHILD((ref), (right_child));
        #endif
    #elif defined(POTTERY_RED_BLACK_TREE_RIGHT_CHILD)
        // If SET_RIGHT_CHILD is not defined but RIGHT_CHILD is, it must be an l-value.
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            POTTERY_RED_BLACK_TREE_RIGHT_CHILD((rbt->context), (ref)) = right_child;
        #else
            (void)rbt;
            POTTERY_RED_BLACK_TREE_RIGHT_CHILD((ref)) = right_child;
        #endif
    #else
        // Otherwise it's a "right_child" field.
        (void)rbt;
        ref->right_child = right_child;
    #endif
}

static inline
void pottery_rbt_set_parent(pottery_rbt_t* rbt,
        pottery_rbt_ref_t ref, pottery_rbt_ref_t parent)
{
            //printf("%i SET %p PARENT %p\n",__LINE__,(void*)ref,(void*)parent);
    pottery_assert(rbt != NULL);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, ref));

    #ifdef POTTERY_RED_BLACK_TREE_SET_PARENT
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            POTTERY_RED_BLACK_TREE_SET_PARENT((rbt->context), (ref), (parent));
        #else
            (void)rbt;
            POTTERY_RED_BLACK_TREE_SET_PARENT((ref), (parent));
        #endif
    #elif defined(POTTERY_RED_BLACK_TREE_PARENT)
        // If SET_PARENT is not defined but PARENT is, it must be an l-value.
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            POTTERY_RED_BLACK_TREE_PARENT((rbt->context), (ref)) = parent;
        #else
            (void)rbt;
            POTTERY_RED_BLACK_TREE_PARENT((ref)) = parent;
        #endif
    #else
        // Otherwise it's a "parent" field.
        (void)rbt;
        ref->parent = parent;
    #endif
}

static inline
void pottery_rbt_set_is_red(pottery_rbt_t* rbt, pottery_rbt_ref_t ref, bool is_red) {
    pottery_assert(rbt != NULL);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, ref));

    #ifdef POTTERY_RED_BLACK_TREE_SET_IS_RED
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            POTTERY_RED_BLACK_TREE_SET_IS_RED((rbt->context), (ref), (is_red));
        #else
            (void)rbt;
            POTTERY_RED_BLACK_TREE_SET_IS_RED((ref), (is_red));
        #endif
    #elif defined(POTTERY_RED_BLACK_TREE_IS_RED)
        // If SET_IS_RED is not defined but IS_RED is, it must be an l-value.
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
            POTTERY_RED_BLACK_TREE_IS_RED((rbt->context), (ref)) = is_red;
        #else
            (void)rbt;
            POTTERY_RED_BLACK_TREE_IS_RED((ref)) = is_red;
        #endif
    #else
        // Otherwise it's an "is_red" field.
        (void)rbt;
        ref->is_red = is_red;
    #endif
}



pottery_rbt_entry_t pottery_rbt_next(pottery_rbt_t* rbt, pottery_rbt_entry_t entry);

pottery_rbt_ref_t pottery_rbt_previous(pottery_rbt_t* rbt, pottery_rbt_entry_t entry);

pottery_rbt_entry_t pottery_rbt_begin(pottery_rbt_t* rbt);

static inline
pottery_rbt_entry_t pottery_rbt_end(pottery_rbt_t* rbt) {
    pottery_rbt_sanity_check(rbt);
    return pottery_rbt_null(rbt);
}

pottery_rbt_entry_t pottery_rbt_first(pottery_rbt_t* rbt);

pottery_rbt_entry_t pottery_rbt_last(pottery_rbt_t* rbt);



static inline
void pottery_rbt_init(pottery_rbt_t* rbt
        #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
        , pottery_rbt_context_t context
        #endif
) {
    pottery_assert(rbt != NULL);
    #if POTTERY_RED_BLACK_TREE_HAS_CONTEXT
    rbt->context = context;
    #endif
    rbt->root = pottery_rbt_null(rbt);
    rbt->count = 0;
    pottery_rbt_sanity_check(rbt);
}

static inline
void pottery_rbt_unlink_all(pottery_rbt_t* rbt) {
    pottery_assert(rbt != NULL);
    rbt->root = pottery_rbt_null(rbt);
    rbt->count = 0;
    pottery_rbt_sanity_check(rbt);
}

#if POTTERY_FORWARD_DECLARATIONS
POTTERY_RED_BLACK_TREE_EXTERN
void pottery_rbt_swap(pottery_rbt_t* left, pottery_rbt_t* right);

/**
 * Links a new entry into the tree.
 *
 * If an entry already exists with a matching key, it is unlinked and returned;
 * otherwise a null ref is returned.
 */
POTTERY_RED_BLACK_TREE_EXTERN
pottery_rbt_entry_t pottery_rbt_link(pottery_rbt_t* rbt, pottery_rbt_entry_t entry);

POTTERY_RED_BLACK_TREE_EXTERN
void pottery_rbt_unlink(pottery_rbt_t* rbt, pottery_rbt_entry_t entry);

/**
 * Replaces a node in the tree.
 *
 * The replacement node must have the same key that the current node had.
 * Neither key will be accessed during this call; you can move (relocate) the
 * key and other node data from the old node to the new one before or after
 * this call.
 *
 * @param current A node currently in the tree
 * @param replacement A node not in the tree that should replace current
 */
POTTERY_RED_BLACK_TREE_EXTERN
void pottery_rbt_replace(pottery_rbt_t* rbt,
        pottery_rbt_entry_t current, pottery_rbt_entry_t replacement);

POTTERY_RED_BLACK_TREE_EXTERN
pottery_rbt_entry_t pottery_rbt_impl_find_location(pottery_rbt_t* rbt,
        pottery_rbt_key_t key, pottery_rbt_location_t* /*nullable*/ out_location);
#endif

static pottery_always_inline
pottery_rbt_entry_t pottery_rbt_find(pottery_rbt_t* rbt, pottery_rbt_key_t key) {
    return pottery_rbt_impl_find_location(rbt, key, NULL);
}

/**
 * Finds an entry with the given key or the location where such an entry could
 * be inserted.
 *
 * If the given key exists in the tree, its entry is returned. Otherwise, a
 * non-existent entry is returned and the location where such an entry could be
 * inserted is stored in the given location struct.
 *
 * This exists as an optimization of find() followed by link(), which would
 * necessitate two tree traversals. Instead you can do a lookup with
 * find_location() and if the entry is not found, you can allocate a node and
 * insert it with link_location() without traversing the tree a second time.
 *
 * @see pottery_rbt_link_location
 */
static pottery_always_inline
pottery_rbt_entry_t pottery_rbt_find_location(pottery_rbt_t* rbt,
        pottery_rbt_key_t key, pottery_rbt_location_t* out_location)
{
    pottery_assert(out_location != NULL);
    return pottery_rbt_impl_find_location(rbt, key, out_location);
}

#if POTTERY_FORWARD_DECLARATIONS
/**
 * Commits an entry emplace operation.
 *
 * You can only call this immediately after a call to find_location() that
 * returned a non-existent entry.
 *
 * @warning You can only call this if you have not called any non-const
 *          functions on the tree in since calling find_location()! Any
 *          mutating functions on the tree may change its structure which would
 *          cause link_location() to corrupt the tree.
 *
 * @see pottery_rbt_find_location()
 */
POTTERY_RED_BLACK_TREE_EXTERN
void pottery_rbt_link_location(pottery_rbt_t* rbt, pottery_rbt_entry_t entry,
        pottery_rbt_location_t* location);
#endif

static inline
pottery_rbt_entry_t pottery_rbt_any(pottery_rbt_t* rbt) {
    pottery_rbt_sanity_check(rbt);
    return rbt->root;
}

static inline
void pottery_rbt_destroy(pottery_rbt_t* rbt) {
    pottery_assert(rbt != NULL);

    // It is an error to destroy a non-empty list. The list contents must be
    // properly disposed of.
    pottery_assert(rbt->count == 0);
    pottery_rbt_sanity_check(rbt);
}

static inline
size_t pottery_rbt_count(pottery_rbt_t* rbt) {
    pottery_rbt_sanity_check(rbt);
    return rbt->count;
}

static inline
bool pottery_rbt_is_empty(pottery_rbt_t* rbt) {
    pottery_rbt_sanity_check(rbt);
    return rbt->count == 0;
}
