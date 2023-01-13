Project from this repository is developed further only as part of:
https://github.com/Kostu96/emulators

# K4004
K4004 originaly started out as Intel 4004 assembler, disassembler and emulator project.
It evolved into project that focuses on capabilities of the two Intel 4-bit processors (4004 and 4040) and tries to expand on them.

Emulator tries to "crank everything up to eleven", meaning amount of ROM and RAM, number of I/O lines and available peripherals.

The ultimate goal is to make something fun (like simple BASIC interpreter maybe?) running on the system.

# Building
Standard Cmake build.

### Cmake options
`BUILD_TESTS` - `ON` by default, requires gtest (cloned submodules).
`STATIC_ANALYSIS` - `OFF` by default, requires clang-tidy. Builds source with clang-tidy checks.
