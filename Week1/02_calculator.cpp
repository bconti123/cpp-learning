// Exercise 2: Calculator
// Topics: switch, enums, structs, error handling without exceptions.
//
// TASK: implement the functions marked TODO. Run `make test` until green.
//
// DESIGN NOTE -- read this, it's the actual lesson:
// Embedded code usually runs with exceptions disabled (-fno-exceptions). They
// cost flash space and make timing unpredictable. So how does a function report
// failure? It returns a *result struct* carrying both the value and a status.
// You will see this pattern everywhere in real firmware.

#include "check.hpp"

// `enum class` (C++11) instead of a plain `enum`:
//   - its values don't leak into the surrounding scope (no name collisions)
//   - it does NOT implicitly convert to int, so you can't accidentally do
//     arithmetic on a status code
enum class Status {
    Ok,
    DivideByZero,
    UnknownOperator,
};

struct CalcResult {
    double value;
    Status status;
};

// Apply `op` to lhs and rhs.
//   op is one of: '+', '-', '*', '/'
//
// Rules:
//   - division by zero            -> {0.0, Status::DivideByZero}
//   - any other character for op  -> {0.0, Status::UnknownOperator}
//   - otherwise                   -> {result, Status::Ok}
//
// Use a `switch` on op. Remember `break` (or `return`) on every case, and give
// the switch a `default:`.
// Parameter - lhs - left hand side
// Parameter - rhs - right hand side
// Parameter - op - operator
// Example: calculate(6.0, 3.0, '+') returns {9.0, Status::Ok}
CalcResult calculate(double lhs, double rhs, char op) {
    (void)lhs;
    (void)rhs;
    (void)op;
    if (op == '+') {
        return {lhs + rhs, Status::Ok};
    } else if (op == '-') {
        return {lhs - rhs, Status::Ok};
    } else if (op == '*') {
        return {lhs * rhs, Status::Ok};
    } else if (op == '/') {
        if (rhs == 0.0) {
            return {0.0, Status::DivideByZero};
        } else {
            return {lhs / rhs, Status::Ok};
        }
    } else {
        return {0.0, Status::UnknownOperator};
    }
    return {0.0, Status::UnknownOperator};
}

// Return a human-readable name for a status, for printing.
//   Status::Ok              -> "ok"
//   Status::DivideByZero    -> "divide by zero"
//   Status::UnknownOperator -> "unknown operator"
const char* status_name(Status status) {
    (void)status;
    switch (status) {
        case Status::Ok:
            return "ok";
        case Status::DivideByZero:
            return "divide by zero";
        case Status::UnknownOperator:
            return "unknown operator";
    }
    return "";
}

int main() {
    CalcResult r = calculate(6.0, 3.0, '+');
    CHECK_NEAR(r.value, 9.0, 1e-9);
    CHECK_TRUE(r.status == Status::Ok);

    CHECK_NEAR(calculate(6.0, 3.0, '-').value, 3.0, 1e-9);
    CHECK_NEAR(calculate(6.0, 3.0, '*').value, 18.0, 1e-9);
    CHECK_NEAR(calculate(6.0, 3.0, '/').value, 2.0, 1e-9);
    CHECK_NEAR(calculate(-2.5, 4.0, '*').value, -10.0, 1e-9);

    CHECK_TRUE(calculate(1.0, 0.0, '/').status == Status::DivideByZero);
    CHECK_NEAR(calculate(1.0, 0.0, '/').value, 0.0, 1e-9);
    CHECK_TRUE(calculate(1.0, 2.0, '%').status == Status::UnknownOperator);
    CHECK_TRUE(calculate(1.0, 2.0, 'x').status == Status::UnknownOperator);

    // 0.0 / 5.0 is fine -- only a zero *divisor* is an error.
    CHECK_TRUE(calculate(0.0, 5.0, '/').status == Status::Ok);

    CHECK_EQ(std::string(status_name(Status::Ok)), std::string("ok"));
    CHECK_EQ(std::string(status_name(Status::DivideByZero)), std::string("divide by zero"));
    CHECK_EQ(std::string(status_name(Status::UnknownOperator)), std::string("unknown operator"));

    return check::summary();
}
