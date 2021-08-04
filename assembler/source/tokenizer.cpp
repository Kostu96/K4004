#include "assembler/source/tokenizer.hpp"
#include "assembler/source/conversions.hpp"

#include "shared/source/assembly.hpp"

Tokenizer::Token::Token() :
    type(TokenType::Invalid),
    line(0u),
    value(0u) {}

Tokenizer::Token::Token(TokenType inType, std::string&& inText, size_t inLine, unsigned int inValue) noexcept :
    type(inType),
    text(std::move(inText)),
    line(inLine),
    value(inValue) {}

Tokenizer::Token::Token(Token&& other) noexcept :
    type(other.type),
    text(std::move(other.text)),
    line(other.line),
    value(other.value) {}

Tokenizer::Token& Tokenizer::Token::operator=(Token && other) noexcept
{
    type = other.type;
    text = std::move(other.text);
    line = other.line;
    value = other.value;
    return *this;
}

Tokenizer::Tokenizer(std::istream& stream) :
    m_stream(stream),
    m_currentCh(-1),
    m_line(1u),
    m_col(-1) {}

Tokenizer::Token Tokenizer::getNext()
{
    while (getChar()) {
        switch (m_currentCh) {
        case ';':
            while (getCharIfNotMatch('\n'));
            break; // Ignore comment.
        case ' ':
        case '\r':
        case '\t': break; // Ignore whitespace.
        case '\n': return Token(TokenType::NewLine, "\n", m_line++);
        case ',': return Token(TokenType::Comma, ",", m_line);
        case '-': return Token(TokenType::Minus, "-", m_line);
        case '+': return Token(TokenType::Plus, "+", m_line);
        case '*': return Token(TokenType::Star, "*", m_line);
        case '=': return Token(TokenType::Equal, "=", m_line);
        case '.': return pragma();
        case '$': return hexNumber();
        case '%': return binNumber();
        default:
            if (m_currentCh == '0') {
                char ch = peekChar();
                if (isOctDigit(ch))
                    return octNumber();

                return Token(TokenType::Number, "0", m_line, 0u);
            }

            if (isDecDigit(m_currentCh))
                return decNumber();

            if (isLetter(m_currentCh))
                return identifier();

            // TODO: Make better error handling
            std::cout << "Unexpected character: '" << m_currentCh << "' in line: " << m_line << " col: " << m_col << '\n';
        }
    }
    
    return Token(); // Return invalid token
}

char Tokenizer::peekChar()
{
    if (!m_stream.good())
        return '\0';

    return m_stream.peek();
}

bool Tokenizer::getChar()
{
    bool ret = static_cast<bool>(m_stream.get(m_currentCh));
    ++m_col;
    if (ret && m_currentCh == '\n') {
        m_col = -1;
    }
    return ret;
}

bool Tokenizer::getCharIfMatch(char ch)
{
    if (peekChar() == '\0' || peekChar() != ch)
        return false;

    return getChar();
}

bool Tokenizer::getCharIfNotMatch(char ch)
{
    if (peekChar() == '\0' || peekChar() == ch)
        return false;

    return getChar();
}

Tokenizer::Token Tokenizer::pragma()
{
    return Token();
}

Tokenizer::Token Tokenizer::hexNumber()
{
    std::string text = "$";
    char ch;
    bool isHex;
    do {
        ch = peekChar();
        isHex = isHexDigit(ch);
        if (isHex) {
            text.push_back(ch);
            getChar();
        }
    } while (isHex);

    std::string_view textView = text;
    return Token(TokenType::Number, std::move(text), m_line, textToHex(textView.substr(1)));
}

Tokenizer::Token Tokenizer::octNumber()
{
    std::string text = "0";
    char ch;
    bool isOct;
    do {
        ch = peekChar();
        isOct = isOctDigit(ch);
        if (isOct) {
            text.push_back(ch);
            getChar();
        }
    } while (isOct);

    std::string_view textView = text;
    return Token(TokenType::Number, std::move(text), m_line, textToOct(textView.substr(1)));
}

