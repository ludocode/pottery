/*
 * MIT License
 *
 * Copyright (c) 2020-2021 11037650 Canada Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef POTTERY_QUICK_SORT_IMPL
#error "This is an internal header. Do not include it."
#endif

/*
 * This is the quick_sort template's implementation of partitioning.
 *
 * It is instantiated four times, one for each combination of thin/fat and
 * swap/move. There is also a variation based on whether a three-way comparator
 * is used.
 *
 * All of the below combinations are unit tested. See the various
 * test_quick_sort unit test files.
 *
 *
 * - Thin/Fat:
 *
 * A thin partition is a two-way Hoare partition. Pointers scan from either end
 * of the partition looking for inversions and meet in the middle to find the
 * location of the pivot. Elements on the left are less than or equal to the
 * pivot and elements on the right are greater than or equal to the pivot. All
 * elements equal to the pivot are treated as inversions and swapped to avoid
 * an unbalanced pivot on a partition of equal elements. Each partition
 * therefore ends up with half the elements equal to the pivot.
 *
 * A fat partition is a three-way partition using the method in "Engineering a
 * Sort Function" by Bentley and McIlroy (1993). Elements equal to the pivot
 * are moved to either end of the range during inversions and then moved into
 * the middle after inversions are complete. A good description of this is
 * here:
 *     https://cs.stackexchange.com/a/22752
 *
 *
 * - Swap/Move:
 *
 * A partition by swap can only swap elements. It uses no temporaries (other
 * than potentially a temporary within the swap implementation) so it is
 * completely in-place. It therefore does not require a value type and can
 * operate on an arbitrary ref type.
 *
 * A partition by move can move elements and use temporaries. The pivot is
 * pulled out into a temporary leaving a hole in the array. Inversions are
 * done by alternating between sides moving elements into the hole from one
 * partition to another. This results in two thirds as many moves per inversion
 * compared to swap. It may also be able to hold the pivot in a register which
 * can greatly improve performance.
 *
 *
 * - Three-Way/Boolean:
 *
 * If a user-defined three-way comparator is available, it is called during fat
 * partitioning to determine in which of the three partitions the value belongs
 * in one comparison call.
 *
 * If not, fat partitioning is done by calling boolean ordering expressions.
 * This results in up to two comparisons for each pair of elements. This can be
 * slow for non-trivial comparison expressions, so fat partitioning is disabled
 * by default for a non-trivial comparator without a user-defined three-way
 * comparison expression. However you can still force-enable fat partitioning
 * for quick_sort and it will use the multiple boolean calls.
 */

