#include "assembler/source/tokenizer.hpp"

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

TEST(TokenizerTests, givenNewLinesWhenGetNextThenInvalidTokenIsReturned) {
    std::stringstream ss;
    ss.str("\n\n");

    Tokenizer::Token token;
    token = Tokenizer::getNext(ss);

    EXPECT_EQ(token.type, Tokenizer::TokenType::Invalid);
}

// TODO: give proper test name
TEST(TokenizerTests, fooTestName) {
    std::stringstream ss;
    ss.str("    FIM P0, $42");

    Tokenizer::Token token;
    token = Tokenizer::getNext(ss);

    EXPECT_EQ(token.str, "FIM");
    EXPECT_EQ(token.type, Tokenizer::TokenType::Text);

    token = Tokenizer::getNext(ss);

    EXPECT_EQ(token.str, "P0");
    EXPECT_EQ(token.type, Tokenizer::TokenType::Text);

    token = Tokenizer::getNext(ss);

    EXPECT_EQ(token.str, ",");
    EXPECT_EQ(token.type, Tokenizer::TokenType::Separator);
}
