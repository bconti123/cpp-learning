// Week 2, Lesson 1: What a "type" actually is
//
// A variable is a labelled box in memory. Its TYPE decides two things:
//
//   1. How BIG the box is        (how many bytes)
//   2. How the bits are READ     (as a whole number? a letter? a fraction?)
//
// That is the whole idea. Everything else about data types is detail.
//
// TASK
//   Part A: fill in your GUESS for each size in the table below. Guess first,
//           before you build. Being wrong here is the point -- a wrong guess
//           you then correct sticks; a right answer you looked up does not.
//   Part B: replace each `0` in the CHECKs at the bottom with the same guess.
//   Part C: run `make run-01_sizes`. The program prints the real sizes and the
//           CHECKs tell you which guesses were off.
//
// Do NOT look up the answers. Guess, run, then we talk about the surprises.

#include <cstddef>
#include <iostream>
#include <string>

#include "check.hpp"

// ---------------------------------------------------------------------------
// Part A -- your guesses, in bytes. (1 byte = 8 bits.)
//
//   type          your guess
//   ----          ----------
//   bool          1 byte
//   char          1 byte
//   int           4 bytes
//   double        8 bytes
//
// ---------------------------------------------------------------------------

int main() {
    // `sizeof(T)` asks the compiler: how many bytes does one T take?
    // It is answered at COMPILE time -- nothing is measured while running.
    std::cout << "sizeof(bool)        = " << sizeof(bool) << " bytes\n";
    std::cout << "sizeof(char)        = " << sizeof(char) << " bytes\n";
    std::cout << "sizeof(int)         = " << sizeof(int) << " bytes\n";
    std::cout << "sizeof(double)      = " << sizeof(double) << " bytes\n";
    std::cout << "sizeof(std::string) = " << sizeof(std::string) << " bytes\n";
    std::cout << "\n";

    // Part B: replace each 0 with your guess from the table above.
    // The `u` suffix just means "this number is unsigned", which matches the
    // type sizeof gives back. Write `4u`, not `4`.
    CHECK_EQ(sizeof(bool), 1u);
    CHECK_EQ(sizeof(char), 1u);
    CHECK_EQ(sizeof(int), 4u);
    CHECK_EQ(sizeof(double), 8u);

    // Not a TODO -- this one is guaranteed by the standard on every machine
    // that has ever existed. `char` is DEFINED as "one byte". Whatever a byte
    // is on the hardware, char is exactly one of them.
    //
    // Its SIGNEDNESS is a separate question, and is NOT guaranteed:
    // signed here (x86 + g++, -128..127), unsigned on ARM (0..255).
    CHECK_EQ(sizeof(char), 1u);

    return check::summary();
}
