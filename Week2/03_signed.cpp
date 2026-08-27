// Week 2, Lesson 3: signed vs unsigned
//
// 8 bits = 256 patterns. Always. The only question is what they MEAN:
//
//     uint8_t:     0 ...................... 255      (all positive)
//     int8_t:   -128 ......... 0 ......... 127       (half spent on negatives)
//
// TASK
//   Step 1: write your predictions in the table below. IN WRITING, first.
//   Step 2: copy those same numbers into the `0`s marked TODO.
//   Step 3: build. The failures tell you which predictions were off.
//
// Do step 1 before step 3. The build output gives the answers away, so the
// only way this teaches you anything is if your guess is already committed to
// the page. A wrong guess you then correct is the entire point.
//
//   ---------------------------------------------------------------
//   PREDICTIONS                                        your answer
//   ---------------------------------------------------------------
//   biggest value an int8_t can hold 127 ............
//   smallest value an int8_t can hold -128 ..........
//   biggest value a uint8_t can hold 255 ............
//
//   a uint8_t holding 255, plus 1, becomes 0 ........
//   a uint8_t holding 0, minus 1, becomes 255 ........
//   a uint16_t holding 65535, plus 1, becomes 0 .......
//   ---------------------------------------------------------------

#include <cstdint>
#include <iostream>

#include "check.hpp"

int main() {
    // -----------------------------------------------------------------------
    // Part A -- ranges.
    //
    // These names come from <cstdint>. INT8_MAX is "the biggest value an
    // int8_t can hold", and so on. Fill in the numbers.
    // -----------------------------------------------------------------------
    CHECK_EQ(INT8_MAX, 127);
    CHECK_EQ(INT8_MIN, -128);
    CHECK_EQ(UINT8_MAX, 255);

    // (A uint8_t's smallest value is 0. That is the whole point of unsigned --
    // there is nothing below zero, so there is nothing to predict.)

    // -----------------------------------------------------------------------
    // Part B -- wraparound. The odometer.
    // -----------------------------------------------------------------------

    // Why static_cast here? `up + 1` is computed as an int (a uint8_t gets
    // promoted before any arithmetic -- more on that in Lesson 5). Storing an
    // int back into a uint8_t narrows it, and -Wconversion makes that an
    // error unless you say "yes, I meant it" with the cast.
    uint8_t up = 255;
    up = static_cast<uint8_t>(up + 1);
    CHECK_EQ(static_cast<int>(up), 0);  // 255 + 1 wraps to 0

    uint8_t down = 0;
    down = static_cast<uint8_t>(down - 1);
    CHECK_EQ(static_cast<int>(down), 255);  // 0 - 1 wraps to 255

    // Why static_cast<int> inside the CHECK? Because uint8_t IS unsigned char,
    // so printing one sends a *character* to the screen, not a number. The
    // cast is only so the failure message is readable.
    std::cout << "up   = " << static_cast<int>(up) << "\n"
              << "down = " << static_cast<int>(down) << "\n";

    // -----------------------------------------------------------------------
    // Part C -- the same thing, one size up.
    //
    // A 16-bit counter, the sort of thing that holds a tick count.
    // -----------------------------------------------------------------------
    uint16_t ticks = UINT16_MAX;
    ticks = static_cast<uint16_t>(ticks + 1);
    CHECK_EQ(static_cast<int>(ticks), 0);  // 65535 + 1 wraps to 0

    return check::summary();
}
