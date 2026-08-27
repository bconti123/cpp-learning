# cpp-learning

Learning modern C++, aimed at embedded software.

## Layout

```
Week1/          Core C++ syntax — exercises with failing tests to fill in
Week2/          Types, references, pointers — one lesson at a time
helloworld.cpp  first program
```

Each week is a self-contained directory with its own `Makefile` and `README.md`.

## Getting started

```bash
cd Week1
make test       # build and run the exercises
cat README.md   # what to do and why
```

## Toolchain

`g++ 11.4` (Ubuntu / WSL2), C++17, no external dependencies.

C++17 rather than C++20 on purpose: it's what embedded toolchains (GCC ARM, IAR,
TI) reliably support. Every exercise builds with `-Wall -Wextra -Wpedantic
-Wconversion -Wshadow -Werror` — warnings are errors, because a codebase kept at
zero warnings stays that way and one at 400 never recovers.

## Roadmap

- [x] **Week 1 — Core syntax.** Variables, types, functions, `if`/`switch`,
      loops, arrays, references, pointers, structs, enums.
- [ ] **Week 2 — Types, references, pointers.** Data types in depth, signed vs.
      unsigned, fixed-width types, conversion, `&`, `*`, `nullptr`, parameter
      passing, basic `const`.
- [ ] Week 3 — Classes, RAII, `const`-correctness.
- [ ] Week 4 — Memory model, stack vs. static, why firmware avoids the heap.
- [ ] Week 5 — Embedded-specific: registers, volatile, interrupts, fixed-point.
