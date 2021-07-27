#include "assembler/source/tokenizer.hpp"

#include <cassert>

Tokenizer::Token::Token(Token&& other) noexcept :
    str(std::move(other.str)),
    type(other.type)
{}

Tokenizer::Token& Tokenizer::Token::operator=(Token && other) noexcept
{
    str = std::move(other.str);
    type = other.type;

    return *this;
}

Tokenizer::Token Tokenizer::getNext(std::istream& stream)
{
    Token token;
    char ch;
    m_currentState = Tokenizer::State::Entry;
    while (m_currentState != State::Finish && stream.get(ch))
        m_currentState = m_stateFunctions[static_cast<unsigned int>(m_currentState)](token, ch);
    stream.unget();

    return token;
}

Tokenizer::State Tokenizer::entryState(Token& token, char ch)
{
    switch (ch) {
    case '\n':
    case '\r':
    case '\t':
    case ' ': return token.str.empty() ? State::Entry : State::Finish;
    case ';': return State::EatComment;
    case ',':
        if (token.str.empty()) {
            token.str = ch;
            token.type = TokenType::Separator;
        }
        return State::Finish;
    }

    token.str.push_back(ch);
    token.type = TokenType::Text;
    return State::Entry;
}

Tokenizer::State Tokenizer::eatCommentState(Token& token, char ch)
{
    if (ch != '\n')
        return State::EatComment;

    return State::Entry;
}

Tokenizer::State Tokenizer::m_currentState;
