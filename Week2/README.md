# Week 2 — Types, References, and Pointers

Week 1 gave you the shape of a C++ program. Week 2 slows down and goes after the
two things that are still fuzzy: **what a type really is**, and **what `&` and
`*` really mean**.

This week is deliberately slower than Week 1. One concept per lesson, one short
example, one small exercise. Nothing here is a race.

```bash
cd Week2
make run-01_sizes      # run one lesson's exercise
make test              # run all of them
make clean
```

## Lesson plan

Lessons unlock one at a time. Each file appears when we get to it.

| # | File | Concept |
|---|---|---|
| 1 | `01_sizes.cpp` | What a type is: size in bytes + how the bits are read |
| 2 | `02_types.cpp` | `int`, `double`, `char`, `bool`, `std::string` — what each is *for* |
| 3 | — | Signed vs. unsigned: the negative half, and wraparound |
| 4 | — | Fixed-width types: `int32_t`, `uint8_t`, and why embedded insists |
| 5 | — | Assignment and type conversion; when the compiler quietly loses data |
| 6 | — | Addresses: `&x` means "where does x live?" |
| 7 | — | Pointers: `int* p` and what `*p` does |
| 8 | — | `nullptr`: a pointer that deliberately points nowhere |
| 9 | — | References: `int& r`, a second name for the same box |
| 10 | — | Passing to functions: by value, by reference, by pointer |
| 11 | — | `const`: promising not to change something |

## The `&` and `*` confusion (read this now, believe it later)

Both symbols mean **two different things** depending on where they appear. This
trips up every single beginner, and it is not your fault — the language reuses
the punctuation.

| Symbol | In a **type** (declaration) | In an **expression** (code that runs) |
|---|---|---|
| `&` | `int& r = x;` — r is a *reference* | `&x` — the *address of* x |
| `*` | `int* p = &x;` — p is a *pointer* | `*p` — the *value at* p |

Rule of thumb: **left of the variable name = part of the type. In front of a
value = an operation.** We come back to this properly in lessons 6–9.

## Out of scope this week

Same discipline as Week 1. Not yet:

- `new` / `delete`, smart pointers, anything that allocates
- classes, inheritance, templates
- `std::vector`, `std::map`
- exceptions
