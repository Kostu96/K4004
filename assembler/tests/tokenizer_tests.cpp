#include "assembler/source/tokenizer.hpp"

#include <fstream>
#include <gtest/gtest.h>

TEST(TokenizerTests, givenEmptyStramWhenGetNextThenInvalidTokenIsReturned) {
    std::stringstream ss;
    ss.str("");

    Tokenizer::Token token;
    token = Tokenizer::getNext(ss);

    EXPECT_EQ(token.type, Tokenizer::TokenType::Invalid);
}

TEST(TokenizerTests, givenOneLineCommentWhenGetNextThenInvalidTokenIsReturned) {
    std::stringstream ss;
    ss.str("; This is a comment.");

    Tokenizer::Token token;
    token = Tokenizer::getNext(ss);

    EXPECT_EQ(token.type, Tokenizer::TokenType::Invalid);
}

TEST(TokenizerTests, givenNewLineWhenGetNextThenNewLineTokenIsReturned) {
    std::stringstream ss;
    ss.str("\n");

    Tokenizer::Token token;
    token = Tokenizer::getNext(ss);

    EXPECT_EQ(token.str, "\n");
    EXPECT_EQ(token.type, Tokenizer::TokenType::NewLine);
}

TEST(TokenizerTests, givenProgramSourceWhenTokenizingThenProperTokensAreReturned) {
    std::ifstream file("programs/4bit_addition.asm");
    std::vector<Tokenizer::Token> refTokens({
        Tokenizer::Token("FIM", Tokenizer::TokenType::Text),
        Tokenizer::Token("P0", Tokenizer::TokenType::Text),
        Tokenizer::Token(",", Tokenizer::TokenType::Separator),
        Tokenizer::Token(0x42u, Tokenizer::TokenType::Number),
        Tokenizer::Token("\n", Tokenizer::TokenType::NewLine),
        Tokenizer::Token("LD", Tokenizer::TokenType::Text),
        Tokenizer::Token("R0", Tokenizer::TokenType::Text),
        Tokenizer::Token("\n", Tokenizer::TokenType::NewLine),
        Tokenizer::Token("ADD", Tokenizer::TokenType::Text),
        Tokenizer::Token("R1", Tokenizer::TokenType::Text),
        Tokenizer::Token("\n", Tokenizer::TokenType::NewLine),
        Tokenizer::Token("XCH", Tokenizer::TokenType::Text),
        Tokenizer::Token("R1", Tokenizer::TokenType::Text),
        Tokenizer::Token("\n", Tokenizer::TokenType::NewLine),
        Tokenizer::Token("\n", Tokenizer::TokenType::NewLine),
        Tokenizer::Token("DONE", Tokenizer::TokenType::Text),
        Tokenizer::Token("\n", Tokenizer::TokenType::NewLine),
        Tokenizer::Token("JUN", Tokenizer::TokenType::Text),
        Tokenizer::Token("DONE", Tokenizer::TokenType::Text),
        Tokenizer::Token("\n", Tokenizer::TokenType::NewLine),
    });

    Tokenizer::Token token;
    size_t tokenCount = 0;
    do {
        token = Tokenizer::getNext(file);

        ASSERT_LT(tokenCount, refTokens.size());
        EXPECT_EQ(token.str, refTokens[tokenCount].str);
        EXPECT_EQ(token.value, refTokens[tokenCount].value);
        EXPECT_EQ(token.type, refTokens[tokenCount].type);

        if (token.type != Tokenizer::TokenType::Invalid)
            ++tokenCount;
    } while (token.type != Tokenizer::TokenType::Invalid);

    EXPECT_EQ(tokenCount, refTokens.size());
}
