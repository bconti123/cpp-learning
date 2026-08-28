// Week 2, Lesson 4: fixed-width types
//
// Lesson 1 asked "how big is an int?" and your machine answered 4 bytes.
// Here is the uncomfortable part: THAT IS NOT A PROMISE.
//
// The C++ standard only says an `int` is *at least* 16 bits. On your laptop
// it happens to be 32. On an AVR (Arduino Uno) or an MSP430 it is 16. Same
// source file, same `int`, different size -- and nothing warns you.
//
//     int counter = 40000;    // fine on a laptop
//                             // on an AVR, int maxes out at 32767, so this
//                             // silently wraps to -25536
//
// So <cstdint> gives you names that say the size out loud:
//
//     int8_t   int16_t   int32_t   int64_t      signed, exactly N bits
//     uint8_t  uint16_t  uint32_t  uint64_t     unsigned, exactly N bits
//
// `uint16_t` is 16 bits on your laptop, on an AVR, on an STM32, everywhere.
// If a platform cannot provide that exact width, the name does not exist and
// your build fails loudly instead of misbehaving quietly.
//
// WHY EMBEDDED INSISTS ON THESE
//   1. Hardware registers are exactly N bits wide. A control register is a
//      uint32_t because the silicon says it is 32 bits.
//   2. RAM is measured in kilobytes. Spending 4 bytes on a 0..100 battery
//      percentage, 200 times over, is real money.
//   3. Bytes going out over a wire (I2C, CAN, a radio packet) have a layout
//      the receiver already agreed to. "Whatever int is here" is not a layout.
//
// TASK
//   Part B has four variables whose types have been replaced with
//   FILL_IN_THE_TYPE. The file does not compile until you replace all four.
//   For each one, pick the SMALLEST fixed-width type that can hold the whole
//   stated range -- including the negative part, where there is one.
//
//   Write your four answers in this table first, then copy them into the code.
//
//   ---------------------------------------------------------------------
//   value                       range needed          your type
//   ---------------------------------------------------------------------
//   battery percent             0 .. 100              uint8_t
//   temperature in C          -40 .. 125              int8_t
//   pressure in Pa              0 .. 65535            uint16_t
//   uptime in seconds           0 .. 4000000000       uint32_t
//   ---------------------------------------------------------------------
//
// Two questions per row, in this order:
//   (1) Does the range go below zero?  -> signed (intN_t) or unsigned (uintN_t). Answer is signed intN_t for the range goes below zero.
//   (2) How many bits does the biggest value need?
//       8 bits unsigned reaches 255, 16 reaches 65535, 32 reaches ~4.29 billion.
//       8 bits signed reaches 127, 16 reaches 32767. 32 signed reaches ~2.14 billion.

#include <cstdint>
#include <iostream>

#include "check.hpp"

int main() {
    // -----------------------------------------------------------------------
    // Part A -- the problem, demonstrated. Nothing to do here, just read it.
    // -----------------------------------------------------------------------

    // `int` is whatever this compiler on this machine felt like. Print it.
    std::cout << "on THIS machine, sizeof(int)     = " << sizeof(int) << " bytes\n";
    std::cout << "                 sizeof(long)    = " << sizeof(long) << " bytes\n";
    std::cout << "                 sizeof(int32_t) = " << sizeof(int32_t) << " bytes\n\n";

    // These two checks look identical. They are not.
    //
    // The first is true on your laptop today and would FAIL on an AVR. It is a
    // fact about this machine.
    CHECK_EQ(sizeof(int), 4u);

    // The second is true on every machine that can compile this file at all.
    // It is a fact about the type. That difference is the whole lesson.
    CHECK_EQ(sizeof(int32_t), 4u);

    // -----------------------------------------------------------------------
    // Part B -- a sensor packet. Replace the four FILL_IN_THE_TYPE markers.
    //
    // Do not change the values, the names, or the CHECKs. Only the types.
    // -----------------------------------------------------------------------

    // (a) Battery charge as a whole-number percentage, 0 to 100.
    uint8_t battery_percent = 100;

    // (b) Ambient temperature in whole degrees C. This board is rated for
    //     industrial use: -40 up to 125.
    int8_t temperature_c = -40;

    // (c) Pressure in pascals above ambient, 0 to 65535.
    uint16_t pressure_pa = 65535;

    // (d) Seconds since power-on. Runs up to 4000000000 before we stop caring.
    uint32_t uptime_seconds = 4000000000;

    // Remember from Lesson 3: uint8_t IS unsigned char, so printing one sends
    // a character to the screen. static_cast<int> is only so this is readable.
    std::cout << "battery:     " << static_cast<int>(battery_percent) << " %\n"
              << "temperature: " << static_cast<int>(temperature_c) << " C\n"
              << "pressure:    " << pressure_pa << " Pa\n"
              << "uptime:      " << uptime_seconds << " s\n\n";

    // The values must survive being stored. If (b) came out as 216 instead of
    // -40, you picked an unsigned type for a range that goes below zero.
    CHECK_EQ(static_cast<int>(battery_percent), 100);
    CHECK_EQ(static_cast<int>(temperature_c), -40);
    CHECK_EQ(static_cast<int>(pressure_pa), 65535);
    CHECK_EQ(uptime_seconds, 4000000000u);

    // And they must be the SMALLEST type that fits. If a size check fails,
    // your type is wider than it needs to be.
    CHECK_EQ(sizeof(battery_percent), 1u);
    CHECK_EQ(sizeof(temperature_c), 1u);
    CHECK_EQ(sizeof(pressure_pa), 2u);
    CHECK_EQ(sizeof(uptime_seconds), 4u);

    // 1 + 1 + 2 + 4 = 8 bytes for the whole packet. Four plain `int`s would
    // have been 16 on this machine, and an unknown number on the target.
    std::cout << "packet size: "
              << sizeof(battery_percent) + sizeof(temperature_c) +
                     sizeof(pressure_pa) + sizeof(uptime_seconds)
              << " bytes\n";

    return check::summary();
}
