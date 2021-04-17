# Node List

A doubly-linked list that dynamically allocates nodes to store values. This is essentially a [lifecycle](../lifecycle/) and [alloc](../alloc/) wrapper around [doubly\_linked\_list](../doubly_linked_list/).

This the closest Pottery analogue to a typical implementation of C++ `std::list`.

## Example

### Int List

Here's a list of `int`s:

```c
#define POTTERY_NODE_LIST_PREFIX int_list
#define POTTERY_NODE_LIST_VALUE_TYPE int
#define POTTERY_NODE_LIST_LIFECYCLE_BY_VALUE 1
#include "pottery/node_list/pottery_node_list_static.t.h"
```

Use it like this:

```c
int_list_t list;
int_list_init(&list);

int_list_insert_last(&list, 1);
int_list_insert_last(&list, 2);
int_list_insert_last(&list, 3);

int* i = int_list_begin(&list);
while (i != NULL) {
    printf("int: %i\n", *i);
    i = int_list_next(&list, i);
}

int_list_destroy(&list); // frees all ints
```

Of course this is hugely inefficient since each int will be individually allocated with next/previous pointers. See the next example for something more realistic.

### Observer Pattern

Here's a pattern used often in [HomeFort](https://homefort.app/). Suppose you have an object, called here `subject_t`, that maintains a list of listeners for events. This is often called the [Observer Pattern](https://en.wikipedia.org/wiki/Observer_pattern).

```c
// subject.h

typedef struct subject_t subject_t;

subject_t* subject_new(void);

void subject_delete(subject_t* subject);


typedef struct subject_listener_t subject_listener_t;

subject_listener_t* subject_register_listener(subject_t* subject,
    void (*callback)(void* context, subject_t* subject), void* context);

void subject_unregister_listener(subject_t* subject, subject_listener_t* listener);
```

This subject lets you register and unregister callbacks which it will call when some event happens. You can easily implement this with a node list, letting it handle allocation and management of listeners for you. The advantage of using a node list is that it makes it easy to allow listeners to unregister themselves during their callback.

```c
// subject.c

typedef struct subject_listener_t {
    void (*callback)(void* context, subject_t* subject);
    void* context;
} subject_listener_t;

#define POTTERY_NODE_LIST_PREFIX subject_listeners
#define POTTERY_NODE_LIST_VALUE_TYPE subject_listener_t
#define POTTERY_NODE_LIST_LIFECYCLE_BY_VALUE 1
#include "pottery/node_list/pottery_node_list_static.t.h"

struct subject_t {
    subject_listeners_t listeners;
    // other stuff
}

subject_t* subject_new(void) {
    subject_t* subject = (subject_t*) malloc(sizeof(subject_t));
    subject_listeners_init(&subject->listeners);
    // init other stuff
    return subject;
}

void subject_delete(subject_t* subject) {
    // destroy other stuff
    if (!subject_listeners_is_empty(&subject->listeners)) {
        // A listener wasn't unregistered!
        abort();
    }
    subject_listeners_destroy(&subject->listeners);
}

subject_listener_t* subject_register_listener(subject_t* subject,
    void (*callback)(void* context, subject_t* subject), void* context)
{
    subject_listener_t* listener;
    subject_listeners_emplace_last(&subject->listeners, &listener);
    listener->callback = callback;
    listener->context = context;
    return listener;
}

void subject_unregister_listener(subject_t* subject, subject_listener_t* listener) {
    subject_listeners_remove(&subject->listeners, listener);
}

static void subject_invoke_listeners(subject_t* subject) {
    subject_listener_t* listener = subject_listeners_begin(&subject->listeners);
    while (listener != NULL) {
        // get next listener before calling callback in case a listener
        // unregisters itself during its callback
        subject_listener_t* next = subject_listeners_next(&subject->listeners, listener);
        listener->callback(listener->context, subject);
        listener = next;
    }
}
```
