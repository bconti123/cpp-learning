# Week 1 — Core C++ Syntax

Five exercises, already scaffolded with **failing tests**. Your job is to fill in
each `// TODO`. When `make test` is green, the week is done.

```bash
cd Week1
make test              # build + run exercises 1-4
make run-05_menu       # exercise 5 is interactive
make clean
```

Work them in order — each one leans on the last.

| File | Topics | Checks |
|---|---|---|
| `01_temperature.cpp` | functions, data types, float vs. integer math | 16 |
| `02_calculator.cpp`  | `switch`, `enum class`, `struct`, error handling | 14 |
| `03_reverse_string.cpp` | pointers, arrays, references, `const` | 15 |
| `04_min_max.cpp` | arrays, structs, out-parameters, overflow | 16 |
| `05_menu.cpp` | loops, enums, separating logic from I/O | 13 |

---

## How to work an exercise

1. Read the whole file first, including the tests at the bottom. The tests are
   the specification — more precise than any prose I could write.
2. Implement one function. Run `make test`. Read the failure.
3. Repeat. Don't write all four functions then debug all four at once.

If you're stuck for more than ~15 minutes on one function, that's the signal to
ask me — not to go read a solution. The stuck part is the interesting part.

---

## Notes on the Week 1 topics

Short version of things that matter for embedded specifically. Skim now, come
back when a test surprises you.

### Data types
On a desktop, `int` is what you reach for. On an MCU you say exactly what you
mean, using `<cstdint>`: `uint8_t`, `int16_t`, `uint32_t`. `int` is only
guaranteed 16 bits by the standard, and a register field is a precise width —
guessing gets you silent truncation. That's what `-Wconversion` in the Makefile
is watching for.

`sizeof` and overflow are not academic here. See the `average()` test in
exercise 4: accumulating 1000 ADC samples into a 16-bit variable overflows, and
signed overflow is *undefined behavior* — the compiler is allowed to assume it
never happens and optimize accordingly.

### Floating point
Assume you don't have it. Many MCUs (Cortex-M0/M0+, most 8-bit parts) have no
FPU, so every `double` operation becomes a software routine costing hundreds of
cycles — fatal inside an interrupt handler. The fix is fixed-point integers:
store tenths or hundredths in an integer. That's Part B of exercise 1.

Also: never compare floats with `==`. `check.hpp` uses a tolerance for exactly
this reason.

### References vs. pointers
- A reference (`int&`) can't be null and can't be reseated. Prefer it when the
  thing must exist.
- A pointer (`int*`) can be null and can be moved. You need it for buffers,
  optional outputs, and any C API.
- `const int&` on a parameter means "I'll read this and won't copy it."

Pass a raw buffer as `(pointer, length)` and never let the length get separated
from the pointer. Losing track of a length is the #1 source of buffer overruns
in C and C++.

### Structs and enums
Use `enum class`, not plain `enum`. Plain enums leak their names into the
surrounding scope and implicitly convert to `int`, so you can accidentally do
arithmetic on a status code. `enum class` blocks both.

`switch` over an `enum class` with no `default:` is a feature: add a new enum
value and the compiler warns you about every switch that forgot to handle it.
That's a free audit of your codebase. (`status_name()` in exercise 2 uses this.)

### Error handling without exceptions
Firmware usually builds with `-fno-exceptions` — exceptions cost flash and make
timing unpredictable. So functions report failure by returning a struct holding
both a value and a status. Exercise 2 is that pattern. You'll see it in every
vendor HAL you touch.

---

## Things NOT to do this week

Deliberately out of scope. Resist the urge:

- `new` / `delete` / smart pointers — most firmware never dynamically allocates
- classes, inheritance, templates — that's later
- `std::vector`, `std::map` — heap-backed
- exceptions

You are building the C-shaped core of C++ first. That is the right order for
embedded.

---

## When Week 1 is green

Ask me for a **code review** of your solutions before moving on. Passing tests
means correct; it doesn't yet mean good. Reviewing your own working code is
where most of the learning actually lands.
