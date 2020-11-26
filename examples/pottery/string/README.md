# Pottery String

This shows how you can use Pottery to implement a simple mutable byte string in C. This example calls it simply `string`, and the type is `string_t`.

Even though this string is just an example to demonstrate Pottery, it is far superior to other C string libraries. Compare it to some other strings:

- [GString](https://developer.gnome.org/glib/stable/glib-Strings.html)
- [bstring](http://mike.steinert.ca/bstring/)
- [sds](https://github.com/antirez/sds)
- [vstr](http://www.and.org/vstr/tutorial)
- [maxim2266/str](https://github.com/maxim2266/str)

I believe you will find this API much more sane and less error-prone (although it can be a bit more verbose.) Despite this, it probably performs better than all of these anyway since it's much more like typical implementations of `std::string`: the container itself is not allocated and it contains internal storage to avoid allocations for small strings.

See [`main.c`](main.c) for an example of how it's used, and take a look at [Clayfish](../clayfish/) to see this string used in a "real" application.

If you'd like to use this string, add Pottery and this example to your include path and copy `string.h` and `string.c` into your project (or just include `string.c` into one of your C files.) Rename this if you already have something else called `string`.





## Implementation Details

The implementation is mostly just inline functions that wrap an internal Pottery [vector](../../../include/pottery/vector/) of `char`, preserving a null-terminating byte at the end of the vector. If an out-of-memory situation occurs, the program aborts.

### Small string optimization

`string_t` uses a small string optimization. This provides a small amount of internal storage similar to most implementations of `std::string`, avoiding allocation for short strings. It does this simply by enabling the corresponding feature in Pottery's [Vector](../../../include/pottery/vector/).

`string_t` implements all of the [Pottery-style lifecycle functions](../../../docs/lifecycle_style.md). You should read that document to understand how to safely manage its lifecycle. In particular, with the small string optimization, `string_t` is not bitwise-movable. `string_move()` must be used to move it around, for example when storing it in Pottery containers. Never pass a `string_t` by value and never move it with `memcpy()`.

### Null-termination

`string_t` carries around a null-terminator in its contents. This allows it to be used a C-string through `string_cstr()` so it can be passed to traditional C APIs that require null termination.

To get the string's length, you must use `string_length()`, which will subtract the null-terminator from the number of bytes in its storage.

All manipulation functions preserve the null-terminator at the end. For example increasing the length with `string_set_length()` adds uninitialized space and moves the null-terminator after it, so once the space is initialized `string_cstr()` will return a valid null-terminated string (as long as there are no other null bytes in the string.)





## API Quick Reference

The documentation is mostly inline in the header file. Here's a quick reference.



### Type

The string value type is `string_t`, which is a struct of 24 or 32 bytes depending on the platform. You can declare this by value on the stack, as a field in another struct, etc.

`string_t` is not bitwise-movable! You must never pass it by value to functions, return it from functions, `memcpy()` or `memmove()` it, etc. You also cannot do any of these things with structs that have a `string_t` value as a field; structs with `string_t` values are themselves not bitwise-movable. Use `string_move()` to move strings.

`string_t` is a typedef to `struct string_t`. You can forward-declare `struct string_t` and use pointers to it to avoid including the header file (and thus avoid including Pottery's dependencies), although you won't be able to declare values of it without including the header.



### Lifecycle

This string implements [Pottery-style lifecycle functions](../../../docs/lifecycle_style.md) to manage its lifecycle.

String functions that start with `string_init` take an uninitialized string as the first argument and initialize it. All other string functions operate on initialized strings. You must initialize a string first before calling any other function on it.

```c
void string_init(string_t* string)
```

Initialize the string to the empty string.

```c
void string_init_copy(string_t* string, const string_t* other)
```

Initialize the string as a copy of another string.

```c
void string_init_steal(string_t* string, string_t* other)
```

Initialize the string by stealing the contents of another string (leaving it initialized with junk.)

```c
void string_destroy(string_t* string)
```

De-initialize the string.

```c
void string_move(string_t* to, string_t* from)
```

Move the string from one address to another without (de-)initialization.

```c
void string_copy(string_t* to, const string_t* from)
```

Replace the contents of the "to" string, copying the contents of the "from" string into it.

```c
void string_steal(string_t* to, const string_t* from)
```

Replace the contents of the "to" string by stealing the contents of the "from" string (leaving it initialized with junk.)

```c
void string_swap(string_t* left, string_t* right)
```

Swap the contents of two strings.



### Introspection

```c
size_t string_length(const string_t* string)
```

Returns the length of the string in bytes, not counting the null-terminator.

```c
char* string_bytes(string_t* string)
```

Returns a mutable pointer to the bytes contained in the string.

```c
const char* string_cstr(string_t* string)
```

Returns a const pointer to the bytes contained in the string, usable as a null-terminated C string.



### Comparison

```c
bool string_equal(const string_t* left, const string_t* right)
```

Returns true if the given strings are equal.

```c
int string_compare(const string_t* left, const string_t* right)
```

Performs a three-way comparison of the given strings similar to `strcmp()`.

```c
bool string_equal_bytes(const string_t* left, const char* right, size_t right_length)
```

Returns true if the string is equal to the given length-delimited char array.

```c
bool string_equal_cstr(const string_t* string, const char* cstr)
```

Returns true if the string is equal to the given null-terminated C string.

```c
bool string_equal_char(const string_t* string, char c)
```

Returns true if if the string has length 1 and its sole character matches the given character.



### Search

```c
size_t string_find_char(string_t* string, size_t index, char c)
```

Find the first occurence of the given character within the string starting at the given index, or the length of the string if not found.

```c
size_t string_find_not_char(string_t* string, size_t index, char c)
```

Find the first character within the string that does not match the given character starting at the given index, or the length of the string if all remaining characters match the given character.

```c
size_t string_find_chars(string_t* string, size_t index, const char* chars, size_t char_count)
```

Find the first occurence of any character in the given char array within this string starting at the given index, or the length of the string if not found.

```c
size_t string_find_chars_cstr(string_t* string, size_t index, const char* chars)
```

Find the first occurence of any character in the given null-terminated C string within this string starting at the given index, or the length of the string if not found.



### String manipulation

#### Length manipulation

```c
void string_clear(string_t* string)
```

Clears the string.

```c
void string_set_length(string_t* string, size_t length)
```

Sets the length of the string to the given length, truncating it or adding uninitialized space.

#### Insert

```c
void string_insert_char(string_t* string, size_t index, char c)
```

Inserts the given char into this string at the given index.

```c
void string_insert_chars(string_t* string, size_t index, const char* chars, size_t count)
```

Inserts the given char array into this string at the given index.

```c
void string_insert_cstr(string_t* string, size_t index, const char* cstr)
```

Inserts the given null-terminated C string into this string at the given index.

```c
void string_insert_string(string_t* string, size_t index, const string_t* other)
```

Inserts the given string into this string at the given index.

```c
void string_insert_substr(string_t* string, size_t index, const string_t* other, size_t other_index, size_t count)
```

Inserts a substring of the given string into this string at the given index.

#### Append

```c
void string_append_char(string_t* string, char c)
```

Appends the given char to the end of the string.

```c
void string_append_chars(string_t* string, const char* chars, size_t count)
```

Appends the given char array to the end of the string.

```c
void string_append_cstr(string_t* string, const char* cstr)
```

Appends the given null-terminated C string to the end of the string.

```c
void string_append_string(string_t* string, const string_t* other)
```

Appends the given string to the end of this string.

```c
void string_append_substr(string_t* string, const string_t* other, size_t other_index, size_t count)
```

Appends a substring of the given string to the end of this string.

#### Set

(Note that there is no `string_set()`. To set the contents of the string to that of another string, use `string_copy()`; see the lifecycle functions above.)

```c
void string_set_chars(string_t* string, const char* chars, size_t count)
```

Replaces the contents of the string, copying the given char array into it.

```c
void string_set_cstr(string_t* string, const char* cstr)
```

 Replaces the contents the string, copying the given null-terminated C string into it.

#### Remove

```c
void string_remove(string_t* string, size_t index, size_t count)
```

Removes the given number of bytes from the given index.

```c
void string_remove_first(string_t* string, size_t count)
```

Removes the given number of bytes from the start of the string.

```c
void string_remove_last(string_t* string, size_t count)
```

Removes the given number of bytes from the end of the string.



### Format

These add formatted string data to the string using C99 `vsnprintf()`. Your platform must support it.

```c
void string_append_format(string_t* string, const char* format, ...)
```

Append formatted string data to the end of the string.

```c
void string_append_vformat(string_t* string, const char* format, va_list args)
```

Append formatted string data to the end of the string.

```c
void string_insert_format(string_t* string, size_t index, const char* format, ...)
```

Insert formatted string data at the given index.

```c
void string_insert_vformat(string_t* string, size_t index, const char* format, va_list args)
```

Insert formatted string data at the given index.

```c
void string_set_format(string_t* string, const char* format, ...)
```

Replace the contents of the string with formatted string data.

```c
void string_set_vformat(string_t* string, const char* format, va_list args)
```

Replace the contents of the string with formatted string data.

```c
void string_set_format(string_t* string, const char* format, ...)
```

Initialize the string with formatted string data.

```c
void string_set_vformat(string_t* string, const char* format, va_list args)
```

Initialize the string with formatted string data.



### Miscellaneous functions

```c
void string_lstrip_chars(string_t* string, const char* chars, size_t char_count)
```

Strip any occurrences of the given characters from the start of the string.

```c
void string_lstrip_chars_cstr(string_t* string, const char* str)
```

Strip any occurrences of the characters in the given null-terminated C string from the start of the string.

```c
void string_rstrip_chars(string_t* string, const char* chars, size_t char_count)
```

Strip any occurrences of the given characters from the end of the string.

```c
void string_rstrip_chars_cstr(string_t* string, const char* str)
```

Strip any occurrences of the characters in the given null-terminated C string from the end of the string.

```c
void string_strip_chars(string_t* string, const char* chars, size_t char_count)
```

Strip any occurrences of the given characters from the start and end of the string.

```c
void string_strip_chars_cstr(string_t* string, const char* str)
```

Strip any occurrences of the characters in the given null-terminated C string from the start and end of the string.
