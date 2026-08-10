// Exercise 5: Menu-driven program
// Topics: enums, switch, loops, and the single most useful habit in this file:
//         SEPARATING LOGIC FROM I/O.
//
// TASK: implement the functions marked TODO, then run `make run-05_menu`
//       to use it interactively. Self-tests run first.
//
// DESIGN NOTE -- the actual lesson:
// The interactive loop at the bottom is thin and untestable by nature (it reads
// stdin). Everything that makes a *decision* lives in small pure functions above
// it, which the tests can hammer. In firmware this is how you unit-test code
// that ultimately talks to hardware: the logic doesn't know about the UART.

#include <iostream>
#include <string>

#include "check.hpp"

enum class MenuOption {
    ConvertTemperature,
    ReverseText,
    ShowCounter,
    Quit,
    Invalid,
};

// Map a raw user keystroke to a menu option.
//   '1' -> ConvertTemperature
//   '2' -> ReverseText
//   '3' -> ShowCounter
//   'q' or 'Q' -> Quit
//   anything else -> Invalid
MenuOption parse_option(char key) {
    (void)key;
    return MenuOption::Invalid;  // TODO
}

// The text shown for each option in the menu listing.
//   ConvertTemperature -> "Convert temperature"
//   ReverseText        -> "Reverse text"
//   ShowCounter        -> "Show counter"
//   Quit               -> "Quit"
//   Invalid            -> "Invalid option"
const char* option_label(MenuOption option) {
    (void)option;
    return "";  // TODO
}

// Should the main loop keep running after handling `option`?
// Everything continues except Quit.
bool should_continue(MenuOption option) {
    (void)option;
    return true;  // TODO
}

// ---------------------------------------------------------------------------
// The I/O layer. Already written -- read it, don't change it (yet).
// ---------------------------------------------------------------------------

void print_menu() {
    std::cout << "\n--- Menu ---\n"
              << "  1) " << option_label(MenuOption::ConvertTemperature) << "\n"
              << "  2) " << option_label(MenuOption::ReverseText) << "\n"
              << "  3) " << option_label(MenuOption::ShowCounter) << "\n"
              << "  q) " << option_label(MenuOption::Quit) << "\n"
              << "choice> ";
}

int run_tests() {
    std::cout << "=== self-tests\n";
    CHECK_TRUE(parse_option('1') == MenuOption::ConvertTemperature);
    CHECK_TRUE(parse_option('2') == MenuOption::ReverseText);
    CHECK_TRUE(parse_option('3') == MenuOption::ShowCounter);
    CHECK_TRUE(parse_option('q') == MenuOption::Quit);
    CHECK_TRUE(parse_option('Q') == MenuOption::Quit);
    CHECK_TRUE(parse_option('7') == MenuOption::Invalid);
    CHECK_TRUE(parse_option(' ') == MenuOption::Invalid);

    CHECK_EQ(std::string(option_label(MenuOption::ReverseText)), std::string("Reverse text"));
    CHECK_EQ(std::string(option_label(MenuOption::Quit)), std::string("Quit"));
    CHECK_EQ(std::string(option_label(MenuOption::Invalid)), std::string("Invalid option"));

    CHECK_TRUE(should_continue(MenuOption::ShowCounter));
    CHECK_TRUE(should_continue(MenuOption::Invalid));
    CHECK_TRUE(!should_continue(MenuOption::Quit));
    return check::summary();
}

int main() {
    if (run_tests() != 0) {
        std::cout << "Fix the failing checks before running the menu.\n";
        return 1;
    }

    int counter = 0;
    bool running = true;

    while (running) {
        print_menu();

        std::string line;
        if (!std::getline(std::cin, line)) {
            break;  // stdin closed (Ctrl-D)
        }
        const char key = line.empty() ? '\0' : line[0];
        const MenuOption option = parse_option(key);

        switch (option) {
            case MenuOption::ConvertTemperature: {
                std::cout << "Celsius> ";
                double celsius = 0.0;
                if (std::cin >> celsius) {
                    std::cout << celsius << " C = " << (celsius * 9.0 / 5.0 + 32.0) << " F\n";
                }
                std::cin.ignore(10000, '\n');  // drop the rest of the line
                break;
            }
            case MenuOption::ReverseText: {
                std::cout << "text> ";
                std::string text;
                std::getline(std::cin, text);
                std::cout << std::string(text.rbegin(), text.rend()) << "\n";
                break;
            }
            case MenuOption::ShowCounter:
                std::cout << "counter = " << ++counter << "\n";
                break;
            case MenuOption::Quit:
                std::cout << "bye\n";
                break;
            case MenuOption::Invalid:
                std::cout << "Unrecognized choice: '" << key << "'\n";
                break;
        }

        running = should_continue(option);
    }

    return 0;
}
