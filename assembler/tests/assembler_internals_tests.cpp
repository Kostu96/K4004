#include "assembler/source/assembler.hpp"

#include <gtest/gtest.h>

template<>
struct WhiteBox<Assembler> {
    static uint8_t callParseOperand(Assembler& assembler, const std::string& token) {
        return assembler.parseOperand(token);
    }
};

struct AssemblerInternalsTests : public testing::Test {
    Assembler assembler;
};

TEST_F(AssemblerInternalsTests, givenValidOperandWhenParseOperandThenCorrectValueIsReturned) {
    // Decimal Numbers
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "0"), 0u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "10"), 10u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "42"), 42u);
    
    // Hex Numbers
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "$0"), 0x0u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "$10"), 0x10u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "$2A"), 0x2Au);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "$fA"), 0xFAu);

    // Binary Numbers
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "%0"), 0b0u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "%10"), 0b10u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "%101001"), 0b101001u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "%0001110"), 0b0001110u);

    // Octal Numbers
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "0254"), 0254u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "0100"), 0100u);

    // Registers
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "R0"), 0u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "R1"), 1u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "R2"), 2u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "R4"), 4u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "R7"), 7u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "R9"), 9u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "RA"), 10u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "RB"), 11u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "RD"), 13u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "RE"), 14u);
    EXPECT_EQ(WhiteBox<Assembler>::callParseOperand(assembler, "RF"), 15u);

    // Register Pairs

}
