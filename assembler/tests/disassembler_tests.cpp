#include "assembler/source/assembler.hpp"

#include <gtest/gtest.h>

struct DisassemblerTestParam {
    std::vector<std::uint8_t> byteCode;
    std::vector<std::string> refDisassembly;

    DisassemblerTestParam(std::initializer_list<std::uint8_t> inByteCode, std::initializer_list<std::string> inRefDisassembly) :
        byteCode(inByteCode),
        refDisassembly(inRefDisassembly) {}
};

struct DisassemblerTests : public testing::TestWithParam<DisassemblerTestParam> {
    Assembler assembler;
};

TEST_P(DisassemblerTests, givenProgramByteCodeWhenDisassemblingThenCorrectDisassemblyIsReturned) {
    auto& testParam = GetParam();
    
    std::vector<std::string> output;
    assembler.disassemble(testParam.byteCode, output);
    
    EXPECT_EQ(testParam.byteCode.size(), output.size());
    EXPECT_EQ(output.size(), testParam.refDisassembly.size());
    ASSERT_LE(output.size(), testParam.refDisassembly.size());

    for (size_t i = 0; i < output.size(); ++i)
        EXPECT_EQ(output[i], testParam.refDisassembly[i]);
}

INSTANTIATE_TEST_SUITE_P(Parametrized, DisassemblerTests,
    testing::Values(
        DisassemblerTestParam(
            { 0x20, 0xA2, 0xA0, 0x81, 0xB1, 0x40, 0x05 },
            {
                "FIM P0, $A2",
                "",
                "LD R0",
                "ADD R1",
                "XCH R1",
                "JUN $05",
                ""
            }
        )
    )
);
