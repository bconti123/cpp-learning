// Week 2, Lesson 2: choosing the right type
//
// Every variable below is missing its type -- the placeholder
// FILL_IN_THE_TYPE marks the blank. The file does NOT compile until you have
// replaced all five. That is expected; read the compiler error, it names the
// line.
//
// For each one, answer this in plain English BEFORE you write any C++:
//
//     What kind of thing is this?
//         counting whole things?  -> int / int32_t
//         measuring something?    -> float / double
//         yes or no?              -> bool
//         one single letter?      -> char
//         words, a name, text?    -> std::string
//
// Decide the row first. Then write the type.
//
// Do not change the values, the names, or the CHECKs. Only the types.

#include <cstdint>
#include <iostream>
#include <string>

#include "check.hpp"

int main() {
    // (a) How many times a button has been pressed since power-on.
    int press_count = 3;

    // (b) Whether the motor is spinning right now.
    bool motor_spinning = true;

    // (c) The temperature read from a sensor, in degrees C.
    float temperature_c = 21.7f;

    // (d) The model name printed on the board.
    std::string board_name = "STM32F4";

    // (e) A single letter the user typed.
    char typed_letter = 'K';

    std::cout << "board:  " << board_name << "\n"
              << "temp:   " << temperature_c << " C\n"
              << "presses:" << press_count << "\n"
              << "letter: " << typed_letter << "\n"
              << "motor:  " << motor_spinning << "\n\n";

    CHECK_EQ(press_count, 3);
    CHECK_TRUE(motor_spinning);
    CHECK_NEAR(temperature_c, 21.7, 0.001);
    CHECK_EQ(board_name, "STM32F4");
    CHECK_EQ(typed_letter, 'K');

    // One letter is one byte. If this check fails, (e) is not a char.
    CHECK_EQ(sizeof(typed_letter), 1u);

    return check::summary();
}
