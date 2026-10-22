// Week 2, Lesson 6: addresses
//
// Every variable is a box somewhere in memory. So far you have only cared
// about what is IN the box. Today: WHERE the box is.
//
// Memory is one long row of bytes, and every byte has a number -- its
// address. A variable's address is the number of its first byte.
//
// `&x` (in an expression, in front of a value) asks: "where does x live?"
//
// That is all `&` means today. No pointers yet -- we only LOOK at addresses.
// Storing one in a variable is Lesson 7.
//
// Why embedded cares: on a microcontroller, hardware registers sit at fixed
// addresses from the datasheet (e.g. "GPIOA output register at 0x48000014").
// Talking to hardware IS reading and writing particular addresses. This lesson
// is the first step toward that.
//
// Addresses print in hex (0x7ffd...). They are big because a 64-bit PC has a
// huge address space. Do not try to read meaning into the digits.
//
// TASK
//   Step 1: build and run as-is:   make run-06_addresses
//   Step 2: do the TODO in main() -- print where `limit` lives.
//   Step 3: run it TWICE more and answer these, in your own words:
//
//   Q1. Did the address of `speed` change when its value changed from 40 to 99?
//       your answer: No, the address of `speed` did not change when its value changed from 40 to 99. The variable `speed` is stored in a specific location in memory, and changing its value does not affect its memory address.
//
//   Q2. Do `speed` and `limit` live at the same address? Why must that be so?
//       your answer: No, `speed` and `limit` live at different addresses. Each variable is stored in its own memory location, so they must have distinct addresses.
//
//   Q3. Run the program twice. Are the addresses the same both times?
//       your answer: No. The addresses are not the same both times.
//
// Hint for Q3: the answer on your PC is not the answer on a bare
// microcontroller. Just report what you see; we will talk about why.

#include <cstdint>
#include <iostream>

#include "check.hpp"

int main() {
    int32_t speed = 40;
    int32_t limit = 65;

    std::cout << "speed holds " << speed << " and lives at " << &speed << "\n";

    speed = 99;

    std::cout << "speed holds " << speed << " and lives at " << &speed << "\n";

    std::cout << "limit holds " << limit << " and lives at " << &limit << "\n";

    limit = 120;

    std::cout << "limit holds " << limit << " and lives at " << &limit << "\n";

    // Two different variables are two different boxes, so two different
    // addresses. Comparing addresses with == is legal; this is a real check.
    CHECK_TRUE(&speed != &limit);

    // And the SAME variable always gives the SAME address.
    CHECK_TRUE(&speed == &speed);

    return check::summary();
}
