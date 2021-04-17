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

#define pottery_log(...) /*nothing*/


// Sanity check involves a complete scan of the entire tree, making it way too
// slow. It's disabled even in debug builds. This is really only useful for
// development purposes.
#if 0&&POTTERY_DEBUG
static
void pottery_rbt_sanity_check_node(pottery_rbt_t* rbt, pottery_rbt_ref_t ref,
        size_t black_height)
{
    pottery_assert(!pottery_rbt_ref_is_null(rbt, ref));

    bool is_red = pottery_rbt_is_red(rbt, ref);
    if (!is_red) {
        pottery_assert(black_height > 0);
        --black_height;
    }

    pottery_rbt_ref_t left_child = pottery_rbt_left_child(rbt, ref);
            //pottery_log("%i %p left_child %p parent %p\n",__LINE__,(void*)ref,(void*)left_child, left_child==NULL?NULL:((void*)pottery_rbt_parent(rbt, left_child)));
    if (pottery_rbt_ref_is_null(rbt, left_child)) {
        pottery_assert(black_height == 0);
    } else {
        pottery_assert(!is_red || !pottery_rbt_is_red(rbt, left_child));
        pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) ref,
                    pottery_rbt_parent(rbt, left_child)));
        pottery_rbt_sanity_check_node(rbt, left_child, black_height);
    }

    pottery_rbt_ref_t right_child = pottery_rbt_right_child(rbt, ref);
            //pottery_log("%i %p right_child %p parent %p\n",__LINE__,(void*)ref,(void*)right_child, right_child==NULL?NULL:((void*)pottery_rbt_parent(rbt, right_child)));
    if (pottery_rbt_ref_is_null(rbt, right_child)) {
        pottery_assert(black_height == 0);
    } else {
        pottery_assert(!is_red || !pottery_rbt_is_red(rbt, right_child));
        pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) ref,
                    pottery_rbt_parent(rbt, right_child)));
        pottery_rbt_sanity_check_node(rbt, right_child, black_height);
    }
}

POTTERY_RED_BLACK_TREE_EXTERN
void pottery_rbt_sanity_check(pottery_rbt_t* rbt) {
    // root should be null if and only if the tree is empty.
    pottery_assert((rbt->count == 0) == pottery_rbt_ref_is_null(rbt, rbt->root));
    if (rbt->count == 0)
        return;

    // walk to the left-most leaf counting black nodes
    size_t black_height = 0;
    pottery_rbt_ref_t parent = pottery_rbt_null(rbt);
    pottery_rbt_ref_t ref = rbt->root;
    while (!pottery_rbt_ref_is_null(rbt, ref)) {
        // check parent refs to make sure there isn't a cycle
        pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) parent,
                    pottery_rbt_parent(rbt, ref)));
        if (!pottery_rbt_is_red(rbt, ref))
            ++black_height;
        parent = ref;
        ref = pottery_rbt_left_child(rbt, ref);
    }

            //pottery_log("%i =========== %p PARENT %p\n",__LINE__,(void*)rbt->root,(void*)pottery_rbt_parent(rbt, rbt->root));
    pottery_rbt_sanity_check_node(rbt, rbt->root, black_height);
}
#endif

static
pottery_rbt_entry_t pottery_rbt_leftmost_leaf(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    pottery_rbt_sanity_check(rbt);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, entry));
    while (true) {
        pottery_rbt_entry_t next = pottery_rbt_left_child(rbt, entry);
        if (pottery_rbt_ref_is_null(rbt, next))
            break;
        entry = next;
    }
    return entry;
}

static
pottery_rbt_entry_t pottery_rbt_rightmost_leaf(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    pottery_rbt_sanity_check(rbt);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, entry));
    while (true) {
        pottery_rbt_entry_t next = pottery_rbt_right_child(rbt, entry);
        if (pottery_rbt_ref_is_null(rbt, next))
            break;
        entry = next;
    }
    return entry;
}

pottery_rbt_entry_t pottery_rbt_next(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    pottery_rbt_sanity_check(rbt);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, entry));

    // If we have a right child, we return its leftmost leaf.
    pottery_rbt_entry_t right_child = pottery_rbt_right_child(rbt, entry);
    if (!pottery_rbt_ref_is_null(rbt, right_child))
        return pottery_rbt_leftmost_leaf(rbt, right_child);

    // Otherwise we walk up until we are the left child of a parent.
    while (true) {
        pottery_rbt_entry_t parent = pottery_rbt_parent(rbt, entry);
        if (pottery_rbt_ref_is_null(rbt, parent))
            break;

        if (pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry,
                    pottery_rbt_left_child(rbt, parent)))
            return parent;

        entry = parent;
    }

    // If we haven't found anything, we are the last entry in the list.
    return pottery_rbt_null(rbt);
}

