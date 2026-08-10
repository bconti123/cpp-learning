// Exercise 3: Reverse a string
// Topics: pointers, arrays, references, const-correctness.
//
// TASK: implement the functions marked TODO. Run `make test` until green.
//
// This is the most embedded-relevant exercise of the five. On a microcontroller
// you frequently get handed a raw buffer and a length -- no std::string, no heap
// -- and you must edit it in place. Get comfortable here.

#include <cstddef>  // std::size_t
#include <cstring>  // std::strlen
#include <string>

#include "check.hpp"

// ---------------------------------------------------------------------------
// Part A: the std::string version (fine on Linux, rare in firmware).
// ---------------------------------------------------------------------------

// Return a reversed COPY. The parameter is a const reference: `const&` avoids
// copying the input, and `const` promises you won't modify the caller's data.
std::string reversed(const std::string& input) {
    (void)input;
    return "";  // TODO
}

// ---------------------------------------------------------------------------
// Part B: the in-place buffer version (this is the firmware way).
//
// Reverse the `len` characters starting at `buffer`. No allocation, no copy.
// Walk one pointer from the front and one from the back, swapping as they move,
// and stop when they meet.
//
// Handle len == 0 correctly. Note that `buffer` may not be null-terminated --
// you are given the length, so use it and nothing else.
// ---------------------------------------------------------------------------
void reverse_in_place(char* buffer, std::size_t len) {
    (void)buffer;
    (void)len;
    // TODO
}

// Return true if `text` reads the same forwards and backwards.
// Do NOT allocate a reversed copy -- compare from both ends inwards.
// An empty string and a 1-character string are both palindromes.
bool is_palindrome(const char* text, std::size_t len) {
    (void)text;
    (void)len;
    return false;  // TODO
}

int main() {
    // Part A
    CHECK_EQ(reversed("abc"), std::string("cba"));
    CHECK_EQ(reversed(""), std::string(""));
    CHECK_EQ(reversed("a"), std::string("a"));
    CHECK_EQ(reversed("hello world"), std::string("dlrow olleh"));

    // Part B -- a real char array, like a UART receive buffer
    char buf[16] = "embedded";
    reverse_in_place(buf, std::strlen(buf));
    CHECK_EQ(std::string(buf), std::string("deddebme"));

    char even[] = "abcd";
    reverse_in_place(even, 4);
    CHECK_EQ(std::string(even), std::string("dcba"));

    char one[] = "z";
    reverse_in_place(one, 1);
    CHECK_EQ(std::string(one), std::string("z"));

    char empty[] = "";
    reverse_in_place(empty, 0);  // must not crash or write out of bounds
    CHECK_EQ(std::string(empty), std::string(""));

    // Reversing a slice of a larger buffer: only the first 3 chars move.
    char partial[] = "abcdef";
    reverse_in_place(partial, 3);
    CHECK_EQ(std::string(partial), std::string("cbadef"));

    CHECK_TRUE(is_palindrome("racecar", 7));
    CHECK_TRUE(is_palindrome("abba", 4));
    CHECK_TRUE(is_palindrome("x", 1));
    CHECK_TRUE(is_palindrome("", 0));
    CHECK_TRUE(!is_palindrome("abca", 4));
    CHECK_TRUE(!is_palindrome("ab", 2));

    return check::summary();
}
