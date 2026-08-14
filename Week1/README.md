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

---

## Open gaps — what Week 1 did *not* close

Week 1 is green: 61 checks in exercises 1–4, 13 in the menu. That means the code
is correct on the inputs the tests chose. It does not mean the topics are closed.

Below is the honest list, found during the Week 1 review. Some of these are
things the tests never checked; some are topics the exercises deliberately
didn't reach. Read this before starting Week 2, and again whenever a later
exercise touches one of these.

### Data types

**Integer division truncates toward zero — rounding is never automatic.**
`01_temperature.cpp:45` is specified as *"round to the nearest tenth (not
truncate)"*. The implementation truncates. Every test still passes, because no
test case has a fractional part that rounds differently:

| `celsius_deci` | exact | your result | correctly rounded |
|---|---|---|---|
| 1 | 321.80 | 321 | **322** |
| -1 | 318.20 | 319 | **318** |
| 254 | 777.20 | 777 | 777 (agrees by luck) |

Note the negatives go wrong in the *opposite* direction: truncation pulls toward
zero, not toward negative infinity. Rounding a negative integer quotient is a
separate case you have to write on purpose.

**Integer promotion decides the arithmetic type, not the variable you assign to.**
In `int32_t f = (c * 9 / 5) + 320;` the `int32_t` describes the *destination*.
The multiply happens in `int`, because `int16_t` promotes to `int` before any
arithmetic. On this machine `int` is 32 bits, so it works. On an MCU with a
16-bit `int`, `c * 9` overflows for `c > 3640`, and signed overflow is undefined
behavior. You have not yet had to write the cast that makes the intent explicit:
`static_cast<int32_t>(c) * 9`.

**Untouched entirely:** unsigned types and wraparound, mixed signed/unsigned
comparison, and `<cstdint>` beyond `int16_t`/`int32_t`. `-Wconversion` never
fired this week, so it hasn't taught you anything yet.

### Errors

You have exactly one error mechanism so far:

| Mechanism | Status |
|---|---|
| Return a `{value, status}` struct | **Done** — `02_calculator.cpp` |
| Making the caller *check* the status | Not yet — nothing stops you ignoring `.status` |
| Sticky error flags you must clear | Not taught — see `05_menu.cpp:143` |
| Exceptions | Out of scope, permanently, for firmware |

The gap that matters most is the second row. `calculate()` returns a status, but
a caller can drop it on the floor silently and the compiler says nothing. The
fix is `[[nodiscard]]`, which is Week 2 material.

The third row is the `std::cin` failbit bug in the menu's I/O layer: once a
stream sets its error flag, every later read fails until something calls
`.clear()`. A UART status register with a sticky overrun bit behaves the same
way. That's Week 4.

One subtlety worth keeping: `02_calculator.cpp:54` compares a float with `==`,
which this README tells you never to do. It's correct *there* — you're testing
for an exact zero divisor, and `-0.0 == 0.0` is true, so both zeroes are caught.
What it does *not* catch is a divisor small enough to overflow the quotient. The
rule is really "never use `==` to ask whether two computed floats are equal";
testing against an exact constant is a different question.

### Switch and enums

Solid, with one skipped rep: `02_calculator.cpp:37` asked for a `switch` with a
`default:`, and it's written as an `if`/`else if` chain instead. It's correct,
but it left `line 62` unreachable, and it means exercise 2 didn't actually drill
the construct it was there to drill.

The enum rule itself did land — no `default:` in `status_name()` or
`option_label()`, so `-Wswitch` audits both. Verified by adding a new enumerator
and watching the build fail.

### Loose ends in the code

- Leftover `(void)param;` scaffolding lines in `01_temperature.cpp` and
  `02_calculator.cpp`. Harmless, but they were there to silence warnings about
  parameters you hadn't used yet — now that the functions are written, they're
  dead lines that suggest the work isn't finished.
- `05_menu.cpp:68` — missing blank line between two functions.

### Not started at all

From the roadmap, so you know what's genuinely ahead rather than missed:
classes and RAII, `const`-correctness, the memory model, stack vs. static,
`volatile`, interrupts, and registers.
