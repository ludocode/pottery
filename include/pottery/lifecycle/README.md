# Pottery Lifecycle Template

This is a helper template for other Pottery containers. It is not intended to be used directly.

The Pottery Lifecycle template defines functions that wrap the arbitrary lifecycle expressions you define for your element types stored in Pottery data structures.

The instantiated lifecycle functions follow [Pottery-style lifecycle semantics and naming conventions](../../../docs/lifecycle_style.md). Most Pottery containers and algorithms use this template internally to store or manipulate elements.

The Lifecycle template instantiates functions for as many of the following eight fundamental lifecycle behaviours as possible:

- `init()`
- `init_copy()`
- `init_steal()`
- `copy()`
- `steal()`
- `move()`
- `swap()`
- `destroy()`

The first four of these are allowed to fail; they return an error code result. The last four cannot fail and return void. If an `init*()` function fails, the destination object is not initialized and must not be destroyed.

In addition, the template instantiates functions to operate in bulk:

- `destroy_bulk()` -- destroys an array of objects
- `move_bulk()` -- moves an array of objects to a potentially overlapping address
- `move_bulk_restrict()` -- moves an array of objects to a non-overlapping address
- `move_bulk_up()` -- moves an array of objects to a potentially overlapping higher address
- `move_bulk_down()` -- moves an array of objects to a potentially overlapping lower address



### State and Context

All configured lifecycle expressions can take an optional context as the first parameter. This is enabled if a `CONTEXT_TYPE` is defined. The below descriptions of configuration parameters includes the context; it is omitted if there is no context.