pottery_rbt_ref_t pottery_rbt_previous(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    pottery_rbt_sanity_check(rbt);

    // If we are the end entry, we return the last entry, which must exist.
    if (pottery_rbt_ref_is_null(rbt, entry))
        return pottery_rbt_rightmost_leaf(rbt, rbt->root);

    // If we have a left child, we return its rightmost leaf.
    pottery_rbt_entry_t left_child = pottery_rbt_left_child(rbt, entry);
    if (!pottery_rbt_ref_is_null(rbt, left_child))
        return pottery_rbt_rightmost_leaf(rbt, left_child);

    // Otherwise we walk up until we are the right child of a parent.
    while (true) {
        pottery_rbt_entry_t parent = pottery_rbt_parent(rbt, entry);

        // Calling previous on the first entry is not allowed so there must
        // always be a left entry. Since we've checked that it's not a child
        // above, it must be a parent.
        pottery_assert(!pottery_rbt_ref_is_null(rbt, parent));

        if (pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry,
                    pottery_rbt_right_child(rbt, parent)))
            return parent;

        entry = parent;
    }

    pottery_unreachable();
}

// TODO it would be nice to inline the below, we have to expose our
// leftmost/rightmost functions

pottery_rbt_entry_t pottery_rbt_begin(pottery_rbt_t* rbt) {
    pottery_rbt_sanity_check(rbt);
    if (pottery_rbt_ref_is_null(rbt, rbt->root))
        return pottery_rbt_null(rbt);
    return pottery_rbt_leftmost_leaf(rbt, rbt->root);
}

pottery_rbt_entry_t pottery_rbt_first(pottery_rbt_t* rbt) {
    pottery_rbt_sanity_check(rbt);
    // The tree cannot be empty. The below will assert if it is.
    return pottery_rbt_leftmost_leaf(rbt, rbt->root);
}

pottery_rbt_entry_t pottery_rbt_last(pottery_rbt_t* rbt) {
    pottery_rbt_sanity_check(rbt);
    // The tree cannot be empty. The below will assert if it is.
    return pottery_rbt_rightmost_leaf(rbt, rbt->root);
}

POTTERY_RED_BLACK_TREE_EXTERN
void pottery_rbt_swap(pottery_rbt_t* left, pottery_rbt_t* right) {
    pottery_rbt_sanity_check(left);
    pottery_rbt_sanity_check(right);

    pottery_rbt_t temp = *left;
    *left = *right;
    *right = temp;
}

POTTERY_RED_BLACK_TREE_EXTERN
void pottery_rbt_replace(pottery_rbt_t* rbt,
        pottery_rbt_entry_t current, pottery_rbt_entry_t entry)
{
            //pottery_log("%i\n",__LINE__);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, current));
    pottery_assert(!pottery_rbt_ref_is_null(rbt, entry));

    // replace color
    pottery_rbt_set_is_red(rbt, entry, pottery_rbt_is_red(rbt, current));

    // replace parent
    pottery_rbt_entry_t parent = pottery_rbt_parent(rbt, current);
            //pottery_log("%i\n",__LINE__);
    pottery_rbt_set_parent(rbt, entry, parent);
            //pottery_log("%i SET %p PARENT TO %p\n",__LINE__,(void*)entry,(void*)parent);
            //pottery_log("%i %p PARENT %p\n",__LINE__,(void*)entry,(void*)pottery_rbt_parent(rbt, entry));
    if (!pottery_rbt_ref_is_null(rbt, parent)) {
            //pottery_log("%i\n",__LINE__);
        if (pottery_rbt_left_child(rbt, parent) == current) {
            pottery_rbt_set_left_child(rbt, parent, entry);
        } else {
            pottery_assert(pottery_rbt_right_child(rbt, parent) == current);
            pottery_rbt_set_right_child(rbt, parent, entry);
        }
    }

    // replace left child
    pottery_rbt_entry_t left_child = pottery_rbt_left_child(rbt, current);
    pottery_rbt_set_left_child(rbt, entry, left_child);
    if (!pottery_rbt_ref_is_null(rbt, left_child))
        pottery_rbt_set_parent(rbt, left_child, entry);

    // replace right child
    pottery_rbt_entry_t right_child = pottery_rbt_right_child(rbt, current);
    pottery_rbt_set_right_child(rbt, entry, right_child);
    if (!pottery_rbt_ref_is_null(rbt, right_child))
        pottery_rbt_set_parent(rbt, right_child, entry);

    // replace root
    if (rbt->root == current) {
            //pottery_log("%i\n",__LINE__);
        rbt->root = entry;
    }
            //pottery_log("%i %p PARENT %p\n",__LINE__,(void*)entry,(void*)pottery_rbt_parent(rbt, entry));
}

/**
 * Walks down the tree looking for a node with the given key or a place to
 * insert it.
 *
 * If a node with the given key is found, it is returned.
 *
 * Otherwise the parent where the new node should be inserted is placed in
 * location. The location's compare is set to less than zero if it should be
 * the left child and greater than zero if it should be the right child. The
 * corresponding child ref is null, and null is returned.
 */
