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
        EXPECT_EQ(output[i], testParam.refDisassembly[i]) << "At byte " << i;
}

INSTANTIATE_TEST_SUITE_P(Parametrized, DisassemblerTests,
    testing::Values(
        DisassemblerTestParam({
            0x20, 0xA2, 0xA0, 0x81, 0xB1, 0x40, 0x05
        },
        {
            "FIM P0, $A2",
            "",
            "LD R0",
            "ADD R1",
            "XCH R1",
            "JUN $005",
            ""
        }),
        DisassemblerTestParam({
            0x28, 0x00, 0x29, 0xEA, 0xB0, 0x68, 0x29, 0xEA,
            0xB1, 0x50, 0x18, 0xB2, 0xE1, 0x40, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xF0, 0xB2, 0xD4, 0xB0, 0xF6, 0xB0, 0x1A, 0x2A,
            0xB1, 0xF6, 0xB1, 0xB2, 0xF6, 0xB2, 0xF8, 0x1C,
            0x1B, 0xC0, 0xB1, 0xF6, 0xB1, 0xF1, 0x40, 0x23
        },
        {
            "FIM P4, $00",
            "",
            "SRC P4",
            "RDR",
            "XCH R0",
            "INC R8",
            "SRC P4",
            "RDR",
            "XCH R1",
            "JMS $018",
            "",
            "XCH R2",
            "WMP",
            "JUN $000",
            "",
            "NOP", "NOP", "NOP", "NOP", "NOP", "NOP", "NOP", "NOP", "NOP",
            "CLB",
            "XCH R2",
            "LDM $4",
            "XCH R0",
            "RAR",
            "XCH R0",
            "JCN %1010, $2A",
            "",
            "XCH R1",
            "RAR",
            "XCH R1",
            "XCH R2",
            "RAR",
            "XCH R2",
            "DAC",
            "JCN %1100, $1B",
            "",
            "BBL $0",
            "XCH R1",
            "RAR",
            "XCH R1",
            "CLC",
            "JUN $023",
            ""
        }),
        DisassemblerTestParam({
            0x20, 0x00, 0x22, 0x00, 0xDC, 0xB2, 0x21, 0xE0,
            0xF2, 0x71, 0x06, 0x60, 0x72, 0x06, 0x20, 0x00,
            0x22, 0x00, 0xDC, 0xB2, 0x21, 0xE4, 0xF2, 0xE5,
            0xF2, 0xE6, 0xF2, 0xE7, 0xF2, 0x60, 0x72, 0x14,
            0x40, 0x20
        },
        {
            "FIM P0, $00",
            "",
            "FIM P1, $00",
            "",
            "LDM $C",
            "XCH R2",
            "SRC P0",
            "WRM",
            "IAC",
            "ISZ R1, $06",
            "",
            "INC R0",
            "ISZ R2, $06",
            "",
            "FIM P0, $00",
            "",
            "FIM P1, $00",
            "",
            "LDM $C",
            "XCH R2",
            "SRC P0",
            "WR0",
            "IAC",
            "WR1",
            "IAC",
            "WR2",
            "IAC",
            "WR3",
            "IAC",
            "INC R0",
            "ISZ R2, $14",
            "",
            "JUN $020",
            ""
        })
    )
);
