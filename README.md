# Pottery

A header-only container and algorithm template library in C.

[![Unit Tests](https://github.com/ludocode/pottery/workflows/Unit%20Tests/badge.svg)](https://github.com/ludocode/pottery/actions?query=workflow%3A%22Unit+Tests%22)



## Introduction

Pottery is a collection of templates for instantiating strongly typed containers and container algorithms in C.

It's called "Pottery" because it doesn't exactly give you containers you can use out-of-the-box. It gives you the tools, the materials, the instructions to build them but there is still some assembly required. You have to instantiate these templates manually, and often you'll want to wrap one or more templates with your own interface to build a practical data structure.

Pottery is modern C code written in the ultra-portable intersection of C11, gnu89 and C++11 with no mandatory dependencies (not even libc.) Pottery supports many compilers including GCC, Clang, MSVC, TinyCC and more. It supports (or intends to support) any modern C platform from microcontrollers to OS kernels to WebAssembly.

Pottery does not use void pointer casts, function pointers, code block macros, compiler-dependent hacks, or any other inefficiencies or messyness of typical C containers. Pottery's templates are clean, composable, fast, strongly typed, and highly configurable.



## Documentation

- [Features](docs/features.md)
- [How It Works](docs/how_it_works.md)
- [Examples](examples/pottery/)
- [Integration Guide](docs/integration.md)
- [Lifecycle Style](docs/lifecycle_style.md)
- [Glossary](docs/glossary.md)
- [Template Listing](include/pottery/)
- [Meta-templates](meta/)
- [C++ Bindings](bindings/cxx)
- [Testing](test/)
- [FAQ](docs/faq.md)



## Examples


### Int Vector

Suppose you want a growable array of `int`. Let's call it `int_vector`. Instantiate it like this:

```c
#define POTTERY_VECTOR_PREFIX int_vector
#define POTTERY_VECTOR_VALUE_TYPE int
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1
#include "pottery/vector/pottery_vector_static.t.h"
```

This gives you a type `int_vector_t`:

```c
int_vector_t vector;
int_vector_init(&vector);

int_vector_insert_first(&vector, 10);
int_vector_insert_last(&vector, 20);
int_vector_insert_at(&vector, 1, 15);
int_vector_remove_at(&vector, 0);

for (size_t i = 0; i < int_vector_count(&vector); ++i)
    printf("%i\n", *int_vector_at(&vector, i));

int_vector_destroy(&vector);
```

See the full example [here](examples/pottery/int_vector/).


### String➔Object Map

Suppose you want a map of names to person pointers for this struct:

```c
typedef struct person_t {
    char* name;
    // other stuff
} person_t;

person_t* person_new(const char* name);
void person_delete(person_t* person);
// other functions
```

Instantiate a red-black tree map like this:

```c
#define POTTERY_TREE_MAP_PREFIX person_map
#define POTTERY_TREE_MAP_KEY_TYPE const char*
#define POTTERY_TREE_MAP_COMPARE_THREE_WAY strcmp
#define POTTERY_TREE_MAP_VALUE_TYPE person_t*
#define POTTERY_TREE_MAP_REF_KEY(person) (*person)->name
#define POTTERY_TREE_MAP_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_TREE_MAP_LIFECYCLE_DESTROY(person) person_delete(*person)
#include "pottery/tree_map/pottery_tree_map_static.t.h"
```

This gives you a `person_map_t`:

```c
person_map_t map;
person_map_init(&map);

person_map_insert(&map, person_new("alice"));
person_map_insert(&map, person_new("bob"));

person_t** eve = person_map_find(&map, "eve");
if (eve == NULL) {
    // not found!
}

// for-each loop works on any container
person_t** ref;
POTTERY_FOR_EACH(ref, person_map, &map) {
    printf("%s\n", (*ref)->name);
}

// map destroys its values with person_delete()
person_map_destroy(&map);
```

See the full example [here](examples/pottery/person_map/).


### Sort Strings

Suppose you want to sort an array of C strings:

```c
#define POTTERY_INTRO_SORT_PREFIX sort_strings
#define POTTERY_INTRO_SORT_VALUE_TYPE const char*
#define POTTERY_INTRO_SORT_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_INTRO_SORT_COMPARE_THREE_WAY(x, y) strcmp(*x, *y)
#include "pottery/intro_sort/pottery_intro_sort_static.t.h"
```

This gives us a function called `sort_strings()`:

```c
const char* players[] = {
    "fred", "quincy", "alice", "eve", "zack", "ned", "paul", "bob", "gary",
    "ursula", "yves", "carl", "olivia", "steve", "rob", "mike", "wade", "dave",
    "jake", "helen", "xavier", "karen", "tammy", "laura", "isaac", "vick",
};
size_t count = sizeof(players) / sizeof(*players);

sort_strings(players, count);

for (size_t i = 0; i < count; ++i)
    puts(players[i]);
```

See the full example [here](examples/pottery/sort_strings/).


### Request Queue

Suppose you want a growable priority queue of request pointers:

```c
typedef struct request_t {
    double priority;
    // other stuff
} request_t;

#define POTTERY_PRIORITY_QUEUE_PREFIX request_queue
#define POTTERY_PRIORITY_QUEUE_VALUE_TYPE request_t*
#define POTTERY_PRIORITY_QUEUE_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_PRIORITY_QUEUE_COMPARE_GREATER(x, y) (*x)->priority > (*y)->priority
#include "pottery/priority_queue/pottery_priority_queue_static.t.h"
```

This gives us a type `request_queue_t`:

```c
request_queue_t queue;
request_queue_init(&queue);

// insert some requests
request_queue_insert(&queue, request_new(/*...*/));
request_queue_insert(&queue, request_new(/*...*/));
request_queue_insert(&queue, request_new(/*...*/));

// handle requests in priority order
while (!request_queue_is_empty(&queue))
    request_handle(request_queue_extract_first(&queue));

request_queue_destroy(&queue);
```

See the [priority_queue](include/pottery/priority_queue/) documentation for more.


### Additional Examples

There are more examples in the [`examples/`](examples/pottery/) folder and many more that still need to be written. Have a look at what's there so far to learn more ways you can use Pottery.

In particular, take a look at [Clayfish](examples/pottery/clayfish/) to see various uses of Pottery in a "real" application.
