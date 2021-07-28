#include "assembler/source/tokenizer.hpp"
#include "assembler/source/conversions.hpp"

#include <cassert>

Tokenizer::Token::Token(const char* inStr, TokenType inType) :
    str(inStr),
    value(0u),
    type(inType) {}

Tokenizer::Token::Token(unsigned int inValue, TokenType inType) :
    str(""),
    value(inValue),
    type(inType) {}

Tokenizer::Token::Token(Token&& other) noexcept :
    str(std::move(other.str)),
    type(other.type) {}

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

    return token;
}

Tokenizer::State Tokenizer::entryState(Token& token, char ch)
{
    switch (ch) {
    case '\n':
        token.str = ch;
        token.type = TokenType::NewLine;
        return State::Finish;
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
    case '%': return State::BinNumber;
    case '$': return State::HexNumber;
    }

    token.str.push_back(ch);
    token.type = TokenType::Text;
    return State::Entry;
}

Tokenizer::State Tokenizer::eatCommentState(Token& token, char ch)
{
    if (ch != '\n')
        return State::EatComment;

    return State::EatCommentNL;
}

Tokenizer::State Tokenizer::eatCommentNLState(Token& token, char ch)
{
    if (ch != '\n')
        return State::Entry;
    
    return State::EatCommentNL;
}

Tokenizer::State Tokenizer::BinNumberState(Token& token, char ch)
{
    if (ch == '0' || ch == '1') {
        token.str.push_back(ch);
        return State::BinNumber;
    }

    token.value = textToBin(token.str);
    token.str = "";
    return State::Finish;
}

Tokenizer::State Tokenizer::HexNumberState(Token& token, char ch)
{
    if ((ch >= '0' && ch <= '9') ||
        (ch >= 'A' && ch <= 'F') ||
        (ch >= 'a' && ch <= 'f')) {
        token.str.push_back(ch);
        return State::HexNumber;
    }

    token.value = textToHex(token.str);
    token.str = "";
    return State::Finish;
}

Tokenizer::State Tokenizer::m_currentState;
