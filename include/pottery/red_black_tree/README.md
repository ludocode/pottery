# Red-Black Tree

An intrusive red-black tree.

This is for a tree where you modify your value type to store tree links and allocate values yourself. If you want a tree that stores ordinary values and dynamically allocates its storage, use [`tree_map`](../tree_map/).

This documentation is a work in progress. The main API for an intrusive data structure is `link()` and `unlink()`; see the [glossary](../../../docs/glossary.md).


## Configuring Links

### Simple Configuration

By default the tree expects that your value type is a struct containing ref-type fields called `parent`, `left_child` and `right_child`, plus a `bool is_red` field. For example:

```c
struct foo_t {

    // intrusive red-black tree fields
    struct foo_t* parent;
    struct foo_t* left_child;
    struct foo_t* right_child;
    bool is_red;

    // other stuff

};
```

This will work out-of-the-box with no link configuration.

```c
#define POTTERY_RED_BLACK_TREE_PREFIX foo_tree
#define POTTERY_RED_BLACK_TREE_VALUE_TYPE struct foo_t
#define POTTERY_RED_BLACK_TREE_COMPARE_LESS /*something*/
#include "pottery/red_black_tree/pottery_red_black_tree_static.t.h"
```


### Custom Links

You can configure arbitrary fields or expressions for accessing and setting each link. For example you can define `PARENT` to an l-value, or you can define both `PARENT` and `SET_PARENT` to custom expressions that do whatever you want.

In this example we wrap the links in a `links` struct and combine `right_child` and `is_red` into one field, where `is_red` is the low bit:

```c
struct foo_t {
    struct {
        struct foo_t* parent;
        struct foo_t* left_child;
        uintptr_t right_child_and_is_red;
    } links;

    // other stuff
};
```

To make this work, we configure expressions `RIGHT_CHILD`, `SET_RIGHT_CHILD`, `IS_RED` and `SET_IS_RED` to perform the masking:

```c
#define POTTERY_RED_BLACK_TREE_PREFIX foo_tree
#define POTTERY_RED_BLACK_TREE_VALUE_TYPE struct foo_t
#define POTTERY_RED_BLACK_TREE_COMPARE_LESS /*something*/
#define POTTERY_RED_BLACK_TREE_PARENT(foo) foo->links.parent // l-value
#define POTTERY_RED_BLACK_TREE_LEFT_CHILD(foo) foo->links.left_child // l-value
#define POTTERY_RED_BLACK_TREE_RIGHT_CHILD(foo) \
        (struct foo_t*)(foo->links.right_child_and_is_red & ~(uintptr_t)1)
#define POTTERY_RED_BLACK_TREE_SET_RIGHT_CHILD(foo, v) \
        foo->links.right_child_and_is_red = (uintptr_t)v | (foo->links.right_child_and_is_red & (uintptr_t)1)
#define POTTERY_RED_BLACK_TREE_IS_RED(foo) \
        (bool)(foo->links.right_child_and_is_red & (uintptr_t)1)
#define POTTERY_RED_BLACK_TREE_SET_IS_RED(foo, r) \
        foo->links.right_child_and_is_red = (uintptr_t)(!!r) | (foo->links.right_child_and_is_red & ~(uintptr_t)1)
#include "pottery/red_black_tree/pottery_red_black_tree_static.t.h"
```

The above is in fact how [`tree_map`](../tree_map/) configures its internal `red_black_tree`.



### Embedded Pool

Links are of the ref type, which is conceptually an abstract handle to a value. It doesn't have to be a pointer. This means you can define a ref type of, say, `uint8_t`, representing the index of a node in a small fixed pool. This is ideal for a red-black tree on an embedded device:

```c
struct foo_t {
    uint8_t parent;
    uint8_t left_child;
    uint8_t right_child;
    bool is_red;

    // other stuff
};

static foo_t foos[255];
```

By passing the pool (or some other struct containing the pool) as the context, you can dereference nodes based on their ref. You'll also want to define a custom value to represent a null ref.

