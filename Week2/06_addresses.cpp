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
//       your answer:
//
//   Q2. Do `speed` and `limit` live at the same address? Why must that be so?
//       your answer:
//
//   Q3. Run the program twice. Are the addresses the same both times?
//       your answer:
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

    // TODO: print one line saying what `limit` holds and where it lives,
    // in the same style as the two lines above.

    // Two different variables are two different boxes, so two different
    // addresses. Comparing addresses with == is legal; this is a real check.
    CHECK_TRUE(&speed != &limit);

    // And the SAME variable always gives the SAME address.
    CHECK_TRUE(&speed == &speed);

    return check::summary();
}