pottery_rbt_entry_t pottery_rbt_impl_find_location(pottery_rbt_t* rbt,
        pottery_rbt_key_t key, pottery_rbt_location_t* /*nullable*/ location)
{
    if (pottery_rbt_is_empty(rbt)) {
        // The tree is empty. Store null in location and return null.
        if (location != pottery_null)
            location->parent = pottery_rbt_null(rbt);
        return pottery_rbt_null(rbt);
    }

    pottery_rbt_entry_t current = rbt->root;
    while (true) {
        pottery_rbt_key_t current_key = pottery_rbt_ref_key(POTTERY_RBT_CONTEXT_VAL(rbt) current);

        #if POTTERY_COMPARE_HAS_USER_THREE_WAY
        // We do a three-way comparison only if the user has defined it to
        // avoid unnecessary two-way comparisons.
        int three_way = pottery_rbt_compare_three_way(POTTERY_RBT_CONTEXT_VAL(rbt) key, current_key);
        bool equal = three_way == 0;
        #else
        bool equal = pottery_rbt_compare_equal(POTTERY_RBT_CONTEXT_VAL(rbt) key, current_key);
        #endif

        if (equal) {
            // We've found an entry with matching key. Just return it; location
            // won't be used.
            #if POTTERY_DEBUG
            if (location != pottery_null) {
                // We put in some bogus values in the location struct that will
                // cause an assertion in case it is actually used.
                location->parent = pottery_null;
                location->compare = 0;
                // TODO this would be a good candidate for valgrind mark uninitialized
            }
            #endif
            return current;
        }

        #if POTTERY_COMPARE_HAS_USER_THREE_WAY
        bool less = three_way < 0;
        #else
        bool less = pottery_rbt_compare_less(POTTERY_RBT_CONTEXT_VAL(rbt) key, current_key);
        #endif

        pottery_rbt_entry_t next = less ?
            pottery_rbt_left_child(rbt, current) :
            pottery_rbt_right_child(rbt, current);

        if (pottery_rbt_ref_is_null(rbt, next)) {
            // We've hit a leaf, meaning our key is not in the tree. Store the
            // parent and side in location and return null.
            if (location != pottery_null) {
                location->parent = current;
                #if POTTERY_COMPARE_HAS_USER_THREE_WAY
                location->compare = three_way;
                #else
                location->compare = less ? -1 : 1;
                #endif
            }
            return pottery_rbt_null(rbt);
        }

        // Otherwise we walk down and loop around.
        current = next;
    }

    pottery_unreachable();
}

#if 0
static inline
void pottery_rbt_link_root(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    pottery_assert(pottery_rbt_ref_is_null(rbt, rbt->root));

    rbt->root = entry;
    pottery_rbt_set_parent(rbt, entry, pottery_rbt_null(rbt));
    pottery_rbt_set_left_child(rbt, entry, pottery_rbt_null(rbt));
    pottery_rbt_set_right_child(rbt, entry, pottery_rbt_null(rbt));
    pottery_rbt_set_is_red(rbt, entry, false);

    pottery_rbt_sanity_check(rbt);
}
#endif

// Rotate left: entry is replaced by its right child
static
void pottery_rbt_rotate_left(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    pottery_rbt_entry_t other = pottery_rbt_right_child(rbt, entry);
    pottery_rbt_entry_t parent = pottery_rbt_parent(rbt, entry);
    pottery_rbt_entry_t middle = pottery_rbt_left_child(rbt, other);

    // move middle child over
    pottery_rbt_set_right_child(rbt, entry, middle);
    if (!pottery_rbt_ref_is_null(rbt, middle))
        pottery_rbt_set_parent(rbt, middle, entry);

    // make entry child of other
    pottery_rbt_set_left_child(rbt, other, entry);
    pottery_rbt_set_parent(rbt, entry, other);

    // fixup ref to parent
    pottery_rbt_set_parent(rbt, other, parent);

    // fixup ref from parent or root
    if (pottery_rbt_ref_is_null(rbt, parent)) {
        pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry, rbt->root));
        rbt->root = other;
    } else {
        pottery_assert(!pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry, rbt->root));

        pottery_rbt_entry_t parent_left_child = pottery_rbt_left_child(rbt, parent);
        if (pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry, parent_left_child)) {
            pottery_rbt_set_left_child(rbt, parent, other);
        } else {
            pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry,
                       pottery_rbt_right_child(rbt, parent)));
            pottery_rbt_set_right_child(rbt, parent, other);
        }
    }
}

