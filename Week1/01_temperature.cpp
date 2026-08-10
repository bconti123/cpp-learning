// Exercise 1: Temperature converter
// Topics: functions, data types, floating point, integer math.
//
// TASK: implement the four functions marked TODO. Run `make test` until green.

#include <cstdint>

#include "check.hpp"

// ---------------------------------------------------------------------------
// Part A: the obvious floating-point version.
// ---------------------------------------------------------------------------

// F = C * 9/5 + 32
double celsius_to_fahrenheit(double celsius) {
    (void)celsius;  // silences "unused parameter" until you implement this
    double fahrenheit = (celsius * 9.0/5.0) + 32.0;
    return fahrenheit;
}

// C = (F - 32) * 5/9
double fahrenheit_to_celsius(double fahrenheit) {
    (void)fahrenheit;
    double celsius = (fahrenheit - 32.0) * 5.0/9.0;
    return celsius;
}

// ---------------------------------------------------------------------------
// Part B: the version you'd actually ship on a microcontroller.
//
// Many MCUs have no hardware floating-point unit. A single `double` multiply
// becomes a software routine costing hundreds of cycles. So embedded code keeps
// fixed-point integers: store tenths of a degree in an int16_t.
//
//   25.4 C  is stored as  254
//
// Watch the overflow: intermediate products can exceed int16_t's range of
// -32768..32767 even when the final answer fits. Think about what type the
// arithmetic should happen in.
// ---------------------------------------------------------------------------

// Input:  temperature in tenths of a degree Celsius   (254 == 25.4 C)
// Output: temperature in tenths of a degree Fahrenheit (777 == 77.7 F)
// Round to the nearest tenth (not truncate).
int16_t celsius_deci_to_fahrenheit_deci(int16_t celsius_deci) {
    (void)celsius_deci;
    int32_t fahrenheit_deci = (celsius_deci * 9 / 5) + 320;
    return static_cast<int16_t>(fahrenheit_deci);
}

// Inverse of the above.
int16_t fahrenheit_deci_to_celsius_deci(int16_t fahrenheit_deci) {
    (void)fahrenheit_deci;
    int32_t celsius_deci = (fahrenheit_deci - 320) * 5 / 9;
    return static_cast<int16_t>(celsius_deci);
}

int main() {
    // Part A
    CHECK_NEAR(celsius_to_fahrenheit(0.0), 32.0, 1e-9);
    CHECK_NEAR(celsius_to_fahrenheit(100.0), 212.0, 1e-9);
    CHECK_NEAR(celsius_to_fahrenheit(-40.0), -40.0, 1e-9);
    CHECK_NEAR(celsius_to_fahrenheit(37.0), 98.6, 1e-9);

    CHECK_NEAR(fahrenheit_to_celsius(32.0), 0.0, 1e-9);
    CHECK_NEAR(fahrenheit_to_celsius(212.0), 100.0, 1e-9);
    CHECK_NEAR(fahrenheit_to_celsius(-40.0), -40.0, 1e-9);

    // Part B -- fixed point
    CHECK_EQ(celsius_deci_to_fahrenheit_deci(0), 320);
    CHECK_EQ(celsius_deci_to_fahrenheit_deci(1000), 2120);
    CHECK_EQ(celsius_deci_to_fahrenheit_deci(-400), -400);
    CHECK_EQ(celsius_deci_to_fahrenheit_deci(254), 777);   // 25.4C -> 77.72F -> 77.7F
    CHECK_EQ(celsius_deci_to_fahrenheit_deci(-100), 140);  // -10.0C -> 14.0F

    CHECK_EQ(fahrenheit_deci_to_celsius_deci(320), 0);
    CHECK_EQ(fahrenheit_deci_to_celsius_deci(2120), 1000);
    CHECK_EQ(fahrenheit_deci_to_celsius_deci(-400), -400);
    CHECK_EQ(fahrenheit_deci_to_celsius_deci(986), 370);  // 98.6F -> 37.0C

    return check::summary();
}
