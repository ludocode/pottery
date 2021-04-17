This is a listing of templates available from Pottery.

The documentation for each template is inline in each folder for ease of browsing. Just click a folder to see the documentation.

Documentation is currently incomplete or missing entirely for most templates. Much more needs to be written. Eventually each template README should contain a full API reference.

## Hierarchical Index

- Dynamic Containers, i.e. containers that own and manage memory for their contents
    - Arrays
        - [Vector](vector/), a contiguous dynamically growing array
        - [Ring](ring/), a dynamically growing circular buffer
        - [Pager](pager/), a paged double-ended queue
    - Maps
        - [Tree Map](tree_map/), a self-balancing binary search tree with dynamically allocated nodes
        - [Open Hash Map](open_hash_map/), a dynamically growing open-addressed hash table
        - [Array Map](array_map/), a map over a dynamically growing array
    - Others
        - [Node List](node_list/), a doubly-linked list of dynamically allocated nodes
        - [Priority Queue](priority_queue/), a heap over a growing array

- User-Allocated Containers, i.e. container algorithms that work on user-allocated memory
    - Intrusive Containers
        - [Red-Black Tree](red_black_tree/), a red-black tree with configurable links and user-allocated nodes
        - [Doubly-Linked List](doubly_linked_list/), an intrusive doubly-linked list with user-allocated nodes
    - External Array Containers
        - [Heap](heap/), binary heap algorithms on an external array
        - [Open Hash Table](open_hash_table/), open-addressed hash table algorithms on an external array

- Sorting Algorithms
    - [Insertion Sort](insertion_sort/), a very small algorithm that is efficient for short arrays
    - [Quick Sort](quick_sort/), a simple and very fast in-place partition sort
    - [Intro Sort](intro_sort/), quick sort with speed and safety fallbacks
    - [Shell Sort](shell_sort/), a modified insertion sort that works well on large arrays
    - [Heap Sort](heap_sort/), in-place sort based on binary heap with optimal worst case behaviour

- Helper templates used mainly for implementing other Pottery templates
    - [Container Types](container_types/), typedefs for container types and conversions between them
    - [Lifecycle](lifecycle/), a wrapper and generator for lifecycle functions
    - [Alloc](alloc/), an allocator wrapper to zero, align, expand, relocate, check for overflow
    - [Compare](compare/), a generator for comparison functions
    - [Array Access](array_access/), accessor functions for a generalized array

## C++ Equivalent

Here are the Pottery template equivalents of typical C++ containers and algorithms:

|C++|Pottery|
| :-- | :-- |
| [`std::vector`](https://en.cppreference.com/w/cpp/container/vector) | [`vector`](vector/) |
| [`std::deque`](https://en.cppreference.com/w/cpp/container/deque) | [`pager`](pager/) |
| [`std::list`](https://en.cppreference.com/w/cpp/container/list) | [`node_list`](node_list/) |
| [`std::map`](https://en.cppreference.com/w/cpp/container/map), [`std::set`](https://en.cppreference.com/w/cpp/container/set) | [`tree_map`](tree_map/) |
| [`std::unordered_map`](https://en.cppreference.com/w/cpp/container/unordered_map), [`std::unordered_set`](https://en.cppreference.com/w/cpp/container/unordered_set) | [`open_hash_map`](open_hash_map/) |
| [`std::priority_queue`](https://en.cppreference.com/w/cpp/container/priority_queue) | [`priority_queue`](priority_queue/) |
| [`std::push_heap`](https://en.cppreference.com/w/cpp/algorithm/push_heap), [`std::pop_heap`](https://en.cppreference.com/w/cpp/algorithm/pop_heap), etc. | [`heap`](heap/) |
| [`std::sort`](https://en.cppreference.com/w/cpp/algorithm/sort) | [`intro_sort`](intro_sort/) |
| [`boost::flat_map`](https://www.boost.org/doc/libs/1_76_0/doc/html/boost/container/flat_map.html) | [`array_map`](array_map/) |
| [`boost::circular_buffer_space_optimized`](https://www.boost.org/doc/libs/1_61_0/doc/html/circular_buffer.html) | [`ring`](ring/) |
| [`boost::intrusive::hashtable`](https://www.boost.org/doc/libs/1_35_0/doc/html/boost/intrusive/hashtable.html) | [`open_hash_table`](open_hash_table/) |
| [`boost::intrusive::list`](https://www.boost.org/doc/libs/1_74_0/doc/html/intrusive/list.html) | [`doubly_linked_list`](doubly_linked_list/) |
| [`boost::intrusive::rbtree`](https://www.boost.org/doc/libs/1_74_0/doc/html/intrusive/set_multiset.html) | [`red_black_tree`](red_black_tree/) |

Also note there are [C++ bindings](../../bindings/cxx/) of some Pottery templates that can serve as drop-in replacements for the STL in C++ code.