See [Contexts](../../../docs/how_it_works#contexts) for information on implementation.



### C++ notes

Pottery can generate lifecycle functions that wrap C++ constructors, destructors and assignment operators. This is done for any lifecycle expression configured `BY_VALUE`. This is the normal way to put C++ types in Pottery containers.

In C++, this is the behaviour of enabled `BY_VALUE ` flags:

- `DESTROY_BY_VALUE` -- Pottery will call the destructor;
- `INIT_BY_VALUE` -- Pottery will call the default constructor;
- `INIT_COPY_BY_VALUE` -- Pottery will use copy construction;
- `INIT_STEAL_BY_VALUE` -- Pottery will use move construction;
- `COPY_BY_VALUE` -- Pottery will use copy assignment.
- `STEAL_BY_VALUE` -- Pottery will use move assignment;
- `MOVE_BY_VALUE` -- Pottery will use move construction followed by the destructor of the source;
- `SWAP_BY_VALUE` -- Pottery will call a `swap()` function found by argument-dependent lookup or `std::swap()`.

The `init()`, `init_steal()`, `init_copy()` and `copy()` functions can fail, so their corresponding expressions must return a Pottery error code. When wrapping C++ constructors, destructors or assignment operators, Pottery will catch exceptions (if enabled) and convert them to error codes. `std::bad_alloc` is converted to `POTTERY_ERROR_ALLOC` and all other exceptions are converted to `POTTERY_ERROR_CXX_EXCEPTION`.

The `destroy()`, `move()`, `steal()` and `swap()` functions cannot throw, and the evaluation result of their corresponding configuration expressions is ignored. If a wrapped C++ destructor, move constructor or move assignment operator throws an exception, Pottery will abort. ([See the FAQ on fallible move for a rationale.](../../../docs/faq.md#why-cant-move-fail).)

If a custom expression is defined for any lifecycle operation of a C++ object, this is run _instead of_ the constructor or destructor it would normally run. For example if you define an `INIT_COPY` expression, Pottery will not run the copy constructor or any constructor when initializing the object. It is your responsibility to run a constructor as part of `INIT_COPY` or do whatever is necessary to make the object valid.

If you define custom expressions in C++, they cannot throw. You must catch all exceptions and convert them to Pottery error codes yourself. All generated lifecycle functions are marked `noexcept` so they will abort if an exception is thrown.



# Configuration Options

All below configuration options are prefixed by the template being instantiated along with `LIFECYCLE`. For example on [`vector`](../vector/), `BY_VALUE` is `POTTERY_VECTOR_LIFECYCLE_BY_VALUE`. When using lifecycle directly, the prefix is just `POTTERY_LIFECYCLE`.



## Types

### `REF_TYPE`

- `REF_TYPE`, a type

This is the abstract reference type for which lifecycle functions are being generated. Values of this type are passed to all configured lifecycle expressions and acceepted by all generated functions (after the optional context.)

This is usually either a pointer to the real type allowing lifecycle operations on objects in memory, or an abstract identifier to be used with the context to identify it. For example the context may be a database connection and the type may be the key for a row: in this case the `COPY` expression would copy data from one row to another.

### `VALUE_TYPE`

- `VALUE_TYPE`, a type

This is a concrete value type for performing lifecycle operations on objects in memory. This is required for any of the `BY_VALUE` configuration options and for bulk operations on ranges of elements.

If this is defined, `REF_TYPE` will be defined as a pointer to this. You cannot define both `VALUE_TYPE` and `REF_TYPE`.

### `CONTEXT_TYPE`

- `CONTEXT_TYPE`, a type

An optional comparison context type. If configured, the context type is passed as the first argument to all configured comparison expressions and accepted as the first argument of all generated functions.



## Automatic By Value

The following configuration parameter can be defined to declare that the element is a value type so that all lifecycle operations are automatic:

- `BY_VALUE`

This requires a `VALUE_TYPE` and declares that the element is automatically initialized, assignable by value for both moves and copies, and automatically destroyed. This is equivalent to defining all of the following:

```c
#define MOVE_BY_VALUE 1
#define DESTROY_BY_VALUE 1
#define INIT_BY_VALUE 1
#define INIT_COPY_BY_VALUE 1
#define COPY_BY_VALUE 1
#define INIT_STEAL_BY_VALUE 1
#define STEAL_BY_VALUE 1
#define SWAP_BY_VALUE 1
```

In C this implies that the type is a trivial bitwise copyable type. `move()`, `copy()`, `steal()`, `init_copy()`, `init_steal()` and `swap()` are all performed by simple assignment of the `VALUE_TYPE`, and `init()` and `destroy()` are no-ops.

In C++ this means that `VALUE_TYPE` is a "value type": all of its constructors and its destructor are valid and none are deleted (see [rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three).) Any trivial type (i.e. [`std::is_trivial`](https://en.cppreference.com/w/cpp/types/is_trivial)) is a "value type" as in C. A non-trivial example is `std::string`, but not `std::unique_ptr` because the latter is not copyable.

Use it for trivial types like `int`, for POD structs, for copyable and default-constructible C++ objects, or for pointer types as long as the container does not own the pointers. For pointers that the container owns, define `MOVE_BY_VALUE 1` and a `DESTROY` expression. If your C++ type is not copyable or default-constructible, don't use `BY_VALUE`; instead define explictly the `BY_VALUE` operations it allows.

You can override certain lifecycle behaviours while using this to specialize the functionality. For example suppose you are storing pointers in a vector. You could define `INIT` to zero-initialize the value:

```c
#define BY_VALUE 1
#define INIT(p) *p = 0
```

Just note that this overrides C++ constructors; see the C++ notes above.



## Move

Almost all dynamic containers (those that own their contained objects) must have a way to move elements. They require that at least one of the parameters in this section be defined.

### `MOVE`

- `MOVE`, expression matching `void(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from)`

Moves the given element from storage at `from` to storage at `to`. The value pointed `to` is uninitialized (but default-constructed), and the value pointed `from` must be left uninitialized (but constructed, as its destructor will be run).

Another name for this is "relocate".

### `MOVE_BY_VALUE`

- `MOVE_BY_VALUE`, a flag

Indicates that the type is movable by value. This is implied by `BY_VALUE`.

In C, this means `move()` is a simple assignment, or with `memcpy()` or equivalent for values in bulk.

For bitwise-movable C++ types, move is done with `memcpy()` or equivalent. For non-bitwise-movable C++ types, this will run a move constructor on the destination from the source followed by destructing the source.

By default, a C++ type is bitwise movable if it is [trivially copyable](https://en.cppreference.com/w/cpp/types/is_trivially_copyable). You can explicitly declare that a C++ type is bitwise movable by specializing `pottery::is_bitwise_movable` for your type. For example:

```c++
class Foo {
    // specialized constructors, making this type non-trivial
};
namespace pottery {
    // declare to Pottery that this can be moved with memcpy()
    template<> struct is_bitwise_movable<Foo> : std::true_type {};
}
```

In this case Pottery will use `memcpy()` to relocate `Foo` even though it is non-trivial.



## Destroy

If neither of the below are defined, the element is not destroyable. In this case `remove()`-style functions will not be defined for containers.

Furthermore, it is an error to destroy a non-empty dynamic container with non-destroyable elements (the container will assert() against this in debug mode.) You must manually empty such a container (by displacing all elements) before destroying it.

### `DESTROY`

- `DESTROY`, expression matching `void(CONTEXT_TYPE context, REF_TYPE element)`

Destroys the given element.

### `DESTROY_BY_VALUE`

- `DESTROY_BY_VALUE`, a flag

Indicates that the type is automatically destroyable. This is implied by `BY_VALUE`.

In C, this means `destroy()` does nothing. In C++ it runs the destructor.

In C++, if your destructor throws, Pottery aborts.



## Initialize

Allows initializing an element with empty, blank or default contents.

This doesn't necessarily mean the contents are valid. For example, for an `int`, this doesn't have to assign a value; it just becomes "initialized" to whatever garbage happened to be at that memory location.

- `INIT`, expression matching `pottery_error_t(CONTEXT_TYPE context, REF_TYPE element)`

Initializes the given element with empty, blank or default contents. The given value pointer is uninitialized; you must initialize it.

- `INIT_BY_VALUE`, a flag

Indicates that the type is automatically initializable. This is implied by `BY_VALUE`.

In C, this means `init()` does nothing. In C++ it runs the default constructor.



## Initialize copy

At most one of the following options may be defined:

### `INIT_COPY`

- `INIT_COPY`, expression matching `pottery_error_t(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from)`

Initializes an element at a new storage location, copying an existing element into it.

In C++, the default constructor will be run first, which means the element must have a default constructor. To use copy construction, define `INIT_COPY_BY_VALUE` instead.

### `INIT_COPY_BY_VALUE`

- `INIT_COPY_BY_VALUE`, a flag

Indicates that the type is initializable as a copy by value (bitwise for C types or by copy constructor for C++ types.) This is implied by `BY_VALUE`.



## Copy

At most one of the following assign parameters may be defined:

### `COPY`

- `COPY`, expression matching `pottery_error_t(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from)`

Copies the contents of an existing element into another existing value, replacing its contents. The values in `to` and `from` are already initialized and must remain initialized. In addition, `from` is `const` and must not be modified.

If this fails, it you must leave the destination object initialized, but it is up to you whether its original contents are preserved or whether it contains junk. The generated `copy()` will have the same behaviour.

### `COPY_BY_VALUE`

- `COPY_BY_VALUE`, a flag

Indicates that the type is assignable by value (bitwise for C types or by copy assignment for C++ types, as with `operator=(const&)`.) This is implied by `BY_VALUE`.



## Initialize steal

At most one of the following parameters may be defined:

### `INIT_STEAL`

- `INIT_STEAL`, expression matching `pottery_error_t(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from)`

Initializes an element at a new storage location, stealing an existing value into it.

In C++, the default constructor will be run first, which means the element must have a default constructor. To use move construction, define `INIT_STEAL_BY_VALUE` instead.

### `INIT_STEAL_BY_VALUE`

- `INIT_STEAL_BY_VALUE`, a flag

Indicates that the type is initializable as a steal by value (bitwise for C types or by steal constructor for C++ types.) This is implied by `BY_VALUE`.



## Steal

At most one of the following steal parameters may be defined:

### `STEAL`

- `STEAL`, expression matching `void(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from)`

Moves the contents of an existing element into another existing value, replacing its contents. The value stolen `from` is left initialized but blank. The values in `to` and `from` are already initialized and must remain initialized.

### `STEAL_BY_VALUE`

- `STEAL_BY_VALUE`, a flag

Indicates that the type is stealable by value (bitwise for C types or by move assignment for C++ types, as with `operator=(&&)`.) This is implied by `BY_VALUE`.

In C++, if your move assignment operator throws, Pottery aborts.



## Swap

You can define a custom swap expression:

### `SWAP`

- `SWAP`, expression matching `void(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

Swaps the two given elements.

If this is not defined and `MOVE` and `VALUE_TYPE` are provided, elements are swapped through a temporary using the `MOVE` expression.

Otherwise, if this is not defined and `MOVE_BY_VALUE` and `VALUE_TYPE` are defined: In C, or in C++ if `std::is_trivial<>::value` is true or `pottery::is_bitwise_movable<>::value` is true, elements are swapped bitwise. Otherwise (for non-trivial C++ types) elements are swapped with a `swap()` found by argument-dependent lookup or otherwise by `std::swap()`. 

### `SWAP_BY_VALUE`

- `SWAP_BY_VALUE`, a flag

Indicates that the type is swappable by value.

In C, this means `swap()` will happen by simple assignment or by `memcpy()` or equivalent.

For bitwise-movable C++ types, `swap()` is done with `memcpy()` or equivalent. For non-bitwise-movable C++ types, `swap()` will wrap your own `swap()` function found by argument-dependent lookup or `std::swap()`. If your `swap()` throws, Pottery aborts.



## Special Configuration Rules

The Lifecycle template will attempt to synthesize whatever operations it can from the provided expressions. For example if you define `SWAP` but not `STEAL`, it will synthesize `steal()` by swapping. If you define `MOVE` and `VALUE_TYPE` but not `SWAP`, it will synthesize `swap()` by moving through a temporary.

The Lifecycle template is usually instantiated by other templates. In order to allow them to detect what operations are available, the Lifecycle template always defines all of the following macros upon instantiation.

The fundamental operations:

- `CAN_DESTROY`
- `CAN_INIT`
- `CAN_INIT_COPY`
- `CAN_INIT_STEAL`
- `CAN_MOVE`
- `CAN_COPY`
- `CAN_STEAL`
- `CAN_SWAP`

Additional operations:

- `CAN_PASS`

These are 1 if the functionality is available or 0 otherwise. When these macros are no longer needed, you must include `pottery_lifecycle_cleanup.t.h" to clean them up (templates that use the lifecycle template do this for you.)



# API Reference

The following functions are defined if possible:

## Init

### `init()`

```c
pottery_error_t init(CONTEXT_TYPE context, REF_TYPE element);
```

Constructs and initializes the given element to a blank, empty, or default state.

If this succeeds, POTTERY_OK is returned; otherwise the element is not initialized and must not be destroyed.

### `init_bulk()`

```c
pottery_error_t init_bulk(CONTEXT_TYPE context, VALUE_TYPE* element, size_t n);
```

Constructs and initializes the given array of elements to a blank, empty, or default state.

If this succeeds, POTTERY_OK is returned; otherwise any successfully initialized elements in the destination array are destroyed, leaving the entire array uninitialized.

This requires that a `VALUE_TYPE` be defined.



## Destroy

The following functions are defined if possible:

### `destroy()`

```c
void destroy(CONTEXT_TYPE context, REF_TYPE element);
```

Destroys the given initialized element.

### `destroy_bulk()`

```c
void destroy_bulk(CONTEXT_TYPE context, VALUE_TYPE* element, size_t n);
```

Destroys the given array of initialized elements.

This requires that a `VALUE_TYPE` be defined.



## Init Copy

```c
pottery_error_t init_copy(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from);
```

Constructs and initializes an unconstructed element by copying the contents of an existing element.

The elements must be distinct. It is an error to pass the same element for both `to` and `from`.

If this succeeds, POTTERY_OK is returned; otherwise the element is not initialized and must not be destroyed.

```c
pottery_error_t init_copy_bulk(CONTEXT_TYPE context, VALUE_TYPE* to, VALUE_TYPE* from, size_t n);
```

Constructs and initializes a range of unconstructed elements by copying the contents of a non-overlapping range of existing elements.

The elements must be distinct. It is an error to pass overlapping ranges of elements.

If this succeeds, POTTERY_OK is returned; otherwise any successfully initialized elements in the destination array are destroyed, leaving the entire array uninitialized.

This requires that a `VALUE_TYPE` be defined.



## Init Steal

```c
pottery_error_t init_steal(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from);
```

Constructs and initializes an unconstructed element by stealing the contents of another existing element, leaving the `from` element initialized but with junk contents.

The elements must be distinct. It is an error to pass the same element for both `to` and `from`.

If this succeeds, POTTERY_OK is returned; otherwise the destination element is not initialized and must not be destroyed, while the source element is initialized but in an indeterminate (junk) state. The contents of the source object are not preserved on error.

```c
pottery_error_t init_steal_bulk(CONTEXT_TYPE context, VALUE_TYPE* to, VALUE_TYPE* from, size_t n);
```

Constructs and initializes a range of unconstructed elements by stealing the contents of a non-overlapping range of existing elements, leaving the `from` elements initialized but with junk contents.

The elements must be distinct. It is an error to pass overlapping ranges of elements.

If this succeeds, POTTERY_OK is returned; otherwise any successfully initialized elements in the destination array are destroyed leaving the entire array uninitialized, while the entire source array is initialized but in an indeterminate (junk) state. The contents of the source objects are not preserved on error.

This requires that a `VALUE_TYPE` be defined.



## Move

```c
void move(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from);
```

Moves the element to a new address, without creating or destroying any elements (if possible.) The `to` element must be uninitialized and unconstructed, and the `from` element will be left uninitialized and destructed.

The elements must be distinct. It is an error to pass the same element for both `to` and `from`.

```c
void move_bulk_restrict(CONTEXT_TYPE context, VALUE_TYPE* restrict to, VALUE_TYPE* restrict from, size_t n);
```

Moves `n` elements from non-overlapping addresses `from` to `to`.

This requires that a `VALUE_TYPE` be defined.

```c
void move_bulk_up(CONTEXT_TYPE context, VALUE_TYPE* to, VALUE_TYPE* from, size_t n);
```

Moves `n` elements up from `from` to `to`. The ranges may overlap as long as `to > from`.

This requires that a `VALUE_TYPE` be defined.

```c
void move_bulk_down(CONTEXT_TYPE context, VALUE_TYPE* to, VALUE_TYPE* from, size_t n);
```

Moves `n` elements down from `from` to `to`. The ranges may overlap as long as `to < from`.

This requires that a `VALUE_TYPE` be defined.

```c
void move_bulk(CONTEXT_TYPE context, VALUE_TYPE* to, VALUE_TYPE* from, size_t n);
```

Moves `n` elements from `from` to `to`. The ranges may overlap arbitrarily. (This may automatically call one of the other functions based on how the ranges overlap.)



## Copy

The following functions are defined if possible.

```c
pottery_error_t copy(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from);
```

Replaces the contents of `to` by copying the contents of `from`. Both elements must already be initialized.

The elements must be distinct. It is an error to pass the same element for both `to` and `from`.

If this succeeds, POTTERY_OK is returned. If this fails an error code is returned, and it is up to the configured `COPY` expression whether the destination object is unmodified or whether it contains junk. Either way, the destination is in an initialized state and must be destroyed.




## Steal

The following functions are defined if possible.

```c
void steal(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from);
```

Replaces the contents of `to` by replacing it with the contents of `from`, leaving `from` initialized with junk. Both elements must already be initialized.

The elements must be distinct. It is an error to pass the same element for both `to` and `from`.



## Swap

The following functions are defined if possible.

```c
void swap(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right);
```

Swaps two elements.

The elements must be distinct (as implied by `restrict`.)

```c
void swap_bulk_restrict(CONTEXT_TYPE context, VALUE_TYPE* restrict left, VALUE_TYPE* restrict right, size_t n);
```

Swaps two non-overlapping ranges of elements.

This requires that a `VALUE_TYPE` be defined.



## Pass

The lifecycle template defines `POTTERY_LIFECYCLE_CAN_PASS` if the type can be passed by value. This allows containers to define `insert()` and `extract()` functions that accept the type as an argument or return it.

Without the ability to pass types by value, you must insert them with a method like `emplace()`. This creates an uninitialized entry in which you initialize the value.

Currently this is only allowed if the type can be both copied by value and destroyed by value (in other words it is trivial in C and copy-constructible and destructible in C++.) This is fairly restrictive so it may be relaxed later if I can figure out a good way to make it work.

For example with a vector of allocated `char*` it would be nice to pass `strdup()` straight to `insert()`. However such a type is neither copy-by-value nor destroy-by-value; the vector needs to make sure it is carefully moving it into place and needs to properly dispose of it if the insertion fails. Ideally I'd like to make `CAN_PASS` depend on move-by-value and destroyable but this is not yet implemented and may not be possible.
