This is a listing of templates available from Pottery.

The documentation for each template is inline in each folder for ease of browsing. Just click a folder to see the documentation.

Documentation is currently incomplete or missing entirely for most templates. Much more needs to be written. Eventually each template README should contain a full API reference.

Here's a more hierarchical index:

- Dynamic Containers, i.e. containers that own and manage memory for their contents
    - Arrays
        - [Vector](vector/), a contiguous dynamically growing array
        - [Ring](ring/), a dynamically growing circular buffer
        - [Pager](pager/), a paged double-ended queue
    - Maps
        - [Open Hash Map](open_hash_map/), a dynamically growing open-addressed hash table
    - Others
        - [Priority Queue](priority_queue/), a heap over a growing array

- Intrusive/External Containers, i.e. container algorithms that work on external memory
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
