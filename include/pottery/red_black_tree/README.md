# Red-Black Tree

An intrusive red-black tree.

This is for a tree where you modify your types to store tree links and allocate them yourself. If you want a tree that stores ordinary values and dynamically allocates its storage, use [`tree_map`](../tree_map/).

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



### Custom Ref Types

Links are of the ref type. This means you can define a ref type of, say, `uint8_t`, representing the index of a node in a small fixed pool. This is ideal for a red-black tree on an embedded device:

```c
struct foo_t {
    uint8_t parent;
    uint8_t left_child;
    uint8_t right_child;
    uint8_t is_red;

    // other stuff
};

static foo_t foos[255];
```

By passing the pool (or some other struct containing the pool) as the context, you can dereference nodes based on their ref. You'll also want to define a custom value to represent a null ref.

```c
#define POTTERY_RED_BLACK_TREE_PREFIX foo_tree
#define POTTERY_RED_BLACK_TREE_REF_TYPE uint8_t
#define POTTERY_RED_BLACK_TREE_CONTEXT_TYPE foo_t* // pool
#define POTTERY_RED_BLACK_TREE_COMPARE_LESS /*something*/
#define POTTERY_RED_BLACK_TREE_PARENT(pool, ref) pool[ref].parent
#define POTTERY_RED_BLACK_TREE_LEFT_CHILD(pool, ref) pool[ref].left_child
#define POTTERY_RED_BLACK_TREE_RIGHT_CHILD(pool, ref) pool[ref].right_child
#define POTTERY_RED_BLACK_TREE_IS_RED(pool, ref) pool[ref].is_red
#define POTTERY_RED_BLACK_TREE_NULL(pool) 255
```

(Eventually I'd like to try to make the parent link optional to make this even more suitable for embedded. I have some ideas of how to get there but it will take a lot of work.)
