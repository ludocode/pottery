# Pottery Lifecycle Template

This is a helper template for other Pottery containers. It is not intended to be used directly.

The Pottery Lifecycle template defines functions that wrap the arbitrary lifecycle expressions you define for your value types stored in Pottery data structures.

The instantiated lifecycle functions follow [Pottery-style lifecycle semantics and naming conventions](../../../docs/lifecycle_style.md). Most Pottery containers and algorithms use this template internally to store or manipulate values. These are also summarized in the [glossary](../../../docs/glossary.md).

The Lifecycle template instantiates functions for as many of the following eight fundamental lifecycle behaviours as possible:

- `init()`
- `init_copy()`
- `init_steal()`
- `copy()`
- `steal()`
- `move()`
- `swap()`
- `destroy()`

The first four of these are allowed to fail; they return an error code result. The last four cannot fail and return void. If an `init*()` function fails, the destination value is not initialized and must not be destroyed.

In addition, the template instantiates various functions to operate in bulk (with a `_bulk` suffix), various functions to operate on restrict values (with a `_restrict` suffix), and combinations of these. For example call `move_bulk_restrict()` to move a C array of values to another storage location when the locations do not overlap.

### Reference Type

Unlike most other Pottery container algorithms, the Lifecycle template operates directly on the ref type, not the entry type. Lifecycle functions and configured lifecycle expressions take ref types as arguments.

This is mainly because lifecycle functions need to be able to operate on temporary values (among other things.) When `VALUE_TYPE` is defined, lifecycle functions operate directly on value pointers, and any `ENTRY_TYPE` is ignored. This means algorithms that use the lifecycle template (like sort algorithms) must dereference entries themselves before calling into the lifecycle template.



### Context

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

All below configuration options are prefixed by the configuration prefix of the template being instantiated plus `LIFECYCLE`. For example on [`vector`](../vector/), `BY_VALUE` is `POTTERY_VECTOR_LIFECYCLE_BY_VALUE`. When using lifecycle directly, the prefix is just `POTTERY_LIFECYCLE`.



## Types

### `REF_TYPE`

This is the abstract reference type for which lifecycle functions are being generated. Values of this type are passed to all configured lifecycle expressions and acceepted by all generated functions (after the optional context.)

This is usually either a pointer to the real type allowing lifecycle operations on values in memory, or an abstract identifier to be used with the context to identify it. For example the context may be a database connection and the type may be the key for a row: in this case the `COPY` expression would copy data from one row to another.

Note that `REF_TYPE` and `VALUE_TYPE` are mutually exclusive: you can only define one or the other.

See the [glossary](../../../docs/glossary.md) for more information on the ref type.

### `VALUE_TYPE`

This is a concrete value type for performing lifecycle operations on values in memory. This is required for any of the `BY_VALUE` configuration options and for bulk operations on C arrays of values.

If this is defined, `REF_TYPE` will be defined as a pointer to this. You cannot define both `VALUE_TYPE` and `REF_TYPE`.

See the [glossary](../../../docs/glossary.md) for more information on the value type.

### `CONTEXT_TYPE`

An optional comparison context type. If configured, the context type is passed as the first argument to all configured comparison expressions and accepted as the first argument of all generated functions.



## Automatic By Value

The following configuration parameter can be defined to declare that all lifecycle operations are automatic:

### `BY_VALUE`

A flag indicating that the value is automatically initialized and destroyed, and assignable by value for both moves and copies. Defining this to 1 has the same behaviour as defining all of the following to 1:

- `MOVE_BY_VALUE`
- `DESTROY_BY_VALUE`
- `INIT_BY_VALUE`
- `INIT_COPY_BY_VALUE`
- `COPY_BY_VALUE`
- `INIT_STEAL_BY_VALUE`
- `STEAL_BY_VALUE`
- `SWAP_BY_VALUE`

