# Pottery-Style Object Lifecycle Functions in Modern C

This document describes a set of style, semantics and naming conventions for object lifecycle management in modern C. It's a way to manually manage the initialized lifetimes of objects, as well as move them around, copy them, etc.

For example, an object type `foo_t` is initialized with `foo_init()`, destroyed by `foo_destroy()`, copied with `foo_copy()`, moved with `foo_move()` and so forth. These functions and more are described herein. This style is used by all Pottery object types, as well as all object types in [HomeFort](https://homefort.app/) and its other libraries.

(This document should not be confused with the Pottery [lifecycle template](../include/pottery/lifecycle/) which instantiates wrapper functions in this style for arbitrary types.)

You can follow the same naming conventions for your own C types if you like this style. This might make it easier to put them in Pottery containers. For example many containers need a way to move their contents when they resize their internal storage, so it might make sense for you to implement a `move()` function for your object.

You could call this style "Pottery-style lifecycle functions". Alternatively you could call it "rule of eight", as an analogue to the C++ [rule of three](https://en.cppreference.com/w/cpp/language/rule_of_three) since this describes eight fundamental lifecycle operations:

- `destroy()` or `delete()`
- `init()` or `new()`
- `init_copy()` or `new_copy()`
- `init_steal()` or `new_steal()`
- `copy()`
- `steal()`
- `move()`
- `swap()`

In case you're familiar with C++, this document links to the C++ analogue of many functions to help you understand how they work.

As an example to follow along, we'll define an "employee" object and implement all of these lifecycle functions for it. This will allow us to unlock the full functionality of Pottery containers in which we store it. The full code is in the Pottery [lifecycle style example](../examples/pottery/lifecycle_style/) which has a lot of additional code comments and implementation variations.

This document assumes a style typical of modern C: our "employee" object type will be defined in files `employee.h` and `employee.c` and will be named `employee_t`. Functions operating on it will start with `employee_` and some of them may be defined inline in the header file. This document is meant only to cover the style of lifecycle functions, not to be a general tutorial on modern C.

All Pottery containers implement as much of this style as they can given their configuration. For example if you provide a `LIFECYCLE_INIT_COPY` expression for your value type when instantiating a vector, then the whole vector will be copyable with `_copy()` and `_init_copy()`. If you define `LIFECYCLE_BY_VALUE`, then all eight functions will be available for your vector. See the [lifecycle template](../include/pottery/lifecycle/) for more details.



## Vocabulary

We use the name "object type" to refer to a custom data type in C that requires explicit manual lifecycle management following the rules laid out in this document.

We use the word "object" to refer to initialized objects of such a type.

We use the word "storage" to refer to the underlying value or memory for such a type, which may or may not be initialized.

We use the term "value type" to mean an object for which the user can allocate storage. The object can be placed on the stack, or by value as a field in another struct. This doesn't mean the object can be passed by value!

Some people might use the term "object-oriented programming" to describe this style. A more useful definition of object-oriented programming would be polymorphic or hierarchichal types, which this definitely is not. This just defines the lifecycle of elements which we call "objects"; we use this word without worrying much about object-orientation.



## Type Definition

Object types are named in all lowercase with spaces separating words. All identifiers defined for this object start with this name.

The structure itself is named with the object name suffixed by `_t`. (Technically POSIX reserves all identifiers ending in `_t`. We ignore this rule.) We define the struct with this name, and we typedef it to the same name:

```c
typedef struct employee_t employee_t;
```

Partly this is to guarantee compatibility with C++ which doesn't require the struct tag to refer to a struct. More importantly, it helps with forward-declarations of this struct in the implementations of other objects that use it. They can use the `struct` keyword with a forward-declaration, i.e. `struct employee_t;` in their header, and then strip the `struct` keyword in their implementation file once they've included our employee header.

As in typical C, you can define the structure in the header or the source, with various tradeoffs. For our `employee_t`, we define the structure in the header to allow users and Pottery containers to store it by value. To demonstrate how to implement lifecycle functions with a variety of field types, we'll store each string with a different type. We also store the employee number as an int to demonstrate a trivial field.

```c
struct employee_t {
    string_t first_name;  // Pottery's example string container
    char* last_name;      // An allocated C string, never null
    char start_date[32];  // A fixed-size string buffer
    int number;           // A trivial value type
}
```

The `string_t` above is in the Pottery examples [here](../examples/pottery/string/). It follows all of the conventions in this document so you can use it as another example.



## Naming Rules

Any function that initializes the object must have `init` in the name, and any function that allocates a new object must have `new` in the name. This is critical to make the code auditable. It should be easy to review the code to pair up calls to `init*()/destroy()` and `new*()/delete()` without worrying about alternative names like `make()` or `dup()`.

With objects in Pottery containers, this gets a bit more complicated because emplacement can trade object ownership. `init()` or `new()` can actually be paired with `emplace()`, giving ownership to the container; and `displace()` can be paired with `destroy()` or `delete()`, as we have taken ownership from the container. This added complexity is why consistency in naming is so important.



## Fundamental Lifecycle Functions



### Initialization

An `init()` function initializes the object to a blank, empty, or default state.

```c
bool employee_init(employee_t* employee) {
    string_init(&employee->first_name);
    employee->last_name = strdup("");
    employee->start_date[0] = '\0';
    employee->number = -1;
    return true;
}
```

Additional initialization functions can be created to initialize with various arguments. For this we add an additional word after `init`. For example we might define the following function to initialize our employee with given elements:

```c
bool employee_init_elements(employee_t* employee, const char* first_name,
        const char* last_name, const char* start_date, int number)
{
    string_init_cstr(&employee->first_name, first_name);
    employee->last_name = strdup(last_name);

    if (strlen(start_date) > sizeof(employee->start_date) - 1)
        abort();
    strcpy(employee->start_date, start_date);

    employee->number = number;

    return true;
}
```

Sometimes it doesn't make sense to initialize an object into an empty state. This might be called a "zombie" object, and you don't have to support this. In this case you can take arguments in `init()`, or just don't implement a blank `init()`. Since we have an `employee_init_elements()` function, we could have omitted `employee_init()`.

Initialization for your object type may be able to fail. In this case your `init` functions should return an error code. Here we return `bool` to indicate success or failure to demonstrate that `init` may in general fail, however we don't currently bother to support any failure modes so we just return `true`. (Pottery containers return an `int` (`pottery_error_t`) for any operation that may fail, and the success value `POTTERY_OK` is 0.)

You should only expose an `init()` function if your type is a value type, i.e. the structure is defined in the header. If not, you should support only allocation and de-allocation below.



### De-initialization

The `destroy()` function de-initializes an object.

```c
void employee_destroy(employee_t* employee) {
    string_destroy(&employee->first_name);
    free(employee->last_name);
}
```

You should only expose a `destroy()` function if your type is a value type, i.e. the structure is defined in the header. If not, you should support only allocation and de-allocation below.

`destroy()` can never fail.

Note that initialization and de-initialization can differ from what C++ calls construction and destruction. An object value that has been de-initialized with `destroy()` is not technically _destroyed_ as far as the C++ compiler is concerned; if it has a destructor, it will still run.

When placing a C++ type in a Pottery container, the container does not run constructors and destructors separately from your `init()` and `destroy()` functions. If you declare that your type can `INIT_BY_VALUE` and `DESTROY_BY_VALUE`, then the container [lifecycle template](../include/pottery/lifecycle/) generates `init()` functions that run your constructors and a `destroy()` function that runs your destructor.

If you don't care about C++ compatibility, you don't have to worry about any of that.



### Copy

`copy()` is used to copy the contents of one object into another. The contents of the source are untouched, and the contents of the target are replaced. This is like [copy assignment in C++](https://en.cppreference.com/w/cpp/language/copy_assignment).

```c
bool employee_copy(employee_t* to, const employee_t* from) {
    string_copy(&to->first_name, &from->first_name);

    free(to->last_name);
    to->last_name = strdup(from->last_name);

    strcpy(to->start_date, from->start_date);
    to->number = from->number;

    return true;
}
```

Note that the source object is `const`.



### Copy Initialization/Allocation

`init_copy()` initializes an object by copying the contents of an existing object.

```c
bool employee_init_copy(employee_t* to, const employee_t* from) {
    string_init_copy(&to->first_name, &from->first_name);
    to->last_name = strdup(from->last_name);
    strcpy(to->start_date, from->start_date);
    to->number = from->number;
    return true;
}
```

As with `copy()`, the source object is `const`.



### Move

`move()` means to move the object from one address to another in memory. This does not initialize a new object or de-initialize an old one; it is simply moved to a new address:

```c
void employee_move(employee_t* to, employee_t* from) {
    string_move(&to->first_name, &from->first_name);
    to->last_name = from->last_name;
    strcpy(to->start_date, from->start_date);
    to->number = from->number;
}
```

`move()` should not be implemented for allocated types. In other words if you define `new()`/`delete()` and not `init()`/`destroy()`, the object is responsible for its own allocation, so it should not allow its users to move it.

What we call "move" has no standard C++ equivalent. It's not currently possible to move the storage for an object without creating a new object in standard C++.

The closest equivalent in C++ would be to use move construction to create a new object that steals the source object's internals, and then to destroy the source object. In Pottery lifecycle terms this is `init_steal()` followed by `destroy()`. This is the process used by the lifecycle template to implement `move()` if you define `MOVE_BY_VALUE` to 1 for non-trivial C++ value types.

This C++ operation has been called "relocate" by some, and there is an effort to introduce the property "trivially relocatable" to allow C++ objects to be moved bitwise. Pottery defines `pottery::is_bitwise_movable` which you can specialize to allow it to move your C++ objects with `memcpy()`/`memmove()`.

See [this FAQ](https://quuxplusone.github.io/blog/2018/10/04/trivially-relocatable-faq/) about [this proposal](http://open-std.org/JTC1/SC22/WG21/docs/papers/2018/p1144r0.html) for more C++ information.



### Steal

`steal()` causes the target to steal the contents of the source object. Both objects need to be already initialized, and they are both still initialized afterwards, but the source object is left with junk. This is like [move assignment in C++](https://en.cppreference.com/w/cpp/language/move_assignment).

```c
void employee_steal(employee_t* to, employee_t* from) {
    string_steal(&to->first_name, &from->first_name);

    char* last_name = to->last_name;
    to->last_name = from->last_name;
    from->last_name = last_name;

    strcpy(to->start_date, from->start_date);
    to->number = from->number;
}
```

Note in particular that we don't guarantee that the source object is clear. It contains arbitrary junk, but it is still initialized and must be de-initialized. We can swap contents (as we do with `last_name` above), or we can leave the source object with its original data (as we do with `start_date` and `number`), or we can clear it. It doesn't matter what we leave it with; it just has to be valid.



### Steal Initialization/Allocation

Steal can be combined with initialization. `init_steal()` is used to initialize an object by stealing the contents of another, leaving it with junk. This is like [move construction in C++](https://en.cppreference.com/w/cpp/language/move_constructor).

```c
bool employee_init_steal(employee_t* to, employee_t* from) {
    string_init_steal(&to->first_name, &from->first_name);
    to->last_name = from->last_name;
    from->last_name = strdup("");
    strcpy(to->start_date, from->start_date);
    to->number = from->number;
    return true;
}
```

The source object is still initialized and must be de-initialized.



### Swap

`swap()` swaps the storage of two objects.

One way to implement this is to swap each of the fields. Alternatively, it can be implemented by wrapping our `move()` function:

```c
void employee_swap(employee_t* to, employee_t* from) {
    employee_t temp;
    employee_move(&temp, to);
    employee_move(to, from);
    employee_move(from, &temp);
}
```

Note that this is better than C++-style `std::swap()` because we did not have to initialize and de-initialize the temporary. This would have incurred an unnecessary `strdup("")` and `free()`, as well as many unnecessary assignments to keep it valid through each move.

This is similar to how Pottery swaps (non-C++) objects if you provide only `move()` and not `swap()`. If you implement `move()`, you don't necessarily need to implement `swap()` since users can easily do it themselves. On the other hand if your type is allocated you shouldn't implement `move()` but it can be useful to implement `swap()`.



## New and Delete

Some objects provide allocation and de-allocation functions instead of or in addition to initialization and de-initialization functions. This is required if your type is not a value type, i.e. its structure is not defined in the header. These functions should be called `new()` and `delete()`, as well as `new_copy()` and `new_steal()`.

Here's how we might implement them as wrappers to `init()` and `destroy()`:

```c
employee_t* employee_new(void) {
    employee_t* employee = (employee_t*) malloc(sizeof(employee_t));
    employee_init(employee);
    return employee;
}

employee_t* employee_new_copy(const employee_t* employee) {
    employee_t* employee = (employee_t*) malloc(sizeof(employee_t));
    employee_init_copy(employee);
    return employee;
}

employee_t* employee_new_steal(employee_t* employee) {
    employee_t* employee = (employee_t*) malloc(sizeof(employee_t));
    employee_init_steal(employee);
    return employee;
}

void employee_delete(employee_t* employee) {
    employee_destroy(employee);
    free(employee);
}
```

Note that we don't support deleting `NULL`. You can make this a no-op in your `delete()` function if you like, but Pottery does not. Every `delete()` should be paired with a `new()`; if `delete()` is called on `NULL`, it implies that something wasn't allocated properly and there is a bug. If you actually have an optional pointer where `NULL` represents no object, you should check it before calling `delete()`. Allowing a no-op `delete()` just encourages sloppy coding (and it's also marginally less efficient since you have to actually check for `NULL`.)

Note also that we cast the return value of `malloc()` to maintain compatibility with C++. You don't have to do this if you don't care about C++.

If you're going to implement both `init` and `new` for a value type, you should typically also implement a `new` wrapper function for each corresponding `init` function. This can get pretty repetitive.

```c
employee_t* employee_new_elements(const char* first_name, const char* last_name,
        const char* start_date, int number)
{
    employee_t* employee = (employee_t*) malloc(sizeof(employee_t));
    employee_init_elements(employee, first_name, last_name, start_date, number);
    return employee;
}
```

For most objects, you don't need to support both; you can implement either `init()/destroy()` for value types (where your struct is defined in the header) or `new()/delete()` for non-value types (where your struct is defined in the source.) This can prevent callers from pairing them incorrectly (e.g. calling `destroy()` on an object created with `new()`, thus leaking it.)

All Pottery containers and our employee example type are value types so they don't implement `new()` or `delete()`. For an example allocated type that hides its structure, see [String Set](../examples/pottery/string_set/).