// Rotate right: entry is replaced by its left child
static
void pottery_rbt_rotate_right(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    pottery_rbt_entry_t other = pottery_rbt_left_child(rbt, entry);
    pottery_rbt_entry_t parent = pottery_rbt_parent(rbt, entry);
    pottery_rbt_entry_t middle = pottery_rbt_right_child(rbt, other);

    // move middle child over
    pottery_rbt_set_left_child(rbt, entry, middle);
    if (!pottery_rbt_ref_is_null(rbt, middle))
        pottery_rbt_set_parent(rbt, middle, entry);

    // make entry child of other
    pottery_rbt_set_right_child(rbt, other, entry);
    pottery_rbt_set_parent(rbt, entry, other);

    // fixup ref to parent
    pottery_rbt_set_parent(rbt, other, parent);

    // fixup ref from parent or root
    if (pottery_rbt_ref_is_null(rbt, parent)) {
        pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry, rbt->root));
        rbt->root = other;
    } else {
        pottery_assert(!pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry, rbt->root));

        pottery_rbt_entry_t parent_right_child = pottery_rbt_right_child(rbt, parent);
        if (pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry, parent_right_child)) {
            pottery_rbt_set_right_child(rbt, parent, other);
        } else {
            pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry,
                       pottery_rbt_left_child(rbt, parent)));
            pottery_rbt_set_left_child(rbt, parent, other);
        }
    }
}

static
void pottery_rbt_link_rebalance(pottery_rbt_t* rbt,
        pottery_rbt_entry_t parent, pottery_rbt_entry_t entry)
{
    pottery_log("%i rebalance start\n",__LINE__);
    while (true)  {
        pottery_log("%i --- top of rebalance loop\n",__LINE__);
        pottery_assert(!pottery_rbt_ref_is_null(rbt, parent));
        pottery_assert(!pottery_rbt_ref_is_null(rbt, entry));
        pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
                parent, pottery_rbt_parent(rbt, entry)));
        if (!pottery_rbt_is_red(rbt, parent)) {
            pottery_log("%i parent is black. done\n",__LINE__);
            break;
        }

        // both node and parent are red.

        pottery_rbt_entry_t grandparent = pottery_rbt_parent(rbt, parent);
        if (pottery_rbt_ref_is_null(rbt, grandparent)) {
            // parent is the root. just make it black.
            pottery_log("%i parent is root. making it black, done.\n",__LINE__);
            pottery_rbt_set_is_red(rbt, parent, false);
            break;
        }

        // grandparent must already be black.
        // collect its children and figure out which side parent is on
        pottery_assert(!pottery_rbt_is_red(rbt, grandparent));
        pottery_rbt_entry_t grandparent_left_child = pottery_rbt_left_child(rbt, grandparent);
        pottery_rbt_entry_t grandparent_right_child = pottery_rbt_right_child(rbt, grandparent);
        bool parent_is_left = pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
                parent, grandparent_left_child);
        pottery_assert(parent_is_left || pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
                parent, grandparent_right_child));
        pottery_rbt_entry_t uncle = parent_is_left ? grandparent_right_child :
                grandparent_left_child;

        if (!pottery_rbt_ref_is_null(rbt, uncle) && pottery_rbt_is_red(rbt, uncle)) {
            // parent and uncle are both red. make them both black and make
            // grandparent red.
            pottery_log("%i parent and uncle are both red. "
                    "making them black and grandparent red.\n",__LINE__);
            pottery_rbt_set_is_red(rbt, parent, false);
            if (!pottery_rbt_ref_is_null(rbt, uncle))
                pottery_rbt_set_is_red(rbt, uncle, false);
            pottery_rbt_set_is_red(rbt, grandparent, true);

            // grandparent is now red, and great-grandparent might also be red.
            // loop around if necessary.
            parent = pottery_rbt_parent(rbt, grandparent);
            if (pottery_rbt_ref_is_null(rbt, parent)) {
                pottery_log("%i no great grand-parent, done.\n",__LINE__);
                break;
            }
            entry = grandparent;
            pottery_log("%i great-grandparent exists. looping around on grandparent.\n",__LINE__);
            continue;
        }

        // uncle is also black (or doesn't exist.) figure out whether entry is
        // an inner child of parent.
        pottery_rbt_entry_t parent_left_child = pottery_rbt_left_child(rbt, parent);
        bool entry_is_left = pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
                entry, parent_left_child);
        pottery_assert(entry_is_left || pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
                entry, pottery_rbt_right_child(rbt, parent)));

        if (entry_is_left != parent_is_left) {
            pottery_log("%i entry is an inner %s child, rotating parent %s.\n",__LINE__, entry_is_left ? "left" : "right", entry_is_left ? "right" : "left");
            // entry is an inner child. rotate parent
            if (entry_is_left)
                pottery_rbt_rotate_right(rbt, parent);
            else
                pottery_rbt_rotate_left(rbt, parent);
            // swap them
            pottery_rbt_entry_t temp = parent;
            parent = entry;
            entry = temp;
        }

        // entry is now an outer child. rotate grandparent and swap colors.
        pottery_log("%i rotating grandparent %s, done.\n",__LINE__, parent_is_left ? "right" : "left");
        if (parent_is_left)
            pottery_rbt_rotate_right(rbt, grandparent);
        else
            pottery_rbt_rotate_left(rbt, grandparent);
        pottery_rbt_set_is_red(rbt, grandparent, true);
        pottery_rbt_set_is_red(rbt, parent, false);
        break;
    }

    pottery_rbt_sanity_check(rbt);
}