Tokenizer::Token Tokenizer::binNumber()
{
    std::string text = "%";
    char ch;
    bool isBin;
    do {
        ch = peekChar();
        isBin = (ch == '0' || ch == '1');
        if (isBin) {
            text.push_back(ch);
            getChar();
        }
    } while (isBin);

    std::string_view textView = text;
    return Token(TokenType::Number, std::move(text), m_line, textToBin(textView.substr(1)));
}

Tokenizer::Token Tokenizer::decNumber()
{
    std::string text;
    text.push_back(m_currentCh);
    char ch;
    bool isDec;
    do {
        ch = peekChar();
        isDec = isDecDigit(ch);
        if (isDec) {
            text.push_back(ch);
            getChar();
        }
    } while (isDec);

    return Token(TokenType::Number, std::move(text), m_line, textToDec(text));
}

Tokenizer::Token Tokenizer::identifier()
{
    std::string text;
    text.push_back(m_currentCh);
    char ch;
    bool isAlphaNum;
    do {
        ch = peekChar();
        isAlphaNum = isAlphaNumeric(ch);
        if (isAlphaNum) {
            text.push_back(ch);
            getChar();
        }
    } while (isAlphaNum);

    if (text.size() <= 3) {
        auto x = m_mnemonics.find(text);
        if (x != m_mnemonics.end()) {
            auto desc = x->second;
            return Token(TokenType::Mnemonic, std::move(text), m_line, desc.byte);
        }

        if (text.size() == 2 && (text[0] == 'R' || text[0] == 'P')) {
            auto x = m_registers.find(text);
            if (x != m_registers.end()) {
                auto byte = x->second;
                return Token(text[0] == 'R' ? TokenType::Register : TokenType::RegisterPair,
                             std::move(text),
                             m_line,
                             byte);
            }
        }
    }

    return Token(TokenType::Label, std::move(text), m_line);
}

bool Tokenizer::isLetter(char ch)
{
    return (ch >= 'a' && ch <= 'z') ||
           (ch >= 'A' && ch <= 'Z');
}

bool Tokenizer::isAlphaNumeric(char ch)
{
    return isLetter(ch) || isDecDigit(ch) || ch == '_';
}

bool Tokenizer::isDecDigit(char ch)
{
    return ch >= '0' && ch <= '9';
}

bool Tokenizer::isHexDigit(char ch)
{
    return (ch >= '0' && ch <= '9') ||
           (ch >= 'a' && ch <= 'f') ||
           (ch >= 'A' && ch <= 'F');
}

bool Tokenizer::isOctDigit(char ch)
{
    return ch >= '0' && ch <= '7';
}

