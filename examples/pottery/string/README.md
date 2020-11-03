# Pottery String

This shows how you can use Pottery to implement a simple mutable byte string in C. This example calls it simply `string`, and the type is `string_t`.

Even though this string is just an example to demonstrate Pottery, it is far superior to most other mutable C string libraries. Compare it to some other strings:

- [GString](https://developer.gnome.org/glib/stable/glib-Strings.html)
- [bstring](http://mike.steinert.ca/bstring/)
- [sds](https://github.com/antirez/sds#how-sds-strings-work)
- [vstr](http://www.and.org/vstr/tutorial#hw_s)

I believe you will find this API much more sane and less error-prone (although it can be a bit more verbose.) Despite this, it probably performs better than all of these anyway since it's much more like typical implementations of `std::string`: the container itself is not allocated and it contains internal storage to optimize small strings.

If you'd like to use this string, just copy `string.h` and `string.c` into your project (or include `string.c` straight out of the Pottery examples into some implementation file in your project.) Rename it if you already have something else called `string`.

## Implementation Details

The implementation is mostly just inline functions that wrap an internal Pottery [Vector](../../docs/containers/vector.md) over `uint8_t`. If an out-of-memory situation occurs, the program aborts.

### Small string optimization

`string_t` uses a small string optimization. This provides a small amount of internal storage similar to most implementations of `std::string`, thus avoiding allocation for short strings. It does this simply by enabling the corresponding feature in Pottery's [Vector](../../../include/pottery/vector/).

`string_t` implements most of the [Pottery-style lifecycle functions](../../../docs/lifecycle.md). You should read that document to understand how to safely manage its lifecycle. In particular, with the small string optimization, `string_t` is not bitwise-movable. `string_move()` must be used to move it around, for example when storing it in Pottery containers. Never pass a `string_t` by value and never move it with `memcpy()`.

### Null-termination

`string_t` carries around a null-terminator in its contents. This allows it to be used a C-string through `string_cstr()` so it can be passed to traditional C APIs that require null termination.

To get the string's length, you must use `string_length()`, which will subtract the null-terminator from the number of bytes in its storage.
