#include "lexer.hpp"

#include <cctype>
#include <fstream>

bool Lexer::open(const char* filename)
{
    std::ifstream fin(filename);
    if (!fin.is_open()) return false;

    std::string str;
    Token token;
    char c = fin.get();
    do {
        if (c == EOF) return true;
        if (isspace(c)) continue;

        if (c == ';') {
            while (c != '\n' && c != '\r' && c != EOF) c = fin.get();
            if (c == EOF) return true;
            continue;
        }
        
        if (isalpha(c)) {
            token.string = c;
            c = fin.get();
            while (!isspace(c)) {
                token.string += c;
                c = fin.get();
            }
            token.type = Token::Type::String;
        }

    } while (c = fin.get());

    return true;
}
