# Pottery Utilities

These are prebuilt containers and other helpful utilities which you can embed in your project.

To embed one of these utilities, put `util/` on your include path and build the `.c` files for each utility you want to use in your project. (Either add the source files to your buildsystem or just `#include` the `.c` file directly into one of your source files.)

You could in theory just include all `.c` files in `util/` recursively to your project to include all utilities. This is not recommended because you'll get a lot of stuff you likely won't use. Pottery is not meant to be a library full of source code that you compile into your project; it's supposed to be mostly templates so you only compile what you use.

These are the utilities available so far:

- [hsearch](hsearch/), an implementation of the POSIX `hsearch()` hash table API
- [qsort](qsort/), an implementation of `qsort()` and its platform-specific extensions
- [string](string/), a fast and easy to use mutable string library
- [string set](string_set/), a dynamic set of unique strings
