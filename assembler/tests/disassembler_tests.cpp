#include "assembler/source/assembler.hpp"

#include <gtest/gtest.h>

struct DisassemblerTests : public testing::Test {
    Assembler assembler;
};

TEST_F(DisassemblerTests, disasm) {
    std::vector<std::string> ref = {
        "FIM P0, $A2",
        "",
        "LD R0",
        "ADD R1",
        "XCH R1",
        "JUN $05",
        ""
    };

    std::vector<uint8_t> bytecode = { 0x20, 0xA2, 0xA0, 0x81, 0xB1, 0x40, 0x05 };
    std::vector<std::string> output;

    assembler.disassemble(bytecode, output);
    EXPECT_EQ(bytecode.size(), output.size());
    EXPECT_EQ(output.size(), ref.size());
    ASSERT_LE(output.size(), ref.size());

    for (size_t i = 0; i < output.size(); ++i)
        EXPECT_EQ(output[i], ref[i]);
}
