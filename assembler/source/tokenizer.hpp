#pragma once
#include <iostream>
#include <functional>

class Tokenizer {
public:
    enum class TokenType {
        Invalid = -1,
        Text,
        Number,
        Separator,
        Symbol,
        NewLine
    };

    struct Token {
        std::string str = "";
        unsigned int value = 0u;
        TokenType type = TokenType::Invalid;

        Token() = default;
        Token(const char* str, TokenType type);
        Token(unsigned int value, TokenType type);
        Token(const Token& other) = default;
        Token(Token&& other) noexcept;
        Token& operator=(Token&& other) noexcept;
        Token& operator=(const Token& other) = default;
    };

    static Token getNext(std::istream& stream);

    Tokenizer() = delete;
private:
    enum class State : unsigned int {
        Entry,
        EatComment,
        EatCommentNL,
        BinNumber,
        HexNumber,

        Finish,
        Invalid
    };

    static State entryState(Token& token, char ch);
    static State eatCommentState(Token& token, char ch);
    static State eatCommentNLState(Token& token, char ch);
    static State BinNumberState(Token& token, char ch);
    static State HexNumberState(Token& token, char ch);

    typedef State(*StateFunctionPtr)(Token&, char);
    static constexpr StateFunctionPtr m_stateFunctions[static_cast<unsigned int>(State::Finish)] = {
        entryState,
        eatCommentState,
        eatCommentNLState,
        BinNumberState,
        HexNumberState,
    };

    static State m_currentState;
};