In C this implies that the type is a trivial bitwise copyable type. `move()`, `copy()`, `steal()`, `init_copy()`, `init_steal()` and `swap()` are all performed by simple assignment of the `VALUE_TYPE`, and `init()` and `destroy()` are no-ops.

In C++ this means that `VALUE_TYPE` is a "value type": all of its constructors and its destructor are valid and none are deleted (see [rule of three/five/zero](https://en.cppreference.com/w/cpp/language/rule_of_three).) Any trivial type (i.e. [`std::is_trivial`](https://en.cppreference.com/w/cpp/types/is_trivial)) is a "value type" as in C. A non-trivial example is `std::string`, but not `std::unique_ptr` because the latter is not copyable.

Use it for trivial types like `int`, for POD structs, for copyable and default-constructible C++ objects, or for pointer types as long as the container does not own the pointers. For pointers that the container owns, define `MOVE_BY_VALUE 1` and a `DESTROY` expression. If your C++ type is not copyable or default-constructible, don't use `BY_VALUE`; instead define explictly the `BY_VALUE` operations it allows.

#### Overriding `BY_VALUE`

You can override certain lifecycle behaviours while using this to specialize the functionality. For example suppose you are storing unowned pointers in a container, so you don't need special behaviour for copy, destroy, etc. You could define `INIT` to zero-initialize the value and `BY_VALUE` for everything else:

```c
#define POTTERY_LIFECYCLE_BY_VALUE 1
#define POTTERY_LIFECYCLE_INIT(p) *p = 0
```

Just note that this overrides C++ constructors; see the C++ notes above.



## Move

Most dynamic containers (those that own their contained objects) require a way to move values. They require that at least one of the parameters in this section be defined.

### `MOVE`

An expression matching `void(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from)`.

Moves the given value from storage at `from` to storage at `to`. The value pointed `to` is uninitialized (but default-constructed), and the value pointed `from` must be left uninitialized (but constructed, as its destructor will be run).

Another name for this is "relocate".

Pottery will never call this with equal refs. You do not have to check whether the refs refer to the same value.

### `MOVE_BY_VALUE`

A flag indicating that the type is movable by value. This is implied by `BY_VALUE`.

In C, this means `move()` is a simple assignment, or with `memcpy()` or equivalent for arrays of values.

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

If neither of the below are defined, the value is not destroyable. In this case `remove()`-style functions will not be defined for containers.

Furthermore, it is an error to destroy a non-empty dynamic container with non-destroyable values (the container will assert() against this in debug mode.) You must manually empty such a container (by displacing all values) before destroying it.

### `DESTROY`

An expression matching `void(CONTEXT_TYPE context, REF_TYPE ref)`.

Destroys the given value.

### `DESTROY_BY_VALUE`

A flag indicating that the type is automatically destroyable. This is implied by `BY_VALUE`.

In C, this means `destroy()` does nothing. In C++ it runs the destructor.

In C++, if your destructor throws, Pottery aborts.



## Initialize

Allows initializing a value with empty, blank or default contents.

This doesn't necessarily mean the contents are valid. For example, for an `int`, this doesn't have to assign a value; it just becomes "initialized" to whatever garbage happened to be at that memory location.

### `INIT`

An expression matching `pottery_error_t(CONTEXT_TYPE context, REF_TYPE ref)`.

Initializes the given value with empty, blank or default contents. The given value pointer is uninitialized; you must initialize it.

### `INIT_BY_VALUE`

A flag indicating that the type is automatically initializable. This is implied by `BY_VALUE`.

In C, this means `init()` does nothing. In C++ it runs the default constructor.



## Initialize copy

At most one of the following options may be defined:

### `INIT_COPY`

An expression matching `pottery_error_t(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from)`.

Initializes a value at a new storage location, copying an existing value into it.

In C++, the default constructor will be run first, which means the value must have a default constructor. To use copy construction, define `INIT_COPY_BY_VALUE` instead.

Pottery will never call this with equal refs. You do not have to check whether the refs refer to the same value. (It is nonsensical for them to refer to the same value because _from_ must be initialized and _to_ must not be.)

### `INIT_COPY_BY_VALUE`

A flag indicating that the type is initializable as a copy by value (bitwise for C types or by copy constructor for C++ types.) This is implied by `BY_VALUE`.



## Copy

At most one of the following assign parameters may be defined:

### `COPY`

An expression matching `pottery_error_t(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from)`.

Copies the contents of an existing value into another existing value, replacing its contents. The values in `to` and `from` are already initialized and must remain initialized. In addition, `from` is `const` and must not be modified.

If this fails, it you must leave the destination value initialized, but it is up to you whether its original contents are preserved or whether it contains junk. The generated `copy()` will have the same behaviour.

Pottery will never call this with equal refs. You do not have to check whether the refs refer to the same value.

### `COPY_BY_VALUE`

A flag indicating that the type is assignable by value (bitwise for C types or by copy assignment for C++ types, as with `operator=(const&)`.) This is implied by `BY_VALUE`.



## Initialize steal

At most one of the following parameters may be defined:

### `INIT_STEAL`

An expression matching `pottery_error_t(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from)`.

Initializes a value at a new storage location, stealing an existing value into it.

In C++, the default constructor will be run first, which means the value must have a default constructor. To use move construction, define `INIT_STEAL_BY_VALUE` instead.

Pottery will never call this with equal refs. You do not have to check whether the refs refer to the same value. (It is nonsensical for them to refer to the same value because _from_ must be initialized and _to_ must not be.)

### `INIT_STEAL_BY_VALUE`

A flag indicating that the type is initializable as a steal by value (bitwise for C types or by steal constructor for C++ types.) This is implied by `BY_VALUE`.



## Steal

At most one of the following steal parameters may be defined:

### `STEAL`

An expression matching `void(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from)`.

Moves the contents of an existing value into another existing value, replacing its contents. The value stolen `from` is left initialized but blank. The values in `to` and `from` are already initialized and must remain initialized.

Pottery will never call this with equal refs. You do not have to check whether the refs refer to the same value.

### `STEAL_BY_VALUE`

A flag indicating that the type is stealable by value (bitwise for C types or by move assignment for C++ types, as with `operator=(&&)`.) This is implied by `BY_VALUE`.

In C++, if your move assignment operator throws, Pottery aborts.



## Swap

You can define a custom swap expression:

### `SWAP`

An expression matching `void(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`.

Swaps the two given values.

If this is not defined and `MOVE` and `VALUE_TYPE` are provided, values are swapped through a temporary using the `MOVE` expression.

Otherwise, if this is not defined and `MOVE_BY_VALUE` and `VALUE_TYPE` are defined: In C, or in C++ if `std::is_trivial<>::value` is true or `pottery::is_bitwise_movable<>::value` is true, values are swapped bitwise. Otherwise (for non-trivial C++ types) values are swapped with a `swap()` found by argument-dependent lookup or otherwise by `std::swap()`. 

Pottery will never call this with equal refs. You do not have to check whether the refs refer to the same value.

### `SWAP_BY_VALUE`

A flag indicating that the type is swappable by value.

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

These are 1 if the functionality is available or 0 otherwise. When these macros are no longer needed, you must include `pottery_lifecycle_cleanup.t.h` to clean them up. Other templates that use the lifecycle template do this internally so they do not expose any of these macros.



## Allocated Pointers

The lifecycle template does not define `new()` or `delete()` functions. Pottery's intrusive containers deal with externally allocated and externally owned values so they do not perform any lifecycle operations on them. All other Pottery containers and algorithms deal directly with flat values.

If you are putting allocated pointers in a Pottery container and you want the container to be able to manage them, you can make the value type a pointer, and make the lifecycle init/destroy functions wrap your new/delete functions (since they init and destroy the pointer.) A minimal wrapping for a vector of pointers might look like this:

```c
#define POTTERY_LIFECYCLE_VALUE_TYPE foo_t*
#define POTTERY_LIFECYCLE_DESTROY(x) foo_delete(*x)
#define POTTERY_LIFECYCLE_MOVE_BY_VALUE 1
```

This will instantiate `destroy()`, `move()` and `swap()`, which is all you need to put allocated pointers in a container with nice `insert()` and `remove()` APIs. Note that this is `MOVE_BY_VALUE`, not `BY_VALUE`: it can move our pointers but we don't want it to assume it can trivially copy them!

If your pointer type supports all lifecycle operations, you can forward them all to Pottery. You do have to correctly return errors from your expressions though. This is easy if your `new()` functions can't fail (for example if you are ignoring `malloc()` errors since `malloc()` doesn't fail gracefully on modern desktop platforms.) You might wrap all lifecycle operations for a pointer value like this:

```c
#define POTTERY_LIFECYCLE_VALUE_TYPE foo_t*
#define POTTERY_LIFECYCLE_INIT(x) *x = foo_new(), POTTERY_OK
#define POTTERY_LIFECYCLE_INIT_COPY(x, y) *x = foo_new_copy(*y), POTTERY_OK
#define POTTERY_LIFECYCLE_INIT_STEAL(x, y) *x = foo_new_steal(*y), POTTERY_OK
#define POTTERY_LIFECYCLE_COPY(x, y) foo_copy(*x, *y), POTTERY_OK
#define POTTERY_LIFECYCLE_STEAL(x, y) foo_steal(*x, *y)
#define POTTERY_LIFECYCLE_DESTROY(x) foo_delete(*x)
#define POTTERY_LIFECYCLE_BY_VALUE 1 // for move and swap
```

Of course you only need to define those that are actually relevant for your container.



# API Reference

The following functions are defined if possible:

## Init

```c
pottery_error_t init(CONTEXT_TYPE context, REF_TYPE value);
```

Constructs and initializes the given value to a blank, empty, or default state.

If this succeeds, POTTERY_OK is returned; otherwise the value is not initialized and must not be destroyed.

```c
pottery_error_t init_bulk(CONTEXT_TYPE context, VALUE_TYPE* values, size_t n);
```

Constructs and initializes the given array of values to a blank, empty, or default state.

If this succeeds, POTTERY_OK is returned; otherwise any successfully initialized values in the destination array are destroyed, leaving the entire array uninitialized.

This requires that a `VALUE_TYPE` be defined.



## Destroy

The following functions are defined if possible:

```c
void destroy(CONTEXT_TYPE context, REF_TYPE ref);
```

Destroys the given initialized value.

```c
void destroy_bulk(CONTEXT_TYPE context, VALUE_TYPE* values, size_t n);
```

Destroys the given array of initialized values.

This requires that a `VALUE_TYPE` be defined.



## Init Copy

```c
pottery_error_t init_copy(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from);
```

Constructs and initializes an unconstructed value by copying the contents of an existing value.

The values must be distinct. It is an error to pass the same value for both `to` and `from`.

If this succeeds, POTTERY_OK is returned; otherwise the value is not initialized and must not be destroyed.

```c
pottery_error_t init_copy_bulk(CONTEXT_TYPE context, VALUE_TYPE* to, VALUE_TYPE* from, size_t n);
```

Constructs and initializes a range of unconstructed values by copying the contents of a non-overlapping range of existing values.

The values must be distinct. It is an error to pass overlapping ranges of values.

If this succeeds, POTTERY_OK is returned; otherwise any successfully initialized values in the destination array are destroyed, leaving the entire array uninitialized.

This requires that a `VALUE_TYPE` be defined.



## Init Steal

```c
pottery_error_t init_steal(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from);
```

Constructs and initializes an unconstructed value by stealing the contents of another existing value, leaving the `from` value initialized but with junk contents.

The values must be distinct. It is an error to pass the same value for both `to` and `from`.

If this succeeds, POTTERY_OK is returned; otherwise the destination value is not initialized and must not be destroyed, while the source value is initialized but in an indeterminate (junk) state. The contents of the source value are not preserved on error.

```c
pottery_error_t init_steal_bulk(CONTEXT_TYPE context, VALUE_TYPE* to, VALUE_TYPE* from, size_t n);
```

Constructs and initializes a range of unconstructed values by stealing the contents of a non-overlapping range of existing values, leaving the `from` values initialized but with junk contents.

The values must be distinct. It is an error to pass overlapping ranges of values.

If this succeeds, POTTERY_OK is returned; otherwise any successfully initialized values in the destination array are destroyed leaving the entire array uninitialized, while the entire source array is initialized but in an indeterminate (junk) state. The contents of the source values are not preserved on error.

This requires that a `VALUE_TYPE` be defined.



## Move

```c
void move(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from);
```

Moves the value to a new address, without creating or destroying any values (if possible.) The `to` value must be uninitialized and unconstructed, and the `from` value will be left uninitialized and destructed.

The values must be distinct. It is an error to pass the same value for both `to` and `from`.

```c
void move_bulk_restrict(CONTEXT_TYPE context, VALUE_TYPE* restrict to, VALUE_TYPE* restrict from, size_t n);
```

Moves `n` values from non-overlapping addresses `from` to `to`.

This requires that a `VALUE_TYPE` be defined.

```c
void move_bulk_up(CONTEXT_TYPE context, VALUE_TYPE* to, VALUE_TYPE* from, size_t n);
```

Moves `n` values up from `from` to `to`. The ranges may overlap as long as `to > from`.

This requires that a `VALUE_TYPE` be defined.

```c
void move_bulk_down(CONTEXT_TYPE context, VALUE_TYPE* to, VALUE_TYPE* from, size_t n);
```

Moves `n` values down from `from` to `to`. The ranges may overlap as long as `to < from`.

This requires that a `VALUE_TYPE` be defined.

```c
void move_bulk(CONTEXT_TYPE context, VALUE_TYPE* to, VALUE_TYPE* from, size_t n);
```

Moves `n` values from `from` to `to`. The ranges may overlap arbitrarily. (This may automatically call one of the other functions based on how the ranges overlap.)



## Copy

The following functions are defined if possible.

```c
pottery_error_t copy(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from);
```

Replaces the contents of `to` by copying the contents of `from`. Both values must already be initialized.

The values must be distinct. It is an error to pass the same value for both `to` and `from`.

If this succeeds, POTTERY_OK is returned. If this fails an error code is returned, and it is up to the configured `COPY` expression whether the destination value is unmodified or whether it contains junk. Either way, the destination is in an initialized state and must be destroyed.




## Steal

The following functions are defined if possible.

```c
void steal(CONTEXT_TYPE context, REF_TYPE to, REF_TYPE from);
```

Replaces the contents of `to` by replacing it with the contents of `from`, leaving `from` initialized with junk. Both values must already be initialized.

The values must be distinct. It is an error to pass the same value for both `to` and `from`.



## Swap

The following functions are defined if possible.

```c
void swap(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right);
```

Swaps two values.

The values must be distinct (as implied by `restrict`.)

```c
void swap_bulk_restrict(CONTEXT_TYPE context, VALUE_TYPE* restrict left, VALUE_TYPE* restrict right, size_t n);
```

Swaps two non-overlapping ranges of values.

This requires that a `VALUE_TYPE` be defined.



## Pass

If a value type can be moved by value (i.e. `BY_VALUE` or `MOVE_BY_VALUE` is 1), passing is enabled. This allows containers to define `insert()` and `extract()` functions that accept the type as an argument or return it.

Without the ability to pass types by value, you must insert them yourself with `emplace()`. This creates an uninitialized entry in which you initialize a value or to which you move one.