pottery_maybe_unused static inline
void pottery_quick_sort_partition_impl(
        POTTERY_QUICK_SORT_ARGS
        size_t* first_index, size_t* last_index)
{
    size_t old_first_index = *first_index;
    size_t old_last_index = *last_index;

    // Gather some entries to our partition bounds
    pottery_quick_sort_entry_t first_entry = pottery_quick_sort_array_access_select(
            POTTERY_QUICK_SORT_VALS old_first_index);
    pottery_quick_sort_entry_t last_entry = pottery_quick_sort_array_access_select(
                POTTERY_QUICK_SORT_VALS old_last_index);

    // Choose a pivot
    pottery_quick_sort_ref_t original_pivot_ref =
            pottery_quick_sort_choose_pivot(POTTERY_QUICK_SORT_VALS old_first_index, old_last_index);

    pottery_quick_sort_ref_t first_ref = pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL first_entry);

    #if POTTERY_QUICK_SORT_PARTITION_MOVE
    // Move the pivot into a temporary and move the first value into its place.
    // The first entry will remain empty until the end when we swap equal
    // elements to the middle.
    pottery_quick_sort_value_t pivot_value;
    pottery_quick_sort_lifecycle_move_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
            &pivot_value,
            original_pivot_ref);
    if (!pottery_quick_sort_ref_equal(POTTERY_QUICK_SORT_CONTEXT_VAL original_pivot_ref, first_ref)) {
        pottery_quick_sort_lifecycle_move_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                original_pivot_ref,
                first_ref);
    }
    pottery_quick_sort_ref_t pivot_ref = &pivot_value;
    #else
    // Swap the pivot with the first value in the partition.
    pottery_quick_sort_ref_t pivot_ref = first_ref;
    pottery_quick_sort_lifecycle_swap(POTTERY_QUICK_SORT_CONTEXT_VAL
            original_pivot_ref,
            pivot_ref);
    #endif

    pottery_quick_sort_entry_t left_entry = first_entry;
    pottery_quick_sort_entry_t right_entry = pottery_quick_sort_array_access_next(
            POTTERY_QUICK_SORT_VALS last_entry);

    #if POTTERY_QUICK_SORT_PARTITION_FAT
    pottery_quick_sort_entry_t left_equal_entry = left_entry;
    pottery_quick_sort_entry_t right_equal_entry = right_entry;
    #endif


    while (true) {

        // Search backward from the right until we find an element less than the pivot
        while (true) {
            right_entry = pottery_quick_sort_array_access_previous(POTTERY_QUICK_SORT_VALS right_entry);

            if (pottery_quick_sort_array_access_equal(POTTERY_QUICK_SORT_VALS left_entry, right_entry))
                goto done_inversions;

            #if !POTTERY_QUICK_SORT_PARTITION_FAT
            if (pottery_quick_sort_compare_less_or_equal(POTTERY_QUICK_SORT_CONTEXT_VAL
                        pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry),
                        pivot_ref))
                break;
            #else

            // If we have a user-defined three-way comparison function, we only
            // want to call it once, so we store its result. If we don't, we
            // may be able to avoid an unnecessary comparison so we call less()
            // and greater() separately. We avoid calling equal() since it may
            // also be implemented by calling LESS/GREATER/etc. twice.
            //
            // (Note that this will need to be extended for the C++20 spaceship
            // operator later. We want to use it if it's efficient, but we
            // can't determine that with the preprocessor so we need to
            // restructure this a bit. See other references to it in this
            // template and in Pottery in general.)

            #if POTTERY_COMPARE_HAS_USER_THREE_WAY
            int three_way = pottery_quick_sort_compare_three_way(POTTERY_QUICK_SORT_CONTEXT_VAL
                        pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry),
                        pivot_ref);
            bool greater = three_way > 0;
            #else
            bool greater = pottery_quick_sort_compare_greater(POTTERY_QUICK_SORT_CONTEXT_VAL
                        pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry),
                        pivot_ref);
            #endif
            if (!greater) {

                #if POTTERY_COMPARE_HAS_USER_THREE_WAY
                bool less = three_way < 0;
                #else
                bool less = pottery_quick_sort_compare_less(POTTERY_QUICK_SORT_CONTEXT_VAL
                            pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry),
                            pivot_ref);
                #endif
                if (less)
                    break;

                // The value is equal to the pivot. Swap it with the last entry in
                // the greater portion.
                // (Note that we don't have an optimization for equal elements to
                // move instead of swap as we do with inversions. This is another
                // optimization that can be added later.)
                right_equal_entry = pottery_quick_sort_array_access_previous(POTTERY_QUICK_SORT_VALS right_equal_entry);
                pottery_quick_sort_lifecycle_swap(POTTERY_QUICK_SORT_CONTEXT_VAL
                        pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry),
                        pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_equal_entry));
            }
            #endif
        }

        #if POTTERY_QUICK_SORT_PARTITION_MOVE
        // Put this element in the hole on the left; the hole is now on the right.
        pottery_quick_sort_lifecycle_move_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_entry),
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry));
        #endif

        // Search forward from the left until we find an element greater than the pivot
        while (true) {
            left_entry = pottery_quick_sort_array_access_next(POTTERY_QUICK_SORT_VALS left_entry);

            if (pottery_quick_sort_array_access_equal(POTTERY_QUICK_SORT_VALS left_entry, right_entry))
                goto done_inversions;

            #if !POTTERY_QUICK_SORT_PARTITION_FAT
            if (pottery_quick_sort_compare_greater_or_equal(POTTERY_QUICK_SORT_CONTEXT_VAL
                        pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_entry),
                        pivot_ref))
                break;
            #else

            // As above, use a three-way comparison if user-defined

            #if POTTERY_COMPARE_HAS_USER_THREE_WAY
            int three_way = pottery_quick_sort_compare_three_way(POTTERY_QUICK_SORT_CONTEXT_VAL
                        pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_entry),
                        pivot_ref);
            bool less = three_way < 0;
            #else
            bool less = pottery_quick_sort_compare_less(POTTERY_QUICK_SORT_CONTEXT_VAL
                        pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_entry),
                        pivot_ref);
            #endif
            if (!less) {

                #if POTTERY_COMPARE_HAS_USER_THREE_WAY
                bool greater = three_way > 0;
                #else
                bool greater = pottery_quick_sort_compare_greater(POTTERY_QUICK_SORT_CONTEXT_VAL
                            pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_entry),
                            pivot_ref);
                #endif
                if (greater)
                    break;

                // The value is equal to the pivot. Swap it with the first entry in
                // the less portion.
                left_equal_entry = pottery_quick_sort_array_access_next(POTTERY_QUICK_SORT_VALS left_equal_entry);
                pottery_quick_sort_lifecycle_swap(POTTERY_QUICK_SORT_CONTEXT_VAL
                        pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_entry),
                        pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_equal_entry));

            }
            #endif
        }

        #if POTTERY_QUICK_SORT_PARTITION_MOVE
        // Put this element in the hole on the right; the hole is now on the left.
        pottery_quick_sort_lifecycle_move_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry),
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_entry));
        #else
        // Swap 'em
        pottery_quick_sort_lifecycle_swap_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_entry),
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry));
        #endif
    }