const std::unordered_map<std::string, Tokenizer::MnemonicDesc> Tokenizer::m_mnemonics = {
    { "NOP", { +AsmIns::NOP, InsType::Simple } },
    { "HLT", { +AsmIns::HLT, InsType::Simple } },
    { "BBS", { +AsmIns::BBS, InsType::Simple } },
    { "LCR", { +AsmIns::LCR, InsType::Simple } },
    { "OR4", { +AsmIns::OR4, InsType::Simple } },
    { "OR5", { +AsmIns::OR5, InsType::Simple } },
    { "AN6", { +AsmIns::AN6, InsType::Simple } },
    { "AN7", { +AsmIns::AN7, InsType::Simple } },
    { "DB0", { +AsmIns::DB0, InsType::Simple } },
    { "DB1", { +AsmIns::DB1, InsType::Simple } },
    { "SB0", { +AsmIns::SB0, InsType::Simple } },
    { "SB1", { +AsmIns::SB1, InsType::Simple } },
    { "EIN", { +AsmIns::EIN, InsType::Simple } },
    { "DIN", { +AsmIns::DIN, InsType::Simple } },
    { "RPM", { +AsmIns::DIN, InsType::Simple } },
    { "JCN", { +AsmIns::JCN, InsType::TwoByte } },
    { "FIM", { +AsmIns::FIM, InsType::TwoByte } },
    { "SRC", { +AsmIns::SRC, InsType::Complex } },
    { "FIN", { +AsmIns::FIN, InsType::Complex } },
    { "JIN", { +AsmIns::JIN, InsType::Complex } },
    { "JUN", { +AsmIns::JUN, InsType::TwoByte } },
    { "JMS", { +AsmIns::JMS, InsType::TwoByte } },
    { "INC", { +AsmIns::INC, InsType::Complex } },
    { "ISZ", { +AsmIns::ISZ, InsType::TwoByte } },
    { "ADD", { +AsmIns::ADD, InsType::Complex } },
    { "SUB", { +AsmIns::SUB, InsType::Complex } },
    { "LD",  { +AsmIns::LD,  InsType::Complex } },
    { "XCH", { +AsmIns::XCH, InsType::Complex } },
    { "BBL", { +AsmIns::BBL, InsType::Complex } },
    { "LDM", { +AsmIns::LDM, InsType::Complex } },
    { "WRM", { +AsmIns::WRM, InsType::Simple } },
    { "WMP", { +AsmIns::WMP, InsType::Simple } },
    { "WRR", { +AsmIns::WRR, InsType::Simple } },
    { "WPM", { +AsmIns::WPM, InsType::Simple } },
    { "WR0", { +AsmIns::WR0, InsType::Simple } },
    { "WR1", { +AsmIns::WR1, InsType::Simple } },
    { "WR2", { +AsmIns::WR2, InsType::Simple } },
    { "WR3", { +AsmIns::WR3, InsType::Simple } },
    { "SBM", { +AsmIns::SBM, InsType::Simple } },
    { "RDM", { +AsmIns::RDM, InsType::Simple } },
    { "RDR", { +AsmIns::RDR, InsType::Simple } },
    { "ADM", { +AsmIns::ADM, InsType::Simple } },
    { "RD0", { +AsmIns::RD0, InsType::Simple } },
    { "RD1", { +AsmIns::RD1, InsType::Simple } },
    { "RD2", { +AsmIns::RD2, InsType::Simple } },
    { "RD3", { +AsmIns::RD3, InsType::Simple } },
    { "CLB", { +AsmIns::CLB, InsType::Simple } },
    { "CLC", { +AsmIns::CLC, InsType::Simple } },
    { "IAC", { +AsmIns::IAC, InsType::Simple } },
    { "CMC", { +AsmIns::CMC, InsType::Simple } },
    { "CMA", { +AsmIns::CMA, InsType::Simple } },
    { "RAL", { +AsmIns::RAL, InsType::Simple } },
    { "RAR", { +AsmIns::RAR, InsType::Simple } },
    { "TCC", { +AsmIns::TCC, InsType::Simple } },
    { "DAC", { +AsmIns::DAC, InsType::Simple } },
    { "TCS", { +AsmIns::TCS, InsType::Simple } },
    { "STC", { +AsmIns::STC, InsType::Simple } },
    { "DAA", { +AsmIns::DAA, InsType::Simple } },
    { "KBP", { +AsmIns::KBP, InsType::Simple } },
    { "DCL", { +AsmIns::DCL, InsType::Simple } },
};

const std::unordered_map<std::string, uint8_t> Tokenizer::m_registers = {
    { "R0", +AsmReg::R0 },
    { "R1", +AsmReg::R1 },
    { "R2", +AsmReg::R2 },
    { "R3", +AsmReg::R3 },
    { "R4", +AsmReg::R4 },
    { "R5", +AsmReg::R5 },
    { "R6", +AsmReg::R6 },
    { "R7", +AsmReg::R7 },
    { "R8", +AsmReg::R8 },
    { "R9", +AsmReg::R9 },
    { "RA", +AsmReg::RA },
    { "RB", +AsmReg::RB },
    { "RC", +AsmReg::RC },
    { "RD", +AsmReg::RD },
    { "RE", +AsmReg::RE },
    { "RF", +AsmReg::RF },
    { "P0", +AsmReg::P0 },
    { "P1", +AsmReg::P1 },
    { "P2", +AsmReg::P2 },
    { "P3", +AsmReg::P3 },
    { "P4", +AsmReg::P4 },
    { "P5", +AsmReg::P5 },
    { "P6", +AsmReg::P6 },
    { "P7", +AsmReg::P7 }
};
