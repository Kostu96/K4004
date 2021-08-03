#include "assembler/source/tokenizer.hpp"

#include "shared/source/assembly.hpp"

#include <fstream>
#include <gtest/gtest.h>

void checkTokens(const Tokenizer::Token& lhs, const Tokenizer::Token& rhs, size_t tokenIdx) {
    EXPECT_EQ(lhs.line, rhs.line) << "  i == " << tokenIdx;
    EXPECT_EQ(lhs.text, rhs.text) << "  i == " << tokenIdx;
    EXPECT_EQ(lhs.type, rhs.type) << "  i == " << tokenIdx;
    EXPECT_EQ(lhs.value, rhs.value) << "  i == " << tokenIdx;
}

struct TokenizerTestParam {
    const char* sourceFilename;
    std::vector<Tokenizer::Token> refTokens;

    TokenizerTestParam(const char* inSourceFilename, std::initializer_list<Tokenizer::Token> inRefTokens) :
        sourceFilename(inSourceFilename),
        refTokens(inRefTokens) {}
};

struct TokenizerTests : public testing::TestWithParam<TokenizerTestParam> {};

TEST_P(TokenizerTests, givenProgramSourceWhenTokenizingThenProperTokensAreReturned) {
    auto& testParam = GetParam();

    std::ifstream file(testParam.sourceFilename);
    Tokenizer tokenizer(file);
    Tokenizer::Token token;
    size_t tokenCount = 0;
    do {
        token = tokenizer.getNext();
        
        if (tokenCount < testParam.refTokens.size())
            checkTokens(token, testParam.refTokens[tokenCount], tokenCount);

        ++tokenCount;
    } while (token.type != Tokenizer::TokenType::Invalid);

    EXPECT_EQ(tokenCount, testParam.refTokens.size());
}

using Token = Tokenizer::Token;
using TokenType = Tokenizer::TokenType;

