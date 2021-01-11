# How It Works

Most of the Pottery source code is in _template headers_. They have the extension `.t.h` and do not have include guards. A Pottery template is configured by macros and then included to instantiate it. The macros within a template consolidate the configuration, forward it to child templates, and rename all of its identifiers to make it unique.

This document is a work in progress. It needs to be expanded.



## Basic Headers

To understand the basics, follow along with the source code to Pottery's [compare](../include/pottery/compare/) template since it's probably Pottery's simplest template. It's mostly intended as a helper for other templates, but you can instantiate and use it directly, like this:

```c
#define POTTERY_COMPARE_PREFIX string
#define POTTERY_COMPARE_REF_TYPE const char*
#define POTTERY_COMPARE_THREE_WAY strcmp
#include "pottery/compare/pottery_compare_static.t.h"
```

A template instantiation here is performed by [`pottery_compare_static.t.h`](../include/pottery/compare/pottery_compare_static.t.h). This performs the following steps separated into different headers:

- Configuring and renaming identifiers. This is [`impl/pottery_compare_macros.t.h`](../include/pottery/compare/impl/pottery_compare_macros.t.h). It checks the user's configuration, then performs a set of simple `#define`s to rename all of the template's types and functions.

- Instantiating the code. The code is in [`impl/pottery_compare_declarations.t.h`](../include/pottery/compare/impl/pottery_compare_declarations.t.h). The compare template mostly just wraps the user's configuration so it's not very interesting. However you can see the `min()`, `max()`, `clamp()`  and `median()` functions at the bottom look somewhat like idiomatic C (the only oddity is an optional context parameter.)

- Undefining renaming and configuration macros. This is in [`impl/pottery_compare_unmacros.t.h`](../include/pottery/compare/impl/pottery_compare_unmacros.t.h). This undefines all macros used to instantiate the template, including the renaming macros and the user's configuration macros. The only macros left after this are the capabilities of the template: for example `POTTERY_COMPARE_CAN_ORDER` is defined to 1 to indicate that we have ordering capabilities so we can expect `string_less()` and friends to be defined.

- Undefining exported capability macros. This is in [`pottery_compare_cleanup.t.h`](../include/pottery/compare/pottery_compare_cleanup.t.h). In this case it happens automatically because we haven't defined `POTTERY_COMPARE_NO_CLEANUP`. See [Exported Configuration](#exported-configuration) below.

The template can now be instantiated again with a different configuration.



## Separate Header and Source Files

The compare template is simple in part because all of its functions are `static inline`. Larger Pottery functions are not inline. Instead, Pottery templates with large functions have a separate `definitions` header.

Take a look at the [alloc](../include/pottery/alloc/) template for an example. This has two separate implementation headers: [`impl/pottery_alloc_declarations.t.h`](../include/pottery/alloc/impl/pottery_alloc_declarations.t.h) and [`impl/pottery_alloc_definitions.t.h`](../include/pottery/alloc/impl/pottery_alloc_definitions.t.h).

These two files correspond more or less to the header and source files of normal C code. The `declarations` file should be included in a normal header, while the `definitions` file should be included in a source file. This way the non-inline definitions exist only in one translation unit.

This does mean that the configuration of the template must be repeated. If you want to avoid duplicating it, you can put your configuration in your own `.t.h` file.



## Template Composition

Pottery's templates are composable. For example the intro_sort template internally instantiates all of the following templates, some of which instantiate their own templates and some of which depend on others:

- [`intro_sort`](../include/pottery/intro_sort/)
    - [`quick_sort`](../include/pottery/quick_sort/)
    - [`insertion_sort`](../include/pottery/insertion_sort/)
    - [`heap_sort`](../include/pottery/heap_sort/)
        - [`heap`](../include/pottery/heap/)
    - [`lifecycle`](../include/pottery/lifecycle/)
    - [`compare`](../include/pottery/compare/)
    - [`array_access`](../include/pottery/array_access/)

Configuration of child templates is done by `config` headers. For example intro_sort configures its child quick_sort in [`pottery_intro_sort_config_quick_sort.t.h`](../include/pottery/intro_sort/impl/pottery_intro_sort_config_quick_sort.t.h). Template composition is accomplished by forwarding along whatever configuration is necessary, instantiating dependent templates, and then renaming the functions they would use in those templates. Some of the forwarding and un-forwarding code is generated using [meta-templates](../meta/).

