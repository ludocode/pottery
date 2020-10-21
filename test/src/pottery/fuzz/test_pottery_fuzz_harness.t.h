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

/*
 * This is the harness for a Pottery fuzz test. It defines either a main()
 * function that takes bytes from standard input or a Pottery unit test that
 * generates random bytes.
 */



#ifdef TEST_POTTERY_FUZZ_MAIN

// Read fuzzer data from stdin
int main(void) {
    size_t max_length = 1024*1024*128;
    uint8_t* data = pottery_cast(uint8_t*, malloc(max_length));
    uint8_t* end = data;

    while (true) {
        size_t remaining = pottery_cast(size_t, data + max_length - end);
        size_t count = fread(end, 1, remaining, stdin);
        //printf("remaining %zu count %zu\n", remaining, count);
        if (count == 0)
            break;
        end += count;
    }
    //printf("loaded %zu bytes\n", pottery_cast(size_t, end - data));

    fuzz_input_t input = {data, end};
    fuzz(&input);

    free(data);
    return EXIT_SUCCESS;
}

#else

// Generate random data for fuzzer
#ifndef TEST_POTTERY_FUZZ_TEST_NAME
    #error "TEST_POTTERY_FUZZ_TEST_NAME is not defined!"
#endif

POTTERY_TEST(TEST_POTTERY_FUZZ_TEST_NAME) {
    size_t length = 32*1024;
    uint8_t* data = pottery_cast(uint8_t*, malloc(length));
    uint8_t* end = data + length;

    // fuzz test this many rounds
    size_t rounds = 20;
    size_t round;
    for (round = 0; round < rounds; ++round) {

        // generate random data
        size_t i = 0;
        for (i = 0; i < length; ++i)
            data[i] = pottery_cast(uint8_t, rand());

        // run the fuzz tester on it
        fuzz_input_t input = {data, end};
        fuzz(&input);

    }

    free(data);
}
#undef TEST_POTTERY_FUZZ_TEST_NAME
#endif
