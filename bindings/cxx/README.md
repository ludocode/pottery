# Pottery C++ Templates

This contains C++ template wrappers to Pottery to implement containers and algorithms that closely match the APIs of the C++ STL. For example [`pottery::vector`](include/pottery_cxx/pottery_cxx_vector.hxx) functions as a nearly drop-in replacement for [`std::vector`](https://en.cppreference.com/w/cpp/container/vector).

This mostly exists as a stubborn, perverse sort of proof that the C algorithms and containers in Pottery can achieve comparable performance and functionality to their C++ STL counterparts. There are some caveats of course, outlined below.

The C++ headers here do not implement much of their own functionality; they simply configure and instantiate the C Pottery headers into template classes. Take a look at [`pottery::introsort()`](include/pottery_cxx/pottery_cxx_intro_sort.hxx) to see an example. Pottery's C templates are so configurable that we really can just pop them in a C++ `template<> class`, and they even instantiate all of their child templates properly within that C++ template.

This could in theory be useful as a sort of portable runtime-free alternative to STL containers and algorithms, but there are certainly better options out there. This has several disadvantages compared to typical C++ container and algorithm libraries:

- Compiler errors arising from incorrect usage of these templates are considerably uglier than their STL equivalents, which is saying a lot. The smallest spelling mistake will cause the compiler to dump colossal amounts of indecipherable macro template errors. That alone is reason enough to not use this.

- Templates herein intentionally do not use eachother. Each C++ template just wraps a Pottery C template which instantiates its own dependencies, so the lack of template sharing can cause worse template bloat than the C++ STL.

- There is limited exception support. These templates do throw `std::bad_alloc` on memory allocation failures, and they can bubble up `std::bad_alloc` if a value's default constructor, copy constructor or copy operator throw. But they do not handle throws from move constructors or move operators (Pottery will terminate), and they do not bubble up other exception types: all other exceptions are re-thrown as `std::exception`. (You can of course disable exceptions if your compiler supports it, in which case any attempt to throw will terminate instead.)

- There is limited support for C++-specific declarations and optimizations like `constexpr` and `noexcept`. There's nothing really preventing this from being added but there isn't much point in putting in the effort since it's not supported in C.

Maybe the only real use for this is for benchmarking purposes since it allows direct comparisons to C++ containers and algorithms. If you know of a benchmarking suite that compares C++ container or algorithm implementations, add support for Pottery's C++ bindings to see how it fares. I have done very little benchmarking of Pottery so I don't expect it to be fast, but maybe it will be eventually.

Of course Pottery's C++ containers, by virtue of attempting compatibility with the STL, are hamstrung by its API limitations. We ignore some of these, like throwing move constructors, but others we don't: for example maps expose `std::pair<>` as their entry type, forcing keys and values to be stored adjacent in memory. The whole point of Pottery is to be able to more precisely define memory layout, so you could for example separate keys and values into distinct arrays and make the index the key/value type to improve cache coherency in tree traversal. Without the ability to do this in C++, there's no point. I don't know if anyone will actually use these C++ templates, so maybe the best thing to do is to delete these bindings entirely and drop most of the C++ complexity in Pottery.
