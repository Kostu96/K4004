# K4004
K4004 originaly started out as Intel4004 assembler, disassembler and emulator project.
It evolved into project that focuses on capabilities of the two Intel 4-bit processors (4004 and 4040) and tries to expand on them.

Emulator tries to "crank everything up to eleven", meaning amount of ROM and RAM, number of I/O lines and available peripherals.

The ultimate goal is to make something fun (like pong maybe?) running on the system.

# Building
Standart Cmake build.

Flag `BUILD_TESTS` is `ON` by default and requires gtest (cloned submodules).
