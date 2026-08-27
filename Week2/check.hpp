// A ~50-line test harness. Deliberately tiny -- you should be able to read and
// understand every line of your own tooling.
//
// Usage:
//   #include "check.hpp"
//   int main() {
//       CHECK_EQ(add(2, 3), 5);
//       CHECK_NEAR(c_to_f(100.0), 212.0, 1e-9);
//       return check::summary();   // exit code 0 = all passed
//   }
#pragma once

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

namespace check {

// `inline` on a variable is C++17. It lets a header define a single shared
// object without a matching .cpp file. Before C++17 this needed extern + a .cpp.
inline int run = 0;
inline int failed = 0;

template <typename T>
std::string to_text(const T& value) {
    std::ostringstream out;
    out << value;
    return out.str();
}

inline void record(bool ok, const std::string& expr, const std::string& got,
                   const std::string& want, int line) {
    ++run;
    if (ok) {
        return;
    }
    ++failed;
    std::cout << "  FAIL (line " << line << "): " << expr << "\n"
              << "        got:      " << got << "\n"
              << "        expected: " << want << "\n";
}

template <typename A, typename B>
void eq(const A& got, const B& want, const std::string& expr, int line) {
    record(got == want, expr, to_text(got), to_text(want), line);
}

inline void near(double got, double want, double tol, const std::string& expr, int line) {
    // Never compare floating point with ==. Rounding makes 0.1 + 0.2 != 0.3.
    record(std::fabs(got - want) <= tol, expr, to_text(got), to_text(want), line);
}

inline int summary() {
    if (failed == 0) {
        std::cout << "  ok: " << run << " checks passed\n";
        return EXIT_SUCCESS;
    }
    std::cout << "  " << failed << " of " << run << " checks FAILED\n";
    return EXIT_FAILURE;
}

}  // namespace check

#define CHECK_EQ(got, want) ::check::eq((got), (want), #got " == " #want, __LINE__)
#define CHECK_NEAR(got, want, tol) ::check::near((got), (want), (tol), #got " ~= " #want, __LINE__)
#define CHECK_TRUE(expr) ::check::eq(static_cast<bool>(expr), true, #expr, __LINE__)
