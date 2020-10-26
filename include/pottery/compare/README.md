# Compare

This is a helper template for other Pottery containers. It is not intended to be used directly.

Given a minimal set of configured comparison expressions, the compare template generates as many relational comparison functions as possible:

- `equal()`
- `not_equal()`
- `less()`
- `less_or_equal()`
- `greater()`
- `greater_or_equal()`
- `three_way()` -- a [three-way comparison](https://en.wikipedia.org/wiki/Three-way_comparison)

The functions are only defined if it is possible to generate them, so for example if only `EQUAL` is configured, ordering functions like `less()` won't exist. To generate all of them, the template needs to be configured with at least one ordering expression (i.e. `THREE_WAY`, `LESS`, `GREATER`, `LESS_OR_EQUAL`, or `GREATER_OR_EQUAL`.)

The template also defines additional comparison helpers, available if any single ordering expression is configured:

- `min()` -- the lesser of two values
- `max()` -- the greater of two values
- `clamp()` -- the given value limited by min and max values
- `median()` -- the median of three values

The compare template operates on an abstract reference type with an optional context. In the simplest case, the reference type is simply a pointer to an element in memory, and the comparison expressions need no context to compare them. In more advanced usage, the context could contain for example a database connection, and the reference type could be the key type for rows to be compared.

This template does not distinguish between "weak" and "strong" ordering. Whether "equal" implies equivalence or equality is specific to the configured comparison expressions and the context in which they are used.

This template currently does not support partial ordering. This means you cannot use it for example to sort an array of floats where some are NaN. Functions will be implemented with their opposites where necessary under the assumptions of symmetry and total ordering. For example if you define only LESS, you'll get `less()`, its simple negation `greater_or_equal()`,  its symmetric equivalent `greater()` with swapped arguments, etc.



## Example

The Compare template is meant as a helper for other Pottery templates. This template allows you to configure any Pottery templates with any one ordering expression (like `LESS`, `LESS_OR_EQUAL`, `THREE_WAY`, etc.) and it gives all comparison functions that templates may need, so they don't have to worry about negating the expression, swapping arguments, etc. Similarly the [`quick_sort` template](../quicksort) needs a `median()` function; this implements it given any ordering expression.

There isn't much point in using this directly. Still, it could potentially be useful on its own. Here's how you might use it to wrap `strcmp()`:

```c
#define POTTERY_COMPARE_PREFIX string
#define POTTERY_COMPARE_REF_TYPE const char*
#define POTTERY_COMPARE_THREE_WAY strcmp
#include "pottery/compare/pottery_compare_static.t.h"
```

This generates `string_equal()`, `string_less()`, `string_greater()`, `string_less_or_equal()`, `string_compare()`, `string_min()`, `string_max()`, `string_median()`, etc. all as wrappers to `strcmp()`.

```c
string_median("carrot", "apple", "banana"); // returns "banana"
```



## Configuration Options

### Types

- `REF_TYPE`, a type

This is the abstract reference type for the type to be compared. Two values of this type are passed to all configured comparison expressions (after the optional context.)

This is usually either:

- A pointer to the real type, allowing comparisons on objects in memory; or

- An identifier to be used with the context to identify it. For example the context may be a database connection and the type may be the key for a row: in this case the compare expressions would pull the given rows from the database and compare them.

- `VALUE_TYPE`, a type

This is a concrete value type for comparing objects in memory. If this is defined, `REF_TYPE` is defined as a pointer to this.

- `CONTEXT_TYPE`, a type

An optional comparison context type. If configured, the context type is passed as the first argument to all configured comparison expressions and accepted as the first argument of all generated functions.

### Automatic By Value

- `BY_VALUE`, a flag

If 1, the comparison template will use value comparisons on `VALUE_TYPE`, i.e. the operators `<`, `<=`, `==`, `!=`, `>=` and `>` to compare the value directly. `VALUE_TYPE` must be defined and must support these operators. (For C++ types this will work with custom overloaded operators.)

### Relational Expressions

- `EQUAL`, expression matching `bool(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

An expression that evaluates to true if the given values are equal, false otherwise.

- `NOT_EQUAL`, expression matching `bool(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

An expression that evaluates to true if the given values are not equal, false otherwise.

- `LESS`, expression matching `bool(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

An expression that evaluates to true if left is less than right, false otherwise.

- `GREATER`, expression matching `bool(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

An expression that evaluates to true if left is greater than right, false otherwise.

- `LESS_OR_EQUAL`, expression matching `bool(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

An expression that evaluates to true if left is less than or equal to right, false otherwise.

- `GREATER_OR_EQUAL`, expression matching `bool(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

An expression that evaluates to true if left is greater than or equal to right, false otherwise.

- `THREE_WAY`, expression matching `int(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

A [three-way comparison](https://en.wikipedia.org/wiki/Three-way_comparison) expression. It must return less than 0 if left is less than right, greater than 0 if left is greater than right, and 0 if they are equal.



## Generated Functions

- `bool equal(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

Returns true if left is equal to right, false otherwise.

- `bool not_equal(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

Returns true if left is not equal to right, false otherwise.

- `bool less(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

Returns true if left is less than right, false otherwise.

- `bool greater(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

Returns true if left is greater than right, false otherwise.

- `bool greater_or_equal(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

Returns true if left is greater than or equal to right, false otherwise.

- `int three_way(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

Performs a [three-way comparison](https://en.wikipedia.org/wiki/Three-way_comparison).

Returns an integer less than 0 if left is less than right, an integer greater than 0 if left is greater than right, and 0 if they are equal.

- `REF_TYPE min(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

Returns the lesser of left or right.

- `REF_TYPE max(CONTEXT_TYPE context, REF_TYPE left, REF_TYPE right)`

Returns the greater of left or right.

- `REF_TYPE clamp(CONTEXT_TYPE context, REF_TYPE value, REF_TYPE left, REF_TYPE right)`

If value is less than left, returns left; otherwise if value is greater than right, returns right; otherwise returns value.

- `REF_TYPE median(CONTEXT_TYPE context, REF_TYPE a, REF_TYPE b, REF_TYPE c)`

Returns the median of a, b and c.
