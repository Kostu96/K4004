#include "assembler/source/assembler.hpp"

#include <gtest/gtest.h>

struct DisassemblerTests : public testing::Test {
    Assembler assembler;
};

TEST_F(DisassemblerTests, disasm) {
    std::string ref =
        "FIM P0, $A2\n"
        "LD R0\n"
        "ADD R1\n"
        "XCH R1\n"
        "JUN $05\n";

    std::vector<uint8_t> bytecode = { 0x20, 0xA2, 0xA0, 0x81, 0xB1, 0x40, 0x05 };
    std::string output;

    assembler.disassemble(bytecode, output);

    EXPECT_EQ(output, ref);
    ASSERT_TRUE(true);
}