In many cases, a parent template needs to declare some identifiers before instantiating its child template. This is accomplished using a `forward` implementation header. For example, [shell sort](../include/pottery/shell_sort/) must forward-declare its state in order to use it as a context for its child [insertion_sort](../include/pottery/insertion_sort/). This happens in [`impl/pottery_shell_sort_forward.t.h`](../include/pottery/shell_sort/impl/pottery_shell_sort_forward.t.h) before insertion_sort is instantiated. You can see the include order in [`pottery_shell_static.t.h`](../include/pottery/shell_sort/pottery_shell_sort_static.t.h)

Some templates can depend on external instantiations of its child templates. For example, with intro_sort above, all of the various sort algorithms can share a single instantiation of lifecycle. Rather than letting each one instantiate its own copy, intro_sort instantiates it, then configures it as an external template for its children.



## Exported Configuration

Some templates export macros to allow parent templates to detect their capabilities after they are instantiated. For example the lifecycle template can instantiate a `swap()` function if given a `SWAP` expression. But it can also instantiate a `swap()` function if given a `MOVE` expression and a `VALUE_TYPE`: it will swap by moving through a temporary.

In order to allow the parent to detect whether swap is possible, the lifecycle template can export the macro `POTTERY_LIFECYCLE_CAN_SWAP`. This is defined to either 1 or 0, corresponding to whether or not `swap()` exists.

Templates by default automatically cleanup these macros. If you want them to be exported, you must define `NO_CLEANUP` on the template before instantiating it, and you must clean them up yourself when no longer needed by including the corresponding `cleanup` header. For example the lifecycle template supports `POTTERY_LIFECYCLE_NO_CLEANUP` to export its capabilities and provides [`pottery_lifecycle_cleanup.t.h`](../include/pottery/lifecycle/pottery_lifecycle_cleanup.t.h) to clean them up. All templates that instantiate lifecycle use this to detect its capabilities.

See for example [`pottery_insertion_sort_static.t.h`](../include/pottery/insertion_sort/pottery_insertion_sort_static.t.h). If it is not using some other template's instantiation of lifecycle functions (via `EXTERNAL_LIFECYCLE`), it instantiates the lifecycle template with `NO_CLEANUP`, instantiates its own functions based on the capabilities of the lifecycle template, then includes the lifecycle cleanup header to clean up. In [`impl/pottery_insertion_sort_definitions.t.h`](../include/pottery/insertion_sort/impl/pottery_insertion_sort_definitions.t.h) you can see where it checks `POTTERY_LIFECYCLE_CAN_MOVE` to decide whether to move or swap.

`POTTERY_LIFECYCLE_CAN_DESTROY` and `POTTERY_LIFECYCLE_CAN_PASS` are some other macros that are checked often. You can see these macros throughout the implementation of [vector](../include/pottery/vector/) for example. These change the vector's behaviour and capabilities based on whether it can destroy or pass the values it contains. For example `displace_at()` is always implemented, but `remove_at()` is only implemented if the vector can destroy its values, and `extract_at()` is only implemented if the vector's values can be passed as function arguments or return values.



## Contexts

Most templates support a context to be passed to configured expressions. For dynamic containers, the container stores the context. For algorithms, the context is passed in as an argument to every function.

Since the context is optional, every function involving a context has an optional argument. To simplify this, templates typically define macros:

- `CONTEXT_ARG` evaluates to a context function parameter within a function prototype if a context is defined, such as "`pottery_context_t context,`" (with the comma.) It evaluates to nothing otherwise.

- `CONTEXT_VAL` evaluates to a context value to be passed as a function parameter if a context is defined, such as "`context,`" (with the comma.) It evaluates to nothing otherwise.

- `CONTEXT_UNUSED` evaluates to a cast of the context to void if a context is defined, e.g. `(void)context`, to silence warnings about unused arguments. It evaluates to nothing otherwise.

If you want to have multiple contexts (for example one context for allocation expressions and another context for compare expressions), you need to combine them into a struct to be passed to all expressions.
