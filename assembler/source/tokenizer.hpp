#pragma once
#include <iostream>
#include <functional>
#include <variant>

class Tokenizer {
public:
    enum class TokenType {
        Invalid = -1,
        NewLine,
        Comma,
        Minus,
        Plus,
        Star,
        Equal,
        Number,
        Register,
        RegisterPair,
        Mnemonic,
        Label,
        BytePragma,
        EndPragma
    };

    struct Token {
        TokenType type;
        std::string text;
        size_t line;
        unsigned int value;

        Token();
        Token(TokenType type, std::string&& text, size_t line, unsigned int value = 0u) noexcept;
        Token(const Token& other) = default;
        Token(Token&& other) noexcept;
        Token& operator=(Token&& other) noexcept;
        Token& operator=(const Token& other) = default;
    };

    enum class InsType {
        Simple,
        Complex,
        TwoByte
    };

    struct MnemonicDesc {
        uint8_t byte;
        InsType type;
    };

    Tokenizer(std::istream& stream);
    Token getNext();
private:
    char peekChar();
    bool getChar();
    bool getCharIfMatch(char ch);
    bool getCharIfNotMatch(char ch);
    Token pragma();
    Token hexNumber();
    Token octNumber();
    Token binNumber();
    Token decNumber();
    Token label();
    bool isLetter(char ch);
    bool isAlphaNumeric(char ch);
    bool isDecDigit(char ch);
    bool isHexDigit(char ch);
    bool isOctDigit(char ch);

    std::istream& m_stream;
    char m_currentCh;
    size_t m_line;
    int m_col;

    static const std::unordered_map<std::string, MnemonicDesc> m_mnemonics;
};
