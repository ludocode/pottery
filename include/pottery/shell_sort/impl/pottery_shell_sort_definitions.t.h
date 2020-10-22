/*
 * MIT License
 *
 * Copyright (c) 2020 Nicholas Fraser
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

#ifndef POTTERY_SHELL_SORT_IMPL
#error "This is an internal header. Do not include it."
#endif

POTTERY_SHELL_SORT_EXTERN
void pottery_shell_sort(
        #ifdef POTTERY_SHELL_SORT_CONTEXT_TYPE
        pottery_shell_sort_context_t context,
        #endif
        pottery_shell_sort_ref_t first,
        size_t count
) {
    // Calculate gaps based on Ciura sequence extended by *2.25 (A102549).
    //     https://en.wikipedia.org/wiki/Shellsort#Gap_sequences
    // The Ciura gap sequence needs 20 elements on 32-bit or 47 elements on
    // 64-bit (not counting our overflow check or limit below, so actually
    // slightly less.) This is a bit larger than needed but it's not important.
    size_t gaps[sizeof(size_t) * 6];
    size_t i = 0;
    gaps[0] = 1;
    for (;;) {
        static const size_t hardcoded_gap_sequence[] = {
            4, 10, 23, 57, 132, 301, 701
        };
        size_t next_gap;
        if (i < pottery_array_count(hardcoded_gap_sequence)) {
            next_gap = hardcoded_gap_sequence[i];
        } else {
            // *9/4 == 2.25
            if (pottery_mul_overflow_s(gaps[i], 9, &next_gap))
                break;
            next_gap /= 4;
        }

        // Limit the gap sequence arbitrarily
        if (next_gap > count / 2)
            break;

        // Make sure we don't overflow the gaps array (this isn't supposed to
        // be possible but we check just in case)
        if (pottery_unlikely(++i == pottery_array_count(gaps))) {
            pottery_assert(false); // oops? file a bug report!
            --i;
            break;
        }

        gaps[i] = next_gap;
    }

    pottery_shell_sort_state_t state;
    #ifdef POTTERY_SHELL_SORT_CONTEXT_TYPE
    state.context = context;
    #endif

    // Perform successive insertion sorts based on gap sequence
    for (;;) {
        state.gap = gaps[i];
        for (state.offset = 0; state.offset < state.gap; ++state.offset) {
            size_t step_count = count / state.gap;
            if (state.offset + state.gap * step_count < count)
                ++step_count;

            //printf("sorting %zi elements with offset %zi gap %zi step_count %zi\n", count, state.offset, state.gap, step_count);
            // TODO we could simplify this by offsetting first before calling
            // this, that way we don't have to put offset in state.
            pottery_shell_sort_insertion_sort(state, first, step_count);
        }

        if (i == 0)
            break;
        --i;
    }
}
