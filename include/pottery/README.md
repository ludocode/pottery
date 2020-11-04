This is a listing of templates available from Pottery.

The documentation for each template is inline in each folder for ease of browsing. Just click a folder to see the documentation.

Here's a more hierarchical index:

- Sorting Algorithms
    - [Insertion Sort](insertion_sort/), a very small algorithm that is efficient for short arrays
    - [Shell Sort](shell_sort/), a modified insertion sort that work well on large arrays
    - [Heap Sort](heap_sort/), in-place sort based on binary heap with optimal worst case behaviour
    - [Quick Sort](quick_sort/), a simple and very fast in-place partition sort
    - [Intro Sort](intro_sort/), quick sort with speed and safety fallbacks

- Dynamic Containers, i.e. containers that own and manage memory for their contents
    - [Vector](vector/), a contiguous dynamically growing array
    - [Open Hash Map](open_hash_map/), a dynamically growing open-addressed hash table

- Intrusive/External Containers, i.e. container algorithms that work on external memory
    - [Heap](heap/), binary heap algorithms operating on an external array
    - [Open Hash Table](open_hash_table/), open-addressed hash table algorithms on an external array

- Helper templates used mainly for implementing other Pottery templates
    - [Lifecycle](include/lifecycle/), a wrapper and generator for lifecycle functions
    - [Alloc](include/alloc/), an allocator wrapper to zero, align, expand, relocate, check for overflow
    - [Compare](include/compare/), a generator for comparison functions