POTTERY_RED_BLACK_TREE_EXTERN
void pottery_rbt_link_location(pottery_rbt_t* rbt, pottery_rbt_entry_t entry,
        pottery_rbt_location_t* location)
{
    pottery_rbt_set_left_child(rbt, entry, pottery_rbt_null(rbt));
    pottery_rbt_set_right_child(rbt, entry, pottery_rbt_null(rbt));
    pottery_rbt_set_is_red(rbt, entry, true);
    ++rbt->count;

    if (pottery_rbt_ref_is_null(rbt, location->parent)) {
        pottery_assert(rbt->count == 1);
        pottery_assert(pottery_rbt_ref_is_null(rbt, rbt->root));
        pottery_rbt_set_parent(rbt, entry, pottery_rbt_null(rbt));
        rbt->root = entry;
    } else {
        pottery_rbt_set_parent(rbt, entry, location->parent);
        if (location->compare < 0) {
            pottery_rbt_set_left_child(rbt, location->parent, entry);
        } else {
            pottery_assert(location->compare > 0);
            pottery_rbt_set_right_child(rbt, location->parent, entry);
        }
        pottery_rbt_link_rebalance(rbt, location->parent, entry);
    }

    pottery_rbt_sanity_check(rbt);
}

#if 1
POTTERY_RED_BLACK_TREE_EXTERN
pottery_rbt_entry_t pottery_rbt_link(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    pottery_rbt_location_t location;
    pottery_rbt_entry_t previous = pottery_rbt_impl_find_location(rbt,
            pottery_rbt_ref_key(POTTERY_RBT_CONTEXT_VAL(rbt) entry), &location);
    if (pottery_rbt_ref_is_null(rbt, previous)) {
        pottery_rbt_link_location(rbt, entry, &location);
    } else {
        pottery_rbt_replace(rbt, previous, entry);
    }
    return previous;
}
#else
POTTERY_RED_BLACK_TREE_EXTERN
pottery_rbt_entry_t pottery_rbt_link(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    pottery_log("=================\n===================\n==============\n%i link\n",__LINE__);

    // TODO call walk
    pottery_rbt_sanity_check(rbt);

    if (pottery_rbt_ref_is_null(rbt, rbt->root)) {
        pottery_log("%i linking new root\n",__LINE__);
        pottery_assert(rbt->count == 0);
        ++rbt->count;
        pottery_rbt_link_root(rbt, entry);
        return pottery_rbt_null(rbt);
    }

    pottery_rbt_key_t entry_key = pottery_rbt_ref_key(POTTERY_RBT_CONTEXT_VAL(rbt) entry);

    pottery_rbt_entry_t current = rbt->root;
    while (true) {
        pottery_rbt_key_t current_key = pottery_rbt_ref_key(POTTERY_RBT_CONTEXT_VAL(rbt) current);
        #if POTTERY_COMPARE_HAS_USER_THREE_WAY
        // We do a three-way comparison only if the user has defined it to
        // avoid unnecessary two-way comparisons.
        int three_way = pottery_rbt_compare_three_way(POTTERY_RBT_CONTEXT_VAL(rbt) entry_key, current_key);
        bool equal = three_way == 0;
        #else
        bool equal = pottery_rbt_compare_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry_key, current_key);
        #endif

        if (equal) {
            // We've found an entry with matching key. Replace the existing
            // entry with the new one and return the old entry. No need to
            // balance because the tree structure is the same.
            pottery_rbt_replace_node(rbt, current, entry);
            return current;
        }

        #if POTTERY_COMPARE_HAS_USER_THREE_WAY
        bool less = three_way < 0;
        #else
        bool less = pottery_rbt_compare_less(POTTERY_RBT_CONTEXT_VAL(rbt) entry_key, current_key);
        #endif

        pottery_rbt_entry_t next = less ?
            pottery_rbt_left_child(rbt, current) :
            pottery_rbt_right_child(rbt, current);

        if (pottery_rbt_ref_is_null(rbt, next)) {
            // We've found a leaf to insert ourselves into.
            if (less)
                pottery_rbt_set_left_child(rbt, current, entry);
            else
                pottery_rbt_set_right_child(rbt, current, entry);
            pottery_rbt_set_parent(rbt, entry, current);
            pottery_rbt_set_left_child(rbt, entry, pottery_rbt_null(rbt));
            pottery_rbt_set_right_child(rbt, entry, pottery_rbt_null(rbt));
            pottery_rbt_set_is_red(rbt, entry, true);
            ++rbt->count;
            pottery_rbt_link_rebalance(rbt, current, entry);
            return pottery_rbt_null(rbt);
        }

        // Otherwise we walk down and loop around.
        current = next;
    }

    pottery_unreachable();
}
#endif

