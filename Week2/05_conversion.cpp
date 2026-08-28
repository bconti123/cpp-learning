// Week 2, Lesson 5: assignment and conversion
//
// You have been copying `static_cast` since Lesson 3 without being told what
// it does. Here is the whole story.
//
// When you assign a value to a variable of a DIFFERENT type, the value gets
// converted. There are two cases, and the compiler treats them very
// differently.
//
//   WIDENING -- every value of the source type fits in the destination.
//               int8_t (-128..127) into an int32_t: nothing can go wrong.
//               The compiler does it silently, because it is always safe.
//
//   NARROWING -- some values of the source type do NOT fit.
//               int32_t into a uint8_t: 300 has nowhere to go.
//               The compiler warns. Our -Wconversion -Werror turns that
//               warning into a build failure.
//
// `static_cast<T>(value)` is how you say "I know this narrows. I meant it."
//
//     IT DOES NOT MAKE THE CONVERSION SAFE.
//     IT DOES NOT CHANGE THE RESULT.
//     It only silences the warning.
//
// The value is mangled exactly the same way with or without the cast. All the
// cast does is move the responsibility from the compiler to you. That is why
// embedded code is full of them and why each one deserves a glance.
//
// TASK
//   Step 1: fill in your predictions below, IN WRITING, before you build.
//   Step 2: copy those same numbers into the `0`s marked TODO.
//   Step 3: build. The failures tell you which predictions were off.
//
//   ---------------------------------------------------------------------
//   what happens when you...                          your prediction
//   ---------------------------------------------------------------------
//   store 3.9 into an int                             ...............
//   store 300 into a uint8_t                          ...............
//   store -1 into a uint16_t                          ...............
//   compute 7 / 2 and store it in a double            ...............
//   ---------------------------------------------------------------------
//
// Hints, not answers: rows 2 and 3 are Lesson 3's odometer again. Row 1 is
// not rounding. Row 4 is the one that catches everybody, and neither the
// compiler nor -Wconversion will say a word about it.

#include <cstdint>
#include <iostream>

#include "check.hpp"

int main() {
    // -----------------------------------------------------------------------
    // Part A -- widening. Nothing to do, just read.
    // -----------------------------------------------------------------------

    int8_t small_value = 100;

    // int8_t holds -128..127. Every one of those fits in an int32_t, so this
    // is widening: no cast, no warning, value arrives intact.
    int32_t wide = small_value;
    CHECK_EQ(wide, 100);

    // Same story going to double. Silence from the compiler means "I checked,
    // this cannot lose anything."
    double as_double = wide;
    CHECK_NEAR(as_double, 100.0, 1e-9);

    // -----------------------------------------------------------------------
    // Part B -- narrowing. Three ways to lose a value.
    // Replace each 0 marked TODO with your prediction.
    // -----------------------------------------------------------------------

    // (1) A fraction into a whole number. The decimal part is DISCARDED.
    double measured = 3.9;
    int truncated = static_cast<int>(measured);
    CHECK_EQ(truncated, 0);  // TODO: prediction row 1

    // Try this: delete the two slashes below, run `make run-05_conversion`,
    // read the error, then put them back. That error is -Wconversion earning
    // its place in the Makefile -- it is the very same conversion as the line
    // above, minus your signature on it.
    // truncated = measured;

    // (2) A value too big for the box. 300 does not fit in 0..255.
    int sensor_reading = 300;
    uint8_t register_field = static_cast<uint8_t>(sensor_reading);
    CHECK_EQ(static_cast<int>(register_field), 0);  // TODO: prediction row 2

    // (3) A negative into an unsigned. There is no negative half to land in,
    // so the bits get re-read as a positive number.
    int error_code = -1;
    uint16_t as_unsigned = static_cast<uint16_t>(error_code);
    CHECK_EQ(static_cast<int>(as_unsigned), 0);  // TODO: prediction row 3

    std::cout << "3.9 -> int        = " << truncated << "\n"
              << "300 -> uint8_t    = " << static_cast<int>(register_field) << "\n"
              << "-1  -> uint16_t   = " << as_unsigned << "\n\n";

    // -----------------------------------------------------------------------
    // Part C -- the quiet one.
    //
    // No cast here. No warning here. -Wconversion says nothing. This is the
    // single most common beginner bug in C++, and the compiler helps you not
    // at all.
    // -----------------------------------------------------------------------

    int total = 7;
    int parts = 2;

    // Read the right-hand side ON ITS OWN first. What are `total` and `parts`?
    // Two ints. So this is int division, and int division has no fractional
    // part to give you -- it happens BEFORE anything is stored in a double.
    // The widening to double happens afterwards, too late to help.
    double average = total / parts;
    CHECK_NEAR(average, 0.0, 1e-9);  // TODO: prediction row 4

    // The fix: make ONE side a double before dividing. Then it is double
    // division, and the other side gets widened up to match.
    double proper_average = static_cast<double>(total) / parts;
    CHECK_NEAR(proper_average, 3.5, 1e-9);

    std::cout << "7 / 2 into a double            = " << average << "\n"
              << "(double)7 / 2 into a double    = " << proper_average << "\n";

    return check::summary();
}
