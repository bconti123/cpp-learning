// Exercise 4: Find min / max in an array
// Topics: arrays, pointers, structs, references, loops.
//
// TASK: implement the functions marked TODO. Run `make test` until green.
//
// DESIGN NOTE: notice there is no `std::vector` here. In firmware, arrays are
// usually fixed-size buffers and get passed as (pointer, length). Losing the
// length is the #1 source of buffer overruns in C and C++.

#include <cstddef>
#include <cstdint>

#include "check.hpp"

struct MinMax {
    int min;
    int max;
};

// Scan `count` ints starting at `data` and return the smallest and largest.
//
// Do it in ONE pass. Scanning twice is twice the memory traffic, which on an
// MCU reading from a slow sensor buffer is a real cost.
//
// Precondition: count >= 1. (Think about why: with zero elements there is no
// sensible min or max to return. Documenting a precondition is a valid design
// choice -- the alternative is returning a status, as in exercise 2.)
MinMax find_min_max(const int* data, std::size_t count) {
    if (count == 0) {
        // Precondition violation: no elements to scan. In a real firmware
        // function you might assert() here, or return an error code.
        return {0, 0};
    }
    MinMax result = {data[0], data[0]};
    for (std::size_t i = 1; i < count; ++i) {
        if (data[i] < result.min) {
            result.min = data[i];
        }
        if (data[i] > result.max) {
            result.max = data[i];
        }
    }
    return result;
}

// Same idea, but reports through OUT PARAMETERS instead of a return value.
// `min_out` and `max_out` are references: writing to them writes to the
// caller's variables directly. This is the C-style API you'll meet constantly
// in vendor HAL code.
void find_min_max_refs(const int* data, std::size_t count, int& min_out, int& max_out) {
    if (count == 0) {
        // Precondition violation: no elements to scan. In a real firmware
        // function you might assert() here, or return an error code.
        min_out = 0;
        max_out = 0;
        return;
    }

    min_out = data[0];
    max_out = data[0];

    for (std::size_t i = 1; i < count; ++i) {
        if (data[i] < min_out) {
            min_out = data[i];
        }
        if (data[i] > max_out) {
            max_out = data[i];
        }
    }
}

// Return the average of `count` samples, rounded toward zero.
//
// CAREFUL: this is the classic embedded bug. If you accumulate 12-bit ADC
// samples into an int16_t, a few thousand readings will overflow it and your
// average will be nonsense (and signed overflow is undefined behavior, so the
// compiler may do something genuinely surprising). Pick an accumulator type
// wide enough to hold the total.
int32_t average(const int16_t* samples, std::size_t count) {
    if (count == 0) {
        // Precondition violation: no samples to average. In a real firmware
        // function you might assert() here, or return an error code.
        return 0;
    }

    int64_t sum = 0;

    for (std::size_t i = 0; i < count; ++i) {
        sum += samples[i];
    }
    return static_cast<int32_t>(sum / static_cast<int64_t>(count)); // integer division rounds toward zero
}

int main() {
    const int a[] = {5, 3, 9, 1, 7};
    MinMax r = find_min_max(a, 5);
    CHECK_EQ(r.min, 1);
    CHECK_EQ(r.max, 9);

    const int single[] = {42};
    CHECK_EQ(find_min_max(single, 1).min, 42);
    CHECK_EQ(find_min_max(single, 1).max, 42);

    const int negatives[] = {-5, -3, -9, -1};
    CHECK_EQ(find_min_max(negatives, 4).min, -9);
    CHECK_EQ(find_min_max(negatives, 4).max, -1);

    const int same[] = {7, 7, 7};
    CHECK_EQ(find_min_max(same, 3).min, 7);
    CHECK_EQ(find_min_max(same, 3).max, 7);

    // min/max at the very ends -- catches an off-by-one in your loop bounds
    const int ends[] = {100, 5, 5, 5, -100};
    CHECK_EQ(find_min_max(ends, 5).min, -100);
    CHECK_EQ(find_min_max(ends, 5).max, 100);

    int lo = 0;
    int hi = 0;
    find_min_max_refs(a, 5, lo, hi);
    CHECK_EQ(lo, 1);
    CHECK_EQ(hi, 9);

    const int16_t s1[] = {10, 20, 30};
    CHECK_EQ(average(s1, 3), 20);

    const int16_t s2[] = {1, 2};
    CHECK_EQ(average(s2, 2), 1);  // 1.5 -> rounds toward zero -> 1

    const int16_t s3[] = {-10, 10};
    CHECK_EQ(average(s3, 2), 0);

    // 1000 samples near the top of int16_t: sum is 32'000'000, which would
    // destroy a 16-bit accumulator.
    int16_t big[1000];
    for (std::size_t i = 0; i < 1000; ++i) {
        big[i] = 32000;
    }
    CHECK_EQ(average(big, 1000), 32000);

    return check::summary();
}