POTTERY_RED_BLACK_TREE_EXTERN
void pottery_rbt_unlink(pottery_rbt_t* rbt, pottery_rbt_entry_t entry) {
    pottery_log("=================\n===================\n==============\n%i unlink\n",__LINE__);
    pottery_rbt_sanity_check(rbt);
    pottery_assert(!pottery_rbt_ref_is_null(rbt, entry));

    --rbt->count;

    pottery_rbt_entry_t parent = pottery_rbt_parent(rbt, entry);
    pottery_rbt_entry_t left_child = pottery_rbt_left_child(rbt, entry);
    pottery_rbt_entry_t right_child = pottery_rbt_right_child(rbt, entry);

            //pottery_log("%i\n",__LINE__);

    // If this node has two children, swap it with its predecessor so that it
    // has at most one child.
    if (!pottery_rbt_ref_is_null(rbt, left_child) && !pottery_rbt_ref_is_null(rbt, right_child)) {
        pottery_log("%i node has two children, swapping with previous\n",__LINE__);
        pottery_rbt_entry_t replacement = pottery_rbt_previous(rbt, entry);

        // First collect all refs
        pottery_rbt_entry_t e_parent = parent;
        pottery_rbt_entry_t r_parent = pottery_rbt_parent(rbt, replacement);
        pottery_rbt_entry_t e_left_child = left_child;
        pottery_rbt_entry_t r_left_child = pottery_rbt_left_child(rbt, replacement);
        pottery_rbt_entry_t e_right_child = right_child;

        // Some sanity checks
        pottery_assert(pottery_rbt_ref_is_null(rbt, pottery_rbt_right_child(rbt, replacement)));
        pottery_assert(!pottery_rbt_ref_is_null(rbt, r_parent));
        pottery_assert(!pottery_rbt_ref_is_null(rbt, e_left_child));
        pottery_assert(!pottery_rbt_ref_is_null(rbt, e_right_child));

        if (pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) e_left_child, replacement)) {
            // The replacement is the direct child of the entry, so make
            // them point at each other.
            pottery_rbt_set_left_child(rbt, replacement, entry);
            pottery_rbt_set_parent(rbt, entry, replacement);
            parent = replacement;

        } else {
            // There are nodes in between, so we need to fix up their
            // references as well.

            pottery_rbt_set_left_child(rbt, replacement, e_left_child);
            pottery_rbt_set_parent(rbt, e_left_child, replacement);

            pottery_rbt_set_parent(rbt, entry, r_parent);
            if (pottery_rbt_left_child(rbt, r_parent) == replacement) {
                pottery_rbt_set_left_child(rbt, r_parent, entry);
            } else {
                pottery_assert(pottery_rbt_right_child(rbt, r_parent) == replacement);
                pottery_rbt_set_right_child(rbt, r_parent, entry);
            }

            parent = r_parent;
        }

        // Set entry parent's child link to replacement
        if (pottery_rbt_ref_is_null(rbt, e_parent)) {
            pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) rbt->root, entry));
            rbt->root = replacement;
        } else {
            if (pottery_rbt_left_child(rbt, e_parent) == entry) {
                pottery_rbt_set_left_child(rbt, e_parent, replacement);
            } else {
                pottery_assert(pottery_rbt_right_child(rbt, e_parent) == entry);
                pottery_rbt_set_right_child(rbt, e_parent, replacement);
            }
        }
        pottery_rbt_set_parent(rbt, replacement, e_parent);

        // Set other links

        pottery_rbt_set_left_child(rbt, entry, r_left_child);
        if (!pottery_rbt_ref_is_null(rbt, r_left_child))
            pottery_rbt_set_parent(rbt, r_left_child, entry);

        pottery_rbt_set_right_child(rbt, replacement, e_right_child);
        if (!pottery_rbt_ref_is_null(rbt, e_right_child))
            pottery_rbt_set_parent(rbt, e_right_child, replacement);

        pottery_rbt_set_right_child(rbt, entry, pottery_rbt_null(rbt));

        // Swap colors
        bool temp = pottery_rbt_is_red(rbt, entry);
        pottery_rbt_set_is_red(rbt, entry, pottery_rbt_is_red(rbt, replacement));
        pottery_rbt_set_is_red(rbt, replacement, temp);

        left_child = r_left_child;
        right_child = pottery_rbt_null(rbt);
        //pottery_log("%i\n",__LINE__);

    } else if (pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry, rbt->root)) {
        pottery_log("%i node is root and has at most one child. replacing with child, done.\n",__LINE__);
        rbt->root = pottery_rbt_ref_is_null(rbt, right_child) ? left_child : right_child;
        if (!pottery_rbt_ref_is_null(rbt, rbt->root)) {
            pottery_rbt_set_parent(rbt, rbt->root, pottery_rbt_null(rbt));
        } else {
            pottery_assert(rbt->count == 0);
        }
        pottery_rbt_sanity_check(rbt);
        return;
    }

    pottery_log("%i node is not root and has at most one child.\n",__LINE__);

    pottery_rbt_ref_t parent_left_child = pottery_rbt_left_child(rbt, parent);
    bool entry_is_left = pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
            entry, parent_left_child);
    pottery_assert(entry_is_left || pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
            entry, pottery_rbt_right_child(rbt, parent)));

    bool is_red = pottery_rbt_is_red(rbt, entry);
    if (is_red) {
        // The node is red and does not have two children so it cannot have any.
        pottery_log("%i node is red with no children. removing, done.\n",__LINE__);
        pottery_assert(pottery_rbt_ref_is_null(rbt, left_child));
        pottery_assert(pottery_rbt_ref_is_null(rbt, right_child));

        if (entry_is_left)
            pottery_rbt_set_left_child(rbt, parent, pottery_rbt_null(rbt));
        else
            pottery_rbt_set_right_child(rbt, parent, pottery_rbt_null(rbt));

        pottery_rbt_sanity_check(rbt);
        return;
    }

    {
        pottery_rbt_ref_t child = pottery_rbt_ref_is_null(rbt, right_child) ? left_child : right_child;
        if (!pottery_rbt_ref_is_null(rbt, child)) {
            // The node is black and has a single child, so it must be red. It
            // can simply be replaced by this child, making it black.
            pottery_log("%i node is black and has one (red) child. "
                    "replacing and making black, done.\n",__LINE__);

            pottery_assert(pottery_rbt_is_red(rbt, child));
            pottery_rbt_set_is_red(rbt, child, false);

            pottery_rbt_set_parent(rbt, child, parent);
            if (entry_is_left)
                pottery_rbt_set_left_child(rbt, parent, child);
            else
                pottery_rbt_set_right_child(rbt, parent, child);

            pottery_rbt_sanity_check(rbt);
            return;
        }
    }

    pottery_log("%i node is black, not root, with no children. removing and rebalancing\n",__LINE__);

    if (pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry, parent_left_child)) {
    //pottery_log("%i\n",__LINE__);
        pottery_rbt_set_left_child(rbt, parent, pottery_rbt_null(rbt));
    } else {
        pottery_assert(pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) entry, pottery_rbt_right_child(rbt, parent)));
        pottery_rbt_set_right_child(rbt, parent, pottery_rbt_null(rbt));
    }
    entry = pottery_rbt_null(rbt);

    while (true) {
        pottery_log("%i --- top of rebalance loop\n",__LINE__);

        // entry is black so sibling cannot be null
        pottery_rbt_entry_t sibling = entry_is_left ? pottery_rbt_right_child(rbt, parent) :
                parent_left_child;
        pottery_assert(!pottery_rbt_ref_is_null(rbt, sibling));
        bool sibling_is_red = pottery_rbt_is_red(rbt, sibling);

        // collect nephews
        pottery_rbt_entry_t sibling_left_child = pottery_rbt_left_child(rbt, sibling);
        pottery_rbt_entry_t sibling_right_child = pottery_rbt_right_child(rbt, sibling);
        pottery_rbt_entry_t close_nephew = entry_is_left ? sibling_left_child : sibling_right_child;
        pottery_rbt_entry_t far_nephew = entry_is_left ? sibling_right_child : sibling_left_child;
        bool close_nephew_is_red = !pottery_rbt_ref_is_null(rbt, close_nephew) &&
                pottery_rbt_is_red(rbt, close_nephew);
        bool far_nephew_is_red = !pottery_rbt_ref_is_null(rbt, far_nephew) &&
                pottery_rbt_is_red(rbt, far_nephew);

        if (sibling_is_red) {
            pottery_assert(!close_nephew_is_red);
            pottery_assert(!far_nephew_is_red);
            pottery_log("%i sibling is red. rotating parent %s\n",__LINE__, entry_is_left ? "left" : "right");
            if (entry_is_left)
                pottery_rbt_rotate_left(rbt, parent);
            else
                pottery_rbt_rotate_right(rbt, parent);
            pottery_rbt_set_is_red(rbt, sibling, false);
            pottery_rbt_set_is_red(rbt, parent, true);
            sibling = close_nephew;
            sibling_is_red = false;

            // TODO copy paste...
            sibling_left_child = pottery_rbt_left_child(rbt, sibling);
            sibling_right_child = pottery_rbt_right_child(rbt, sibling);
            close_nephew = entry_is_left ? sibling_left_child : sibling_right_child;
            far_nephew = entry_is_left ? sibling_right_child : sibling_left_child;
            close_nephew_is_red = !pottery_rbt_ref_is_null(rbt, close_nephew) &&
                    pottery_rbt_is_red(rbt, close_nephew);
            far_nephew_is_red = !pottery_rbt_ref_is_null(rbt, far_nephew) &&
                    pottery_rbt_is_red(rbt, far_nephew);
        }
        pottery_log("%i sibling is black.\n",__LINE__);

        bool parent_is_red = pottery_rbt_is_red(rbt, parent);
        if (!close_nephew_is_red && !far_nephew_is_red) {
            pottery_log("%i both nephews are black.\n",__LINE__);
            if (!parent_is_red) {
                pottery_log("%i parent is black. making sibling red\n",__LINE__);
                pottery_rbt_set_is_red(rbt, sibling, true);

                if (pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt) parent, rbt->root)) {
                    pottery_log("%i reached root. done\n",__LINE__);
                    break;
                }

                pottery_log("%i looping on parent\n",__LINE__);
                entry = parent;
                is_red = pottery_rbt_is_red(rbt, entry);
                parent = pottery_rbt_parent(rbt, entry);
                pottery_assert(!pottery_rbt_ref_is_null(rbt, parent));
                parent_left_child = pottery_rbt_left_child(rbt, parent);
                entry_is_left = pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
                        entry, parent_left_child);
                pottery_assert(entry_is_left || pottery_rbt_ref_equal(POTTERY_RBT_CONTEXT_VAL(rbt)
                        entry, pottery_rbt_right_child(rbt, parent)));
                continue;
            } else {
                pottery_log("%i parent is red. swapping parent and sibling colors, done\n",__LINE__);
                pottery_rbt_set_is_red(rbt, parent, false);
                pottery_rbt_set_is_red(rbt, sibling, true);
                break;
            }
        }

        if (!far_nephew_is_red) {
            pottery_assert(close_nephew_is_red);
            pottery_log("%i far nephew is black, close nephew is red. rotating sibling %s\n",__LINE__, entry_is_left ? "right" : "left");
            if (entry_is_left)
                pottery_rbt_rotate_right(rbt, sibling);
            else
                pottery_rbt_rotate_left(rbt, sibling);
            pottery_rbt_set_is_red(rbt, close_nephew, false);
            pottery_rbt_set_is_red(rbt, sibling, true);
            sibling = close_nephew;
            sibling_is_red = false;

            // TODO copy paste again...
            sibling_left_child = pottery_rbt_left_child(rbt, sibling);
            sibling_right_child = pottery_rbt_right_child(rbt, sibling);
            close_nephew = entry_is_left ? sibling_left_child : sibling_right_child;
            far_nephew = entry_is_left ? sibling_right_child : sibling_left_child;
            close_nephew_is_red = !pottery_rbt_ref_is_null(rbt, close_nephew) &&
                    pottery_rbt_is_red(rbt, close_nephew);
            far_nephew_is_red = !pottery_rbt_ref_is_null(rbt, far_nephew) &&
                    pottery_rbt_is_red(rbt, far_nephew);
        }

        pottery_log("%i far nephew is red. rotating parent %s\n",__LINE__, entry_is_left ? "left" : "right");
        if (entry_is_left)
            pottery_rbt_rotate_left(rbt, parent);
        else
            pottery_rbt_rotate_right(rbt, parent);
        pottery_rbt_set_is_red(rbt, sibling, parent_is_red);
        pottery_rbt_set_is_red(rbt, parent, false);
        pottery_rbt_set_is_red(rbt, far_nephew, false);
        break;
    }

    pottery_rbt_sanity_check(rbt);
}

