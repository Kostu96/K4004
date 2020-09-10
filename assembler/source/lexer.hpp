#pragma once
#include <queue>
#include <string>

class Lexer
{
public:
    struct Token {
        Token() = default;
        Token(const Token&) = delete;
        Token(Token&& other) noexcept :
            string(other.string) {}
        ~Token() {}
      
        enum class Type {
            String,
            Number4bit,
            Number8bit,
            Number12bit,
            Separator,
            Assignment,
            Pragma,
            Unknown
        };

        std::string string = "";
        unsigned short number = 0u;
        char separator = 0;
        Type type = Type::Unknown;
    };

    Lexer() = default;
    bool open(const char* filename);
private:
    std::queue<Token> m_tokenStream;
};