INSTANTIATE_TEST_SUITE_P(Parametrized, TokenizerTests,
    testing::Values(
        TokenizerTestParam("programs/4bit_addition.asm", {
            Token(TokenType::NewLine, "\n", 1u),
            Token(TokenType::NewLine, "\n", 2u),
            Token(TokenType::Mnemonic, "FIM", 3u, +AsmIns::FIM),
            Token(TokenType::RegisterPair, "P0", 3u, +AsmReg::P0),
            Token(TokenType::Comma, ",", 3u),
            Token(TokenType::Number, "$A2", 3u, 0xA2u),
            Token(TokenType::NewLine, "\n", 3u),
            Token(TokenType::Mnemonic, "LD", 4u, +AsmIns::LD),
            Token(TokenType::Register, "R0", 4u, +AsmReg::R0),
            Token(TokenType::NewLine, "\n", 4u),
            Token(TokenType::Mnemonic, "ADD", 5u, +AsmIns::ADD),
            Token(TokenType::Register, "R1", 5u, +AsmReg::R1),
            Token(TokenType::NewLine, "\n", 5u),
            Token(TokenType::Mnemonic, "XCH", 6u, +AsmIns::XCH),
            Token(TokenType::Register, "R1", 6u, +AsmReg::R1),
            Token(TokenType::NewLine, "\n", 6u),
            Token(TokenType::NewLine, "\n", 7u),
            Token(TokenType::Label, "DONE", 8u),
            Token(TokenType::NewLine, "\n", 8u),
            Token(TokenType::Mnemonic, "JUN", 9u, +AsmIns::JUN),
            Token(TokenType::Label, "DONE", 9u),
            Token(TokenType::NewLine, "\n", 9u),
            Token()
        }),
        TokenizerTestParam("programs/4bit_and_subroutine.asm", {
            Token(TokenType::NewLine, "\n", 1u),
            Token(TokenType::NewLine, "\n", 2u),
            Token(TokenType::Label, "START", 3u),
            Token(TokenType::Mnemonic, "FIM", 3u, +AsmIns::FIM),
            Token(TokenType::RegisterPair, "P4", 3u, +AsmReg::P4),
            Token(TokenType::Comma, ",", 3u),
            Token(TokenType::Number, "0", 3u, 0u),
            Token(TokenType::NewLine, "\n", 3u),
            Token(TokenType::Mnemonic, "SRC", 4u, +AsmIns::SRC),
            Token(TokenType::RegisterPair, "P4", 4u, +AsmReg::P4),
            Token(TokenType::NewLine, "\n", 4u),
            Token(TokenType::Mnemonic, "RDR", 5u, +AsmIns::RDR),
            Token(TokenType::NewLine, "\n", 5u),
            Token(TokenType::Mnemonic, "XCH", 6u, +AsmIns::XCH),
            Token(TokenType::Register, "R0", 6u, +AsmReg::R0),
            Token(TokenType::NewLine, "\n", 6u),
            Token(TokenType::Mnemonic, "INC", 7u, +AsmIns::INC),
            Token(TokenType::Register, "R8", 7u, +AsmReg::R8),
            Token(TokenType::NewLine, "\n", 7u),
            Token(TokenType::Mnemonic, "SRC", 8u, +AsmIns::SRC),
            Token(TokenType::RegisterPair, "P4", 8u, +AsmReg::P4),
            Token(TokenType::NewLine, "\n", 8u),
            Token(TokenType::Mnemonic, "RDR", 9u, +AsmIns::RDR),
            Token(TokenType::NewLine, "\n", 9u),
            Token(TokenType::Mnemonic, "XCH", 10u, +AsmIns::XCH),
            Token(TokenType::Register, "R1", 10u, +AsmReg::R1),
            Token(TokenType::NewLine, "\n", 10u),
            Token(TokenType::Mnemonic, "JMS", 11u, +AsmIns::JMS),
            Token(TokenType::Label, "AND", 11u),
            Token(TokenType::NewLine, "\n", 11u),
            Token(TokenType::Mnemonic, "XCH", 12u, +AsmIns::XCH),
            Token(TokenType::Register, "R2", 12u, +AsmReg::R2),
            Token(TokenType::NewLine, "\n", 12u),
            Token(TokenType::Mnemonic, "WMP", 13u, +AsmIns::WMP),
            Token(TokenType::NewLine, "\n", 13u),
            Token(TokenType::Mnemonic, "JUN", 14u, +AsmIns::JUN),
            Token(TokenType::Label, "START", 14u),
            Token(TokenType::NewLine, "\n", 14u),
            Token(TokenType::Mnemonic, "NOP", 15u, +AsmIns::NOP),
            Token(TokenType::NewLine, "\n", 15u),
            Token(TokenType::NewLine, "\n", 16u),
            Token(TokenType::NewLine, "\n", 17u),
            Token(TokenType::Star, "*", 18u),
            Token(TokenType::Equal, "=", 18u),
            Token(TokenType::Number, "24", 18u, 24u),
            Token(TokenType::NewLine, "\n", 18u),
            Token(TokenType::Label, "AND", 19u),
            Token(TokenType::Mnemonic, "CLB", 19u, +AsmIns::CLB),
            Token(TokenType::NewLine, "\n", 19u),
            Token(TokenType::Mnemonic, "XCH", 20u, +AsmIns::XCH),
            Token(TokenType::Register, "R2", 20u, +AsmReg::R2),
            Token(TokenType::NewLine, "\n", 20u),
            Token(TokenType::Mnemonic, "LDM", 21u, +AsmIns::LDM),
            Token(TokenType::Number, "4", 21u, 4u),
            Token(TokenType::NewLine, "\n", 21u),
            Token(TokenType::Label, "AND_3", 22u),
            Token(TokenType::Mnemonic, "XCH", 22u, +AsmIns::XCH),
            Token(TokenType::Register, "R0", 22u, +AsmReg::R0),
            Token(TokenType::NewLine, "\n", 22u),
            Token(TokenType::Mnemonic, "RAR", 23u, +AsmIns::RAR),
            Token(TokenType::NewLine, "\n", 23u),
            Token(TokenType::Mnemonic, "XCH", 24u, +AsmIns::XCH),
            Token(TokenType::Register, "R0", 24u, +AsmReg::R0),
            Token(TokenType::NewLine, "\n", 24u),
            Token(TokenType::Mnemonic, "JCN", 25u, +AsmIns::JCN),
            Token(TokenType::Label, "CZ", 25u),
            Token(TokenType::Comma, ",", 25u),
            Token(TokenType::Label, "ROTR1", 25u),
            Token(TokenType::NewLine, "\n", 25u),
            Token(TokenType::Mnemonic, "XCH", 26u, +AsmIns::XCH),
            Token(TokenType::Register, "R1", 26u, +AsmReg::R1),
            Token(TokenType::NewLine, "\n", 26u),
            Token(TokenType::Mnemonic, "RAR", 27u, +AsmIns::RAR),
            Token(TokenType::NewLine, "\n", 27u),
            Token(TokenType::Mnemonic, "XCH", 28u, +AsmIns::XCH),
            Token(TokenType::Register, "R1", 28u, +AsmReg::R1),
            Token(TokenType::NewLine, "\n", 28u),
            Token(TokenType::Label, "ROTR2", 29u),
            Token(TokenType::Mnemonic, "XCH", 29u, +AsmIns::XCH),
            Token(TokenType::Register, "R2", 29u, +AsmReg::R2),
            Token(TokenType::NewLine, "\n", 29u),
            Token(TokenType::Mnemonic, "RAR", 30u, +AsmIns::RAR),
            Token(TokenType::NewLine, "\n", 30u),
            Token(TokenType::Mnemonic, "XCH", 31u, +AsmIns::XCH),
            Token(TokenType::Register, "R2", 31u, +AsmReg::R2),
            Token(TokenType::NewLine, "\n", 31u),
            Token(TokenType::Mnemonic, "DAC", 32u, +AsmIns::DAC),
            Token(TokenType::NewLine, "\n", 32u),
            Token(TokenType::Mnemonic, "JCN", 33u, +AsmIns::JCN),
            Token(TokenType::Label, "ANZ", 33u),
            Token(TokenType::Comma, ",", 33u),
            Token(TokenType::Label, "AND_3", 33u),
            Token(TokenType::NewLine, "\n", 33u),
            Token(TokenType::Mnemonic, "BBL", 34u, +AsmIns::BBL),
            Token(TokenType::Number, "0", 34u, 0u),
            Token(TokenType::NewLine, "\n", 34u),
            Token(TokenType::Label, "ROTR1", 35u),
            Token(TokenType::Mnemonic, "XCH", 35u, +AsmIns::XCH),
            Token(TokenType::Register, "R1", 35u, +AsmReg::R1),
            Token(TokenType::NewLine, "\n", 35u),
            Token(TokenType::Mnemonic, "RAR", 36u, +AsmIns::RAR),
            Token(TokenType::NewLine, "\n", 36u),
            Token(TokenType::Mnemonic, "XCH", 37u, +AsmIns::XCH),
            Token(TokenType::Register, "R1", 37u, +AsmReg::R1),
            Token(TokenType::NewLine, "\n", 37u),
            Token(TokenType::Mnemonic, "CLC", 38u, +AsmIns::CLC),
            Token(TokenType::NewLine, "\n", 38u),
            Token(TokenType::Mnemonic, "JUN", 39u, +AsmIns::JUN),
            Token(TokenType::Label, "ROTR2", 39u),
            Token(TokenType::NewLine, "\n", 39u),
            Token(TokenType::NewLine, "\n", 40u),
            Token(TokenType::Label, "CZ", 41u),
            Token(TokenType::Equal, "=", 41u),
            Token(TokenType::Number, "10", 41u, 10u),
            Token(TokenType::NewLine, "\n", 41u),
            Token(TokenType::Label, "ANZ", 42u),
            Token(TokenType::Equal, "=", 42u),
            Token(TokenType::Number, "12", 42u, 12u),
            Token(TokenType::NewLine, "\n", 42u),
            Token()
        })
    )
);