#if 0
POTTERY_RED_BLACK_TREE_EXTERN
pottery_error_t pottery_rbt_emplace_key(pottery_rbt_t* rbt, pottery_rbt_key_t key,
        pottery_rbt_entry_t* entry, bool* /*nullable*/ out_created)
{
    pottery_log("=================\n===================\n==============\n%i emplace key\n",__LINE__);
    pottery_rbt_sanity_check(rbt);

    if (pottery_rbt_is_empty(rbt)) {
        pottery_log("%i emplacing new root\n",__LINE__);
        pottery_assert(rbt->count == 0);
        pottery_assert(pottery_rbt_ref_is_null(rbt, rbt->root));
        pottery_rbt_ref_t ref = pottery_rbt_acquire(rbt);
        ++rbt->count;
        pottery_rbt_link_root(rbt, ref);
        *entry = ref;
        if (out_created != pottery_null)
            *out_created = true;
        return POTTERY_OK;
    }

    int compare;
    pottery_rbt_entry_t parent = pottery_rbt_find_key_or_parent(rbt, key, &compare);
    if (compare == 0) {
        *entry = parent;
        if (out_created != pottery_null)
            *out_created = false;
        return POTTERY_OK;
    }

    pottery_rbt_ref_t child = pottery_rbt_acquire(rbt);
    if (pottery_rbt_ref_is_null(rbt, child)) {
        *entry = pottery_rbt_null(rbt);
        if (out_created != pottery_null)
            *out_created = false;
        return POTTERY_ERROR_ALLOC;
    }

    pottery_rbt_set_parent(rbt, child, parent);
    pottery_rbt_set_left_child(rbt, child, pottery_rbt_null(rbt));
    pottery_rbt_set_right_child(rbt, child, pottery_rbt_null(rbt));
    pottery_rbt_set_is_red(rbt, child, true);

    if (compare < 0) {
        pottery_assert(pottery_rbt_ref_is_null(rbt, pottery_rbt_left_child(rbt, parent)));
        pottery_rbt_set_left_child(rbt, parent, child);
    } else {
        pottery_assert(pottery_rbt_ref_is_null(rbt, pottery_rbt_right_child(rbt, parent)));
        pottery_rbt_set_right_child(rbt, parent, child);
    }

    ++rbt->count;
    pottery_rbt_link_rebalance(rbt, parent, child);

    *entry = child;
    if (out_created != pottery_null)
        *out_created = true;
    return POTTERY_OK;
}
#endif
