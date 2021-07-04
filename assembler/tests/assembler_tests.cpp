#include "assembler/source/assembler.hpp"

#include <gtest/gtest.h>

struct AssemblerTestParam {
    const char* sourceFilename;
    std::vector<std::uint8_t> refByteCode;

    AssemblerTestParam(const char* pSourceFilename, std::initializer_list<std::uint8_t> pRefByteCode) :
        sourceFilename(pSourceFilename),
        refByteCode(pRefByteCode) {}
};

struct AssemblerTests : public testing::TestWithParam<AssemblerTestParam> {};

TEST_P(AssemblerTests, given4bitAdditionAssemblerCodeWhenAssemblingThenCorrectByteCodeIsReturned) {
    auto& testParam = GetParam();
    
    std::uint8_t* byteCode;
    std::size_t byteCodeSize;
    bool ret = Assembler::assemble(testParam.sourceFilename, byteCode, byteCodeSize);
    ASSERT_TRUE(ret);

    EXPECT_EQ(byteCodeSize, testParam.refByteCode.size());

    for (size_t i = 0u; i < byteCodeSize; ++i)
        EXPECT_EQ(byteCode[i], testParam.refByteCode[i]);

    Assembler::freeOutput(byteCode);
}

INSTANTIATE_TEST_SUITE_P(Parametrized, AssemblerTests,
    testing::Values(
        AssemblerTestParam("programs/4bit_addition.asm", {
            0x20, 0xA2, 0xA0, 0x81, 0xB1, 0x40, 0x05
        }),
        AssemblerTestParam("programs/basic_program.asm", {
            0x28, 0x00, 0x29, 0xEA, 0xB0, 0x68, 0x29, 0xEA,
            0xB1, 0x50, 0x20, 0xB2, 0xE1, 0x40, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xF0, 0xB2, 0xD4, 0xB0, 0xF6, 0xB0, 0x1A, 0x32,
            0xB1, 0xF6, 0xB1, 0xB2, 0xF6, 0xB2, 0xF8, 0x1C,
            0x23, 0xC0, 0xB1, 0xF6, 0xB1, 0xF1, 0x40, 0x2B
        }),
        AssemblerTestParam("programs/ram_test.asm", {
            0x20, 0x00, 0x24, 0x00, 0xDC, 0xB2, 0x21, 0xE0,
            0xF2, 0x71, 0x06, 0x60, 0x72, 0x06, 0x20, 0x00,
            0x24, 0x00, 0xDC, 0xB2, 0x21, 0xE4, 0xF2, 0xE5,
            0xF2, 0xE6, 0xF2, 0xE7, 0xF2, 0x60, 0x72, 0x14,
            0x40, 0x20
        })
    )
);
