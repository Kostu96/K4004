#include "assembler/source/assembler.hpp"

#include <gtest/gtest.h>

struct AssemblerTestParam {
    const char* sourceFilename;
    std::vector<std::uint8_t> refByteCode;

    AssemblerTestParam(const char* pSourceFilename, std::initializer_list<std::uint8_t> pRefByteCode) :
        sourceFilename(pSourceFilename),
        refByteCode(pRefByteCode) {}
};

struct AssemblerTests : public testing::TestWithParam<AssemblerTestParam> {
    Assembler assembler;
};

TEST_P(AssemblerTests, givenProgramAssemblyCodeWhenAssemblingThenCorrectByteCodeIsReturned) {
    auto& testParam = GetParam();
    
    std::vector<uint8_t> byteCode;
    bool ret = assembler.assemble(testParam.sourceFilename, byteCode);
    ASSERT_TRUE(ret);

    EXPECT_EQ(byteCode.size(), testParam.refByteCode.size());
    ASSERT_LE(byteCode.size(), testParam.refByteCode.size());

    for (size_t i = 0u; i < byteCode.size(); ++i)
        EXPECT_EQ(byteCode[i], testParam.refByteCode[i]) << "  i == " << i;
}

INSTANTIATE_TEST_SUITE_P(Parametrized, AssemblerTests,
    testing::Values(
        AssemblerTestParam("programs/4bit_addition.asm", {
            0xFE, 0xFF,
            0x20, 0xA2, 0xA0, 0x81, 0xB1, 0x40, 0x05
        }),
        AssemblerTestParam("programs/4bit_and_subroutine.asm", {
            0xFE, 0xFF,
            0x28, 0x00, 0x29, 0xEA, 0xB0, 0x68, 0x29, 0xEA,
            0xB1, 0x50, 0x18, 0xB2, 0xE1, 0x40, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xF0, 0xB2, 0xD4, 0xB0, 0xF6, 0xB0, 0x1A, 0x2A,
            0xB1, 0xF6, 0xB1, 0xB2, 0xF6, 0xB2, 0xF8, 0x1C,
            0x1B, 0xC0, 0xB1, 0xF6, 0xB1, 0xF1, 0x40, 0x23
        }),
        AssemblerTestParam("programs/ram_test.asm", {
            0xFE, 0xFF,
            0x20, 0x00, 0x22, 0x00, 0xDC, 0xB2, 0x21, 0xE0,
            0xF2, 0x71, 0x06, 0x60, 0x72, 0x06, 0x20, 0x00,
            0x22, 0x00, 0xDC, 0xB2, 0x21, 0xE4, 0xF2, 0xE5,
            0xF2, 0xE6, 0xF2, 0xE7, 0xF2, 0x60, 0x72, 0x14,
            0x40, 0x20
        }),
        AssemblerTestParam("programs/mcs4_evaluation.asm", {
            0xFE, 0xFF,
            0xE2, 0xCF, 0x2A, 0x41, 0x50, 0xDE, 0x50, 0xE5,
            0x20, 0xFE, 0x50, 0xEE, 0x50, 0xE5, 0x50, 0xEE,
            0x50, 0xE5, 0x2A, 0x42, 0x5F, 0xFF, 0x50, 0x1A,
            0x40, 0x24, 0x5F, 0xFF, 0x50, 0x20, 0x40, 0x18,
            0x5F, 0xFF, 0x4F, 0xFF, 0x22, 0xCB, 0xF0, 0x2B,
            0xE1, 0x21, 0xE0, 0xF2, 0x71, 0x29, 0xE4, 0xF2,
            0xE5, 0xF2, 0xE6, 0xF2, 0xE7, 0x60, 0x72, 0x29,
            0xFA, 0x50, 0xF7, 0x73, 0x39, 0x25, 0xFA, 0xF5,
            0xE1, 0x1A, 0x47, 0x1C, 0x4F, 0x19, 0x50, 0x12,
            0x50, 0x14, 0x52, 0x11, 0x43, 0x40, 0x45, 0xF0,
            0x40, 0x3F, 0x2C, 0x66, 0x2E, 0x59, 0x20, 0x00,
            0x3D, 0x21, 0x84, 0x85, 0xE0, 0xF6, 0x74, 0x59,
            0x75, 0x59, 0x50, 0xDE, 0x40, 0x75, 0x50, 0xDE,
            0x21, 0x94, 0x95, 0xE0, 0xF0, 0x74, 0x68, 0x75,
            0x68, 0xF0, 0x2B, 0xE1, 0x3F, 0xFA, 0x68, 0xA8,
            0xE0, 0xB9, 0xA9, 0xE2, 0xFB, 0xE0, 0x74, 0x75,
            0xF0, 0xF8, 0xE0, 0xFC, 0xE0, 0x74, 0x81, 0xF0,
            0xFB, 0xE0, 0xF2, 0x74, 0x88, 0xDF, 0xE0, 0xF7,
            0xE0, 0x1C, 0x8D, 0xF0, 0x2B, 0xE1, 0xDF, 0xF9,
            0xE0, 0xFA, 0xF9, 0xE0, 0xF3, 0xF6, 0xE0, 0x74,
            0x9C, 0x24, 0xC0, 0x21, 0xE9, 0x71, 0xA3, 0xEC,
            0xED, 0xEE, 0xEF, 0x60, 0x74, 0xA3, 0x20, 0x20,
            0x22, 0x30, 0x21, 0xE8, 0x61, 0x23, 0xE8, 0xE0,
            0x73, 0xB2, 0x20, 0x00, 0x20, 0x10, 0xF0, 0x2B,
            0xE1, 0x21, 0xEB, 0x61, 0x23, 0xEB, 0xE0, 0x73,
            0xC1, 0x2B, 0xEC, 0x14, 0xD7, 0xD8, 0x21, 0xE1,
            0xF0, 0x2B, 0xE4, 0x19, 0xD3, 0x40, 0x02, 0xF2,
            0xE4, 0xD2, 0x21, 0xE1, 0x40, 0x02, 0x2B, 0xAB,
            0xF1, 0xE1, 0xF5, 0xBB, 0xC0, 0x21, 0x23, 0x25,
            0x27, 0x29, 0x2B, 0x2D, 0x2F, 0xC0, 0x32, 0x34,
            0x36, 0x38, 0x3A, 0x3C, 0x3E, 0x30, 0xC0, 0xA4,
            0xF5, 0xFD, 0xB4, 0xEA, 0xC0, 0x00, 0xFF, 0x00
        })
    )
);
