# Doubly-linked List

An intrusive doubly-linked list.

The linked list and its values contain links to other values in the list. This means the entry for a value (usually its address) cannot change while it is in a linked list. In other words, values cannot be moved. You are responsible for allocating the entries and ensuring they remain fixed in place while they are part of the list.

The linked list supports an arbitrary ref (or value) type and arbitrary storage of next and previous refs within your values. By default it expects that your ref type is a pointer to a struct and the next and previous pointers are stored in fields called `next` and `previous`. The simplest way to use the doubly-linked list is to store `next` and `previous` pointers in your struct and just define `VALUE_TYPE` to the type of your struct.

You can define `NEXT` and `PREVIOUS` to change where these refs are stored within your values. You can also define `SET_NEXT` and `SET_PREVIOUS` to customize how they are stored (in case `NEXT` and `PREVIOUS` are not l-values.) In any case, head and tail refs are stored by value within the linked list container. You can also define an arbitrary `REF_TYPE`, a custom `NULL` to represent empty refs if not `NULL`, and even a `REF_EQUAL` expression in case your ref type is not comparable with operator `==`. 



## Examples


### Simple linked list

Here's the simplest possible doubly-linked list. Suppose you have a struct `foo_t` and you want to make an intrusive linked list out of it. Add next and previous pointers:

```c
typedef struct foo_t {
    struct foo_t* next;
    struct foo_t* previous;
    // other stuff
} foo_t;
```

Then instantiate the linked list. Since we've called our pointers `next` and `previous` we don't need to configure them:

```c
#define POTTERY_DOUBLY_LINKED_LIST_PREFIX foo_list
#define POTTERY_DOUBLY_LINKED_LIST_VALUE_TYPE foo_t
#include "pottery/doubly_linked_list/pottery_doubly_linked_list_static.t.h"
```

You can now create a linked list of individually allocated nodes:

```c
foo_list_t list;
foo_list_init(&list);

// intrusive insert is called "link"
foo_list_link_first(&list, foo_new("hello"));
foo_list_link_last(&list, foo_new("world"));

// intrusive remove is called "unlink"
while (!foo_list_is_empty(&list)) {
    foo_delete(foo_unlink_first(&list));
}

foo_destroy(&list);
```


### Multiple linked lists

Here's how you might store values within two independent intrusive linked lists. This demonstrates using custom next and previous fields. You can store these fields however you like; here we store them in two different ways for demonstration purposes:

```c
typedef struct node_t {

    // first list, storing them directly
    struct node_t* foo_next;
    struct node_t* foo_previous;

    // second list, storing them in a wrapper struct
    struct {
        struct node_t* next;
        struct node_t* previous;
    } bar_links;

    // other stuff

} node_t;

#define POTTERY_DOUBLY_LINKED_LIST_PREFIX foo_list
#define POTTERY_DOUBLY_LINKED_LIST_VALUE_TYPE node_t
#define POTTERY_DOUBLY_LINKED_LIST_NEXT(p) p->foo_next
#define POTTERY_DOUBLY_LINKED_LIST_PREVIOUS(p) p->foo_previous
#include "pottery/doubly_linked_list/pottery_doubly_linked_list_static.t.h"

#define POTTERY_DOUBLY_LINKED_LIST_PREFIX bar_list
#define POTTERY_DOUBLY_LINKED_LIST_VALUE_TYPE node_t
#define POTTERY_DOUBLY_LINKED_LIST_NEXT(p) p->bar_links.next
#define POTTERY_DOUBLY_LINKED_LIST_PREVIOUS(p) p->bar_links.previous
#include "pottery/doubly_linked_list/pottery_doubly_linked_list_static.t.h"
```


### Embedded node pool referenced by index

Suppose you want a small pool of nodes on an embedded device and you want them linked by index in order to minimize memory usage. Like this:

```c
typedef struct bar_t {
    uint8_t next
    uint8_t previous;
    // other stuff
} bar_t;

static bar_t bar_pool[100];
```

In this case the `REF_TYPE` is `uint8_t`. We provide a `NULL` value of `UINT8_MAX` to indicate non-existent references. We don't need to provide a `REF_EQUAL` expression because `uint8_t` can be compared with operator `==`.

```c
#define POTTERY_DOUBLY_LINKED_LIST_PREFIX bar_list
#define POTTERY_DOUBLY_LINKED_LIST_REF_TYPE uint8_t
#define POTTERY_DOUBLY_LINKED_LIST_NULL UINT8_MAX
#define POTTERY_DOUBLY_LINKED_LIST_NEXT(bar_id) bar_pool[bar_id].next
#define POTTERY_DOUBLY_LINKED_LIST_PREVIOUS(bar_id) bar_pool[bar_id].previous
#include "pottery/doubly_linked_list/pottery_doubly_linked_list_static.t.h"
```

This instantiates a doubly-linked list using the indices as node references. This uses only two bytes per node.

Since we're using a static global pool, we didn't need to pass it as the context type. The above just uses the pool directly. You could instead use a `CONTEXT_TYPE` to support a dynamic pool (for example, you could allocate your nodes from a [vector](../vector/).)

We can now form a list of nodes from the pool and perform various operations on them:

```c
bar_list_t list;
bar_list_init(&list);

// Initialize some nodes
bar_init(&bar_pool[0], /*...*/)
bar_init(&bar_pool[1], /*...*/)
bar_init(&bar_pool[2], /*...*/)

// Link them into the list. Remember our ref type is just the index.
bar_list_link_first(&list, 0);
bar_list_link_last(&list, 1);
bar_list_link_last(&list, 2);

// Loop over the list, conditionally removing some nodes.
uint8_t entry = bar_list_first(&list);
while (bar_list_entry_exists(&list, entry)) {
    bar_t* bar = &bar_pool[entry];

    if (/*something about bar*/) {
        // remove this entry
        entry = bar_list_remove_and_next(&list, entry);
    } else {
        // don't remove it
        entry = bar_list_next(&list, entry);
    }
}
```
