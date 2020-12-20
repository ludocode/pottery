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

- `min()` -- the lesser of two references
- `max()` -- the greater of two references
- `clamp()` -- the given reference limited by min and max references
- `median()` -- the median of three references

This template allows you to configure any ordered Pottery container or algorithm with any one ordering expression (like `LESS`, `LESS_OR_EQUAL`, `THREE_WAY`, etc.). It defines all comparison functions that the parent templates may need, so they don't have to worry about negating the expression, swapping arguments, etc.

This template does not distinguish between "weak" and "strong" ordering. Whether "equal" implies equivalence or equality is specific to the configured comparison expressions and the context in which they are used.

This template currently does not support partial ordering. This means you cannot use it for example to sort an array of floats where some are NaN. Functions will be implemented with their opposites where necessary under the assumptions of symmetry and total ordering. For example if you configure only `LESS`, you'll get `less()`, its simple negation `greater_or_equal()`, its symmetric equivalent `greater()` with swapped arguments, etc.



## Example

The Compare template is meant as a helper for other Pottery templates so there isn't much point in using this directly. Still, it could potentially be useful on its own. Here's how you might use it to wrap `strcmp()`:

```c
#define POTTERY_COMPARE_PREFIX string
#define POTTERY_COMPARE_REF_TYPE const char*
#define POTTERY_COMPARE_THREE_WAY strcmp
#include "pottery/compare/pottery_compare_static.t.h"
#include "pottery/compare/pottery_compare_cleanup.t.h"
```

This generates `string_equal()`, `string_less()`, `string_greater()`, `string_less_or_equal()`, `string_min()`, `string_max()`, `string_clamp()`, `string_median()`, etc. all as wrappers to `strcmp()`.

```c
string_median("carrot", "apple", "banana"); // returns "banana"
```



## Configuration Options

### Types

The compare template operates on an abstract ref type with an optional context. In the simplest case, the ref type is just a pointer to an element in memory, and the comparison expressions need no context to compare them.

In more advanced usage, the compare template can even compare abstract values not in memory. A context could contain for example a database connection, the reference type could be the key type for rows to be compared, and configured compare expressions could perform database queries to compare elements.

Types are defined by the [container_types](../container_types/) template.

### Automatic By Value

#### `BY_VALUE`

A flag indicating that values should be compared with ordinary value comparison operators.

If 1, the comparison template will use value comparisons on `VALUE_TYPE`, i.e. the C operators `<`, `<=`, `==`, `!=`, `>=` and `>` to compare values directly. No other comparison expressions can be configured with this.

`VALUE_TYPE` must be defined and must support these operators. For C++ types this will work with custom overloaded operators.

### Relational Expressions

#### `EQUAL`

An expression matching `bool(context_t context, ref_t left, ref_t right)`.

Evaluates to true if the given values are equal, false otherwise.

#### `NOT_EQUAL`

An expression matching `bool(context_t context, ref_t left, ref_t right)`.

Evaluates to true if the given values are not equal, false otherwise.

#### `LESS`

An expression matching `bool(context_t context, ref_t left, ref_t right)`

Evaluates to true if left is less than right, false otherwise.

#### `GREATER`

An expression matching `bool(context_t context, ref_t left, ref_t right)`.

Evaluates to true if left is greater than right, false otherwise.

#### `LESS_OR_EQUAL`

An expression matching `bool(context_t context, ref_t left, ref_t right)`.

Evaluates to true if left is less than or equal to right, false otherwise.

#### `GREATER_OR_EQUAL`

An expression matching `bool(context_t context, ref_t left, ref_t right)`.

Evaluates to true if left is greater than or equal to right, false otherwise.

#### `THREE_WAY`

An expression matching `int(context_t context, ref_t left, ref_t right)`.

Performs a [three-way comparison](https://en.wikipedia.org/wiki/Three-way_comparison). It must return less than 0 if left is less than right, greater than 0 if left is greater than right, and 0 if they are equal.



## API Reference

```c
bool equal(context_t context, ref_t left, ref_t right)
```

Returns true if left is equal to right, false otherwise.

```c
bool not_equal(context_t context, ref_t left, ref_t right)
```

Returns true if left is not equal to right, false otherwise. This returns the opposite of `equal()`.

```c
bool less(context_t context, ref_t left, ref_t right)
```

Returns true if left is less than right, false otherwise.

```c
bool greater(context_t context, ref_t left, ref_t right)
```

Returns true if left is greater than right, false otherwise.

```c
bool less_or_equal(context_t context, ref_t left, ref_t right)
```

Returns true if left is less than or equal to right, false otherwise. This returns the opposite of `greater()`.

```c
bool greater_or_equal(context_t context, ref_t left, ref_t right)
```

Returns true if left is greater than or equal to right, false otherwise. This returns the opposite of `less()`.

```c
int three_way(context_t context, ref_t left, ref_t right)
```

Performs a [three-way comparison](https://en.wikipedia.org/wiki/Three-way_comparison).

Returns an integer less than 0 if left is less than right, an integer greater than 0 if left is greater than right, and 0 if they are equal.

```c
ref_t min(context_t context, ref_t left, ref_t right)
```

Returns the lesser of left or right.

```c
ref_t max(context_t context, ref_t left, ref_t right)
```

Returns the greater of left or right.

```c
ref_t clamp(context_t context, ref_t value, ref_t min, ref_t max)
```

If value is less than min, returns min; otherwise if value is greater than max, returns max; otherwise returns value. In other words it limits the range of value to be between min and max.

The behaviour is undefined if min is greater than max.

```c
ref_t median(context_t context, ref_t a, ref_t b, ref_t c)
```

Returns the median of a, b and c.
