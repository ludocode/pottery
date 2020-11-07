# Pottery

A header-only container and algorithm template library in C.



## Introduction

Pottery is a collection of templates for instantiating strongly typed containers and container algorithms in C.

It's called "Pottery" because it doesn't exactly give you containers you can use out-of-the-box. It gives you the tools and the materials to build them but you still need to assemble them yourself. This can mean simply configuring and instantiating a template, but often you'll want to wrap one or more templates with your own code to build a practical data structure.

Pottery is modern C code written in the ultra-portable intersection of C11, gnu89 and C++11 with no mandatory dependencies (not even libc.) Pottery supports many compilers including GCC, Clang, MSVC, TinyCC and various others. It supports (or intends to support) any modern C platform from microcontrollers to OS kernels to WebAssembly.

Pottery does not use void pointer casts, function pointers, code block macros, compiler-dependent hacks, or any other inefficiencies or messyness of typical C containers. Pottery's templates are clean, composable, fast, strongly typed, and highly configurable.

Pottery is not just an experiment to push the boundaries of meta-programming in C. It's a real library for writing real programs, and it's used throughout [HomeFort](https://homefort.app/). I expect that most people who dig into this will decide it is some kind of deranged insanity. For those that don't, I hope you find it not just a curiosity but actually useful.

#### Alpha Quality

This is early stage alpha-quality software under development. It is incomplete, poorly documented and full of bugs, it has had very little testing and benchmarking, and APIs are likely to change. There are currently not a lot of algorithms and data structures here but the templating system is pretty solid so I hope to implement many more in the future. (In fact several more are written already but they are not clean enough to be released yet.)

Mostly I am releasing this now because I am out of money and need to go get a real job again. If you would like to pay me to write code (for this or anything else) please get in touch because I really need to feed [my daughter](https://ludocode.com/assets/celeste-kraft-dinner.jpg).



## Documentation

- [Features](docs/features.md)
- [How It Works](docs/how_it_works.md)
- [Examples](examples/pottery/)
- [Integration](docs/integration.md)
- [Core Concepts](docs/concepts.md)
- [Template Listing](include/pottery/)
- [Meta-templates](meta/)
- [C++ Bindings](bindings/cxx)
- [Testing](test/)
- [FAQ](docs/faq.md)



## Examples


### Sort Strings

Suppose you want to sort an array of strings. You could use `qsort()` but it's not ideal: it uses void pointers; it has to `memcpy()` elements around; and you have to write a special compare function which it calls through a function pointer. This makes it slow and awkward to use.

Instead we can instantiate a strongly-typed sort algorithm with Pottery:

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

The template instantiation preprocesses into a full implementation of [introsort](https://en.wikipedia.org/wiki/Introsort). It takes `const char*[]` as argument, moves `const char*` values around with operator `=`, and calls `strcmp()` directly rather than through a function pointer. The performance (and code size!) is comparable to a typical implementation of C++ `std::sort<>()`.

The [intro_sort](include/pottery/intro_sort/) template uses several other Pottery templates which themselves use other templates: [quick_sort](include/pottery/quick_sort/), [insertion_sort](include/pottery/insertion_sort/), [heap_sort](include/pottery/heap_sort/) which uses [heap](include/pottery/heap/), and the [lifecycle](include/pottery/lifecycle/) and [compare](include/pottery/compare/) helpers. Pottery's templates are composable and its algorithms and containers are split up into small independent components.

We've defined `MOVE_BY_VALUE` to 1 above because `const char*` can be moved by simple assignment. You could instead provide a custom move or swap expression if the type is not bitwise-movable. You could provide a boolean `LESS` expression or others instead of a three-way comparison. You could provide a custom context and access expressions in case the array is not contiguous in memory (so you could sort a [deque](include/pottery/deque/) for example.)

You could even sort over an abstract reference type, so you could provide access, compare and swap functions that query a database, pull elements off a tape drive, make network requests, etc. Pottery's algorithms are not limited to operating on plain values in memory.

See the full example [here](examples/pottery/sort_strings/).


### Int Vector

Suppose you want a dynamically growable array of integers. Let's call it `int_vector`. Instantiate it like this:

```c
#define POTTERY_VECTOR_PREFIX int_vector
#define POTTERY_VECTOR_VALUE_TYPE int
#define POTTERY_VECTOR_LIFECYCLE_BY_VALUE 1
#include "pottery/vector/pottery_vector_static.t.h"
```

You now have a type called `int_vector_t`. You can initialize it with `int_vector_init()`; add to it with `int_vector_insert_at()` and friends; access the contents by `int_vector_count()` and `int_vector_at()`; and so forth. Like this:

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

These are all real functions. You can set a breakpoint on `int_vector_insert_at()` and step through it, bringing you into the [vector implementation code](include/pottery/vector/impl/pottery_vector_definitions.t.h). You can get nice stack traces if something goes wrong. You'll even get somewhat legible compiler errors (`-fmax-errors=1` in GCC or `-Wfatal-errors` in Clang can help.)

We've defined `LIFECYCLE_BY_VALUE` to 1 above because `int` is trivially copyable and destroyable. You could instead provide a destroy expression for the vector to call on your elements to clean them up (for example if it contained pointers, you could tell it to destroy them with `free()`.) You could provide a move expression in case your type is not bitwise-movable. You could give it a copy initialization expression for your type to allow whole vector copies. Pottery calls all of these expressions directly, not through function pointers. Pottery's dynamic containers can fully manage the lifecycles of the elements they contain.

You could also provide the vector with a custom allocator and context. You could configure it to provide some internal space for a small number of elements to avoid small allocations. You could configure it as a double-ended vector. You can even instantiate separate header and source files to use it in multiple translation units without each having a copy of the implementation.

See the full example [here](examples/pottery/int_vector/).


### String Set

Suppose you want a dynamically growable set of unique strings.

We'll use an [`open_hash_map`](include/pottery/open_hash_map/) for this, so first we define a [simple hash function](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash):

```c
static inline size_t fnv1a(const char* p) {
    uint32_t hash = 2166136261;
    for (; *p != 0; ++p)
        hash = (hash ^ (uint8_t)*p) * 16777619;
    return hash;
}
```

You could of course use a better hash function like MurmurHash or CityHash or whatever the cool kids are using these days.

Next we define our map. We'll use `char*` as the value type and `const char*` as the key type, that way we can search it with const strings. In Pottery, map values contain their own keys, so in our case the key for a value is just the value itself.

```c
#define POTTERY_OPEN_HASH_MAP_PREFIX string_set_map
#define POTTERY_OPEN_HASH_MAP_VALUE_TYPE char*
#define POTTERY_OPEN_HASH_MAP_KEY_TYPE const char*
#define POTTERY_OPEN_HASH_MAP_VALUE_KEY(x) *x
#define POTTERY_OPEN_HASH_MAP_KEY_HASH fnv1a
#define POTTERY_OPEN_HASH_MAP_KEY_EQUAL 0 == strcmp
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_MOVE_BY_VALUE 1
#define POTTERY_OPEN_HASH_MAP_LIFECYCLE_DESTROY(x) free(*x)
#define POTTERY_OPEN_HASH_MAP_EMPTY_IS_ZERO 1
#include "pottery/open_hash_map/pottery_open_hash_map_static.t.h"
```

This gives us a `string_set_map_t`. We can initialize it with `string_set_map_init()`; add allocated strings with `string_set_map_insert()` or `emplace()`; query strings with `string_set_map_find()`; remove strings with `string_set_map_remove()` or `remove_key()`; etc.

We've declared that the map should hash keys with `fnv1a()`, compare keys with `strcmp()`, move values by simple assignment, and destroy values with `free()`. The map will manage our string memory for us.

We've also declared `EMPTY_IS_ZERO`. This means that zero (null) is a sentinel value that can be used to mark an empty bucket, making the map more efficient. We could instead (or also) have provided custom `IS_EMPTY` and `SET_EMPTY` expressions to provide some other means of marking an empty bucket (for example if the map value was a struct, we could use a bit in the struct.) We could also declare nothing about empty buckets, in which case the map will allocate its own metadata bits to store which buckets are empty.

The map uses linear probing by default. We could instead have declared `QUADRATIC_PROBING` or `DOUBLE_HASHING` to change the probing strategy. Alternate probing algorithms require the use of tombstones: we could declare `IS_TOMBSTONE` and `SET_TOMBSTONE` to store those in-band like our empty state, or just let the map store this metadata on its own. Pottery's hash tables can be extensively configured to get the exact behaviour you want.

With this example you can start to see why it's called "Pottery". Pottery doesn't just give you a generic set in the way you might type `Set<String>` in other languages. Instead you use Pottery to build your own containers, exercising a level of control over algorithms and memory layout that you can only dream of in other programming languages. You should wrap this map in a nicer container API, one that hides all of the configurability and verbosity of Pottery. Something like this:

```c
typedef struct string_set_t string_set_t;

string_set_t* string_set_new(void);
void string_set_delete(string_set_t*);

bool string_set_add(string_set_t*, const char*);
bool string_set_query(string_set_t*, const char*);
bool string_set_remove(string_set_t*, const char*);
```

It is trivial to implement these functions as wrappers to our `string_set_map`. See the implementation of this in the full example [here](examples/pottery/string_set/).


### Additional Examples

There are more examples in the [`examples/`](examples/pottery/) folder and many more that stll need to be written. Have a look at what's there so far to learn more ways you can use Pottery.
