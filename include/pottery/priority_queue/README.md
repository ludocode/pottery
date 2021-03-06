# Priority Queue

A priority queue, implemented as a binary [`heap`](../heap/) over an arbitrary growable array.

By default, the priority queue instantiates a [`vector`](../vector/) for storing its values. In this configuration it's a close analogue to C++ [`std::priority_queue`](https://en.cppreference.com/w/cpp/container/priority_queue).

You can instead configure the priority queue to use an arbitrary growable array by defining `POTTERY_PRIORITY_QUEUE_EXTERNAL_ARRAY`. For example you could use a [`pager`](../pager/) as the backing array to eliminate stutter from vector's O(n) resizing steps as the array grows.

Currently only `insert()` is implemented for adding values. `emplace()` is not implemented yet so your type needs to be passable (movable by value.) This will be fixed later when heap gets `emplace()` support. The priority queue will also eventually get key support so elements are compared by key and you can emplace by key. Currently `KEY_TYPE` is ignored.



## Examples

### Simple queue of pointers

Suppose you have a request struct like this:

```c
typedef struct request_t {
    double priority;
    // other stuff
} request_t;

request_t* request_new(/*...*/);
void request_handle_and_delete(request_t* request);
```

Instantiate a growable priority queue of request pointers like this:

```c
#define POTTERY_PRIORITY_QUEUE_PREFIX request_queue
#define POTTERY_PRIORITY_QUEUE_VALUE_TYPE request_t*
#define POTTERY_PRIORITY_QUEUE_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_PRIORITY_QUEUE_COMPARE_GREATER(x, y) (*x)->priority > (*y)->priority
#include "pottery/priority_queue/pottery_priority_queue_static.t.h"
```

This gives you a type `request_queue_t`. Use it like this:

```c
request_queue_t queue;
request_queue_init(&queue);

// insert some requests
request_queue_insert(&queue, request_new(/*...*/));
request_queue_insert(&queue, request_new(/*...*/));
request_queue_insert(&queue, request_new(/*...*/));

// handle requests in priority order
while (!request_queue_is_empty(&queue))
    request_handle_and_delete(request_queue_extract_first(&queue));

request_queue_destroy(&queue);
```

### Pointers with index tracking

Like [`heap`](../heap/), Pottery's priority queue supports index tracking of values and removal of arbitrary values by index.

Suppose you want a priority queue containing request pointers, similar to the simple example above. This time we'll store an intrusive `queue_index`, the index of the request in the priority queue:

```c
typedef struct request_t {
    double priority;
    size_t queue_index;
    // other stuff
} request_t;

#define POTTERY_PRIORITY_QUEUE_PREFIX request_queue
#define POTTERY_PRIORITY_QUEUE_VALUE_TYPE request_t*
#define POTTERY_PRIORITY_QUEUE_LIFECYCLE_BY_VALUE 1
#define POTTERY_PRIORITY_QUEUE_COMPARE_LESS(x, y) (*x)->priority < (*y)->priority
#define POTTERY_PRIORITY_QUEUE_SET_INDEX(x, i) (*x)->queue_index = i
#include "pottery/priority_queue/pottery_priority_queue_declare.t.h"
```

The `queue_index` field will now reflect the index of a request within the priority queue. It is updated automatically as values move around in the queue. This allows you to cancel and remove arbitrary requests from the queue:

```c
void request_cancel(request_t* request, request_queue_t* queue) {
    request_queue_remove_at(queue, request->queue_index);
    // destroy request
}
```

This pattern is used throughout [HomeFort](https://homefort.app/).

### Structs by value

Here's a priority queue of values of a struct type `foo_t`:

```c
typedef struct foo_t {
    double priority;
    // other stuff
} foo_t;

#define POTTERY_PRIORITY_QUEUE_PREFIX foo_queue
#define POTTERY_PRIORITY_QUEUE_VALUE_TYPE foo_t
#define POTTERY_PRIORITY_QUEUE_LIFECYCLE_BY_VALUE 1
#define POTTERY_PRIORITY_QUEUE_COMPARE_LESS(x, y) x->priority < y->priority
#include "pottery/priority_queue/pottery_priority_queue_declare.t.h"
```

The structs are stored by value and moved bitwise. If your `foo_t` is not bitwise movable, you could instead define `LIFECYCLE_MOVE` to a move expression to relocate the storage of a `foo_t` value. (NOTE: For heap and priority queue this is not implemented yet; types currently must be passable.)
