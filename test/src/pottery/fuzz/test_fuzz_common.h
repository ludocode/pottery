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

#ifndef POTTERY_FUZZ_TEST_FUZZ_COMMON_H
#define POTTERY_FUZZ_TEST_FUZZ_COMMON_H

#include "pottery/common/test_pottery_ufo.h"

#define FUZZ_UFO_NAME_MAX_LENGTH 7

typedef struct fuzz_input_t {
    const uint8_t* pos;
    const uint8_t* end;
} fuzz_input_t;

/*
 * Byte loaders in little-endian
 */

static inline uint32_t fuzz_load_u8(fuzz_input_t* input) {
    if (input->end == input->pos)
        return 0;
    return *(input->pos++);
}

static inline uint32_t fuzz_load_u16(fuzz_input_t* input) {
    if (input->end - input->pos < 2) {
        input->pos = input->end;
        return 0;
    }

    uint16_t ret = pottery_cast(uint16_t,
            (pottery_cast(uint16_t, input->pos[1]) << 8) |
            (pottery_cast(uint16_t, input->pos[0])));

    input->pos += 2;
    return ret;
}

static inline uint32_t fuzz_load_u24(fuzz_input_t* input) {
    if (input->end - input->pos < 3) {
        input->pos = input->end;
        return 0;
    }

    uint32_t ret = pottery_cast(uint32_t,
            (pottery_cast(uint32_t, input->pos[2]) << 16) |
            (pottery_cast(uint32_t, input->pos[1]) << 8) |
            (pottery_cast(uint32_t, input->pos[0])));

    input->pos += 3;
    return ret;
}

static inline uint32_t fuzz_load_u32(fuzz_input_t* input) {
    if (input->end - input->pos < 4) {
        input->pos = input->end;
        return 0;
    }

    uint32_t ret = pottery_cast(uint32_t,
            (pottery_cast(uint32_t, input->pos[3]) << 24) |
            (pottery_cast(uint32_t, input->pos[2]) << 16) |
            (pottery_cast(uint32_t, input->pos[1]) << 8) |
            (pottery_cast(uint32_t, input->pos[0])));

    input->pos += 4;
    return ret;
}

static inline bool fuzz_ufo_init(ufo_t* ufo, fuzz_input_t* input) {
    if (input->end == input->pos)
        return false;

    // generate a random string
    char string[FUZZ_UFO_NAME_MAX_LENGTH + 1];
    size_t len = pottery_cast(size_t, *(input->pos++)) % (FUZZ_UFO_NAME_MAX_LENGTH - 3) + 3;
    size_t i;
    for (i = 0; i < len; ++i) {
        if (input->end == input->pos)
            return false;
        string[i] = pottery_cast(char, 'a' + *(input->pos++) % ('z' - 'a'));
    }
    string[len] = '\0';

    // generate a random integer
    int16_t integer = pottery_cast(int16_t, fuzz_load_u16(input));
    if (input->end == input->pos)
        return false;

    ufo_init(ufo, string, integer);
    return true;
}

#endif
