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
        Symbol
    };

    struct Token {
        std::string str = "";
        TokenType type = TokenType::Invalid;

        Token() = default;
        Token(Token&& other) noexcept;
        Token& operator=(Token&& other) noexcept;
    };

    static Token getNext(std::istream& stream);

    Tokenizer() = delete;
private:
    enum class State : unsigned int {
        Entry,
        EatComment,

        Finish,
        Invalid
    };

    static State entryState(Token& token, char ch);
    static State eatCommentState(Token& token, char ch);

    typedef State(*StateFunctionPtr)(Token&, char);
    static constexpr StateFunctionPtr m_stateFunctions[static_cast<unsigned int>(State::Finish)] = {
        entryState,
        eatCommentState
    };

    static State m_currentState;
};