done_inversions:;

    #if POTTERY_QUICK_SORT_PARTITION_MOVE
        // We've converged on the hole, where the pivot goes.
        pottery_quick_sort_lifecycle_move_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry),
                &pivot_value);
    #else
        // Our entries have met on a value that belongs on the left (or on the
        // pivot or a left equal value, if there are no lesser elements.)
        pottery_quick_sort_lifecycle_swap(POTTERY_QUICK_SORT_CONTEXT_VAL
                pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry),
                pivot_ref);
    #endif


    #if !POTTERY_QUICK_SORT_PARTITION_FAT
        *first_index = pottery_quick_sort_array_access_index(POTTERY_QUICK_SORT_VALS left_entry);
        *last_index = *first_index;

    #else

        // We now swap the equal value portions back to the middle.
        //
        // It's possible the equal value portions are larger than the less/greater
        // sections so we only swap the minimum number of values needed. e.g. only
        // the starred values need to be swapped here:
        //
        //      ***  ***
        //     <=====<<<P>>=======
        //               **     **
        //
        // Note that the pivot is handled specially in both move and swap
        // variants, which is why there's a left element to the left of the
        // equal elements partition in the diagram above. In the case of no
        // equal elements, we want the below loop branches to all be false.

        size_t left_equal_index = pottery_quick_sort_array_access_index(POTTERY_QUICK_SORT_VALS left_equal_entry);
        size_t pivot_index = pottery_quick_sort_array_access_index(POTTERY_QUICK_SORT_VALS left_entry);

        // range of values less than the pivot, pre-swap and pre-pivot-insertion
        size_t first_less_index = left_equal_index + 1;
        size_t end_less_index = pivot_index + 1;

        size_t first_greater_index = pivot_index + 1;
        size_t end_greater_index = pottery_quick_sort_array_access_index(POTTERY_QUICK_SORT_VALS right_equal_entry);
        pottery_assert(end_less_index >= first_less_index);
        pottery_assert(first_greater_index >= first_less_index);
        pottery_assert(end_greater_index >= first_greater_index);
        size_t i;

        size_t left_swap_count = pottery_min_s(left_equal_index - old_first_index,
                    end_less_index - first_less_index - 1); // -1 because we already swapped the pivot
        size_t right_swap_count = pottery_min_s(end_greater_index - first_greater_index,
                old_last_index + 1 - end_greater_index);

        // Swap the left equal elements into the middle (including the pivot)
        for (i = 0; i < left_swap_count; ++i) {
            first_entry = pottery_quick_sort_array_access_next(POTTERY_QUICK_SORT_VALS first_entry);
            left_entry = pottery_quick_sort_array_access_previous(POTTERY_QUICK_SORT_VALS left_entry);
            pottery_quick_sort_lifecycle_swap_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                    pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL left_entry),
                    pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL first_entry));
        }

        // Swap the right equal elements into the middle
        for (i = 0; i < right_swap_count; ++i) {
            right_entry = pottery_quick_sort_array_access_next(POTTERY_QUICK_SORT_VALS right_entry);
            pottery_quick_sort_lifecycle_swap_restrict(POTTERY_QUICK_SORT_CONTEXT_VAL
                    pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL right_entry),
                    pottery_quick_sort_entry_ref(POTTERY_QUICK_SORT_CONTEXT_VAL last_entry));
            last_entry = pottery_quick_sort_array_access_previous(POTTERY_QUICK_SORT_VALS last_entry);
        }

        *first_index = old_first_index + end_less_index - first_less_index;
        *last_index = old_last_index - end_greater_index + first_greater_index;
    #endif

    pottery_assert(*last_index >= *first_index);
}

#undef POTTERY_QUICK_SORT_PARTITION_FAT
#undef POTTERY_QUICK_SORT_PARTITION_MOVE
#undef pottery_quick_sort_partition_impl
