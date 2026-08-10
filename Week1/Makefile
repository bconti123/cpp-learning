# Week 1 build.
#
# Usage:
#   make            build everything into build/
#   make test       build + run every non-interactive exercise
#   make run-05_menu   build + run one program by name
#   make clean      delete build/
#
# Why these flags matter (read this once, then forget it):
#   -std=c++17     What most embedded toolchains (GCC ARM, IAR, TI) reliably
#                  support today. C++20 is nicer but you can't always have it.
#   -Wall -Wextra  The bugs the compiler already knows about. Non-negotiable.
#   -Wpedantic     Rejects compiler-specific extensions, so your code ports.
#   -Wconversion   THE embedded flag. Warns when a value silently narrows,
#                  e.g. stuffing an int into a uint8_t register field.
#   -Wshadow       Catches an inner variable hiding an outer one.
#   -Werror        Warnings become errors. Harsh on purpose: it's much easier
#                  to keep a codebase at zero warnings than to clean up 400.
#   -g -O0         Debug symbols, no optimization, so a debugger tells the truth.

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Werror -g -O0

SRCS := $(wildcard *.cpp)
BINS := $(patsubst %.cpp,build/%,$(SRCS))

# 05_menu reads from stdin, so it can't run unattended in `make test`.
TEST_BINS := $(filter-out build/05_menu,$(BINS))

.PHONY: all test clean
all: $(BINS)

build:
	@mkdir -p build

build/%: %.cpp check.hpp | build
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: run-%
run-%: build/%
	@./$<

test: $(TEST_BINS)
	@fail=0; \
	for b in $(TEST_BINS); do \
		echo "=== $$b"; \
		./$$b || fail=1; \
	done; \
	if [ $$fail -ne 0 ]; then echo "SOME EXERCISES FAILING"; exit 1; fi; \
	echo "All exercises passing."

clean:
	rm -rf build