```c
#define POTTERY_RED_BLACK_TREE_PREFIX foo_tree
#define POTTERY_RED_BLACK_TREE_REF_TYPE uint8_t
#define POTTERY_RED_BLACK_TREE_CONTEXT_TYPE foo_t* // pool
#define POTTERY_RED_BLACK_TREE_COMPARE_LESS(pool, a, b) foo_less(&pool[a], &pool[b])
#define POTTERY_RED_BLACK_TREE_PARENT(pool, ref) pool[ref].parent
#define POTTERY_RED_BLACK_TREE_LEFT_CHILD(pool, ref) pool[ref].left_child
#define POTTERY_RED_BLACK_TREE_RIGHT_CHILD(pool, ref) pool[ref].right_child
#define POTTERY_RED_BLACK_TREE_IS_RED(pool, ref) pool[ref].is_red
#define POTTERY_RED_BLACK_TREE_NULL(pool) 255
```

(Eventually I'd like to try to make the parent link optional to make this even more suitable for embedded. I have some ideas of how to get there but it will take a lot of work.)


### Vector Pool

Like the previous example, you could store nodes in a pool, but make it a growable array (say, a [vector](../vector/)). This has the advantage that all nodes are in a contiguous slab in memory.

There are some downsides of course: since nodes can be moved in memory, you can't store pointers to them, and each node access incurs a double dereference. This may or may not be faster than individually allocating nodes, and it may or may not reduce memory consumption or fragmentation. The nice thing about Pottery is it makes it really easy to build, so you can just try it and see if it's faster. This is why it's called "Pottery" after all.

Here's how you might do it:

```c
struct foo_t {
    uint32_t parent;
    uint32_t left_child;
    uint32_t right_child;
    bool is_red;

    // other stuff
};

// pool of foo nodes
#define POTTERY_VECTOR_PREFIX foo_pool
#define POTTERY_VECTOR_VALUE_TYPE foo_t
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1
#include "pottery/vector/pottery_vector_static.t.h"

// tree of foos
#define POTTERY_RED_BLACK_TREE_PREFIX foo_tree
#define POTTERY_RED_BLACK_TREE_REF_TYPE uint32_t
#define POTTERY_RED_BLACK_TREE_CONTEXT_TYPE foo_pool_t*
#define POTTERY_RED_BLACK_TREE_COMPARE_LESS(pool, a, b) foo_less(foo_pool_at(pool, a), foo_pool_at(pool, b))
#define POTTERY_RED_BLACK_TREE_PARENT(pool, ref) foo_pool_at(pool, ref)->parent
#define POTTERY_RED_BLACK_TREE_LEFT_CHILD(pool, ref) foo_pool_at(pool, ref)->left_child
#define POTTERY_RED_BLACK_TREE_RIGHT_CHILD(pool, ref) foo_pool_at(pool, ref)->right_child
#define POTTERY_RED_BLACK_TREE_IS_RED(pool, ref) foo_pool_at(pool, ref)->is_red
#define POTTERY_RED_BLACK_TREE_NULL(pool) UINT32_MAX
```

You can now get new nodes by appending to the pool and inserting them into the tree.

For deleting nodes, you have some options. If you want to keep each node's ref consistent so they can be referred to outside of the tree, you'll want to maintain a free list. You can do this with an intrusive an instrusive [doubly linked list](../doubly_linked_list) over deleted nodes as a free list. You can use the same technique of pooling and abstract refs, and you can store the links in a union so they don't take up space for extant values. Like this:

```c
struct foo_t {
    union {
        struct {
            uint32_t parent;
            uint32_t left_child;
            uint32_t right_child;
            bool is_red;
        } tree;
        struct {
            uint32_t previous;
            uint32_t next;
        } list;
    } links;

    // other stuff
};

// list of foos, used for storing the free list in the pool
#define POTTERY_DOUBLY_LINKED_LIST_PREFIX foo_list
#define POTTERY_DOUBLY_LINKED_LIST_REF_TYPE uint32_t
#define POTTERY_DOUBLY_LINKED_LIST_CONTEXT_TYPE foo_pool_t*
#define POTTERY_DOUBLY_LINKED_LIST_PREVIOUS(pool, ref) foo_pool_at(pool, ref)->previous
#define POTTERY_DOUBLY_LINKED_LIST_NEXT(pool, ref) foo_pool_at(pool, ref)->next
#define POTTERY_DOUBLY_LINKED_LIST_NULL(pool) UINT32_MAX
#include "pottery/doubly_linked_list/pottery_doubly_linked_list_static.t.h"
```

As an alternative, you can even compact the array: if a node is deleted, you can move the last node into its place and `replace()` it. This will fix up the references from other nodes that point to it. Of course this won't preserve the ref of elements, so only do this if you don't expose the refs out of your data structure.

You can of course combine fields as in the earlier example to save even more space, perhaps storing the `is_red` flag in the high bit of `right_child`.



