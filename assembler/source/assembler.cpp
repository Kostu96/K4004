#include "assembler/source/assembler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

Assembler::Assembler() :
    m_address(0u),
    m_mnemonics({ // TODO: Make it constexpr
        { "ADD", { InsType::Complex, 0x80 } },
        { "ADM", { InsType::Simple,  0xEB } },
        { "BBL", { InsType::Complex, 0xC0 } },
        { "CLB", { InsType::Simple,  0xF0 } },
        { "CLC", { InsType::Simple,  0xF1 } },
        { "CMA", { InsType::Simple,  0xF4 } },
        { "CMC", { InsType::Simple,  0xF3 } },
        { "DAA", { InsType::Simple,  0xFB } },
        { "DAC", { InsType::Simple,  0xF8 } },
        { "DCL", { InsType::Simple,  0xFD } },
        { "FIM", { InsType::TwoByte, 0x20 } },
        { "FIN", { InsType::Complex, 0x30 } },
        { "IAC", { InsType::Simple,  0xF2 } },
        { "INC", { InsType::Complex, 0x60 } },
        { "ISZ", { InsType::TwoByte, 0x70 } },
        { "JCN", { InsType::TwoByte, 0x10 } },
        { "JIN", { InsType::Complex, 0x31 } },
        { "JMS", { InsType::TwoByte, 0x50 } },
        { "JUN", { InsType::TwoByte, 0x40 } },
        { "KBP", { InsType::Simple,  0xFC } },
        { "LD",  { InsType::Complex, 0xA0 } },
        { "LDM", { InsType::Complex, 0xD0 } },
        { "NOP", { InsType::Simple,  0x00 } },
        { "RAL", { InsType::Simple,  0xF5 } },
        { "RAR", { InsType::Simple,  0xF6 } },
        { "RD0", { InsType::Simple,  0xEC } },
        { "RD1", { InsType::Simple,  0xED } },
        { "RD2", { InsType::Simple,  0xEE } },
        { "RD3", { InsType::Simple,  0xEF } },
        { "RDM", { InsType::Simple,  0xE9 } },
        { "RDR", { InsType::Simple,  0xEA } },
        { "SBM", { InsType::Simple,  0xE8 } },
        { "SRC", { InsType::Complex, 0x21 } },
        { "STC", { InsType::Simple,  0xFA } },
        { "SUB", { InsType::Complex, 0x90 } },
        { "TCC", { InsType::Simple,  0xF7 } },
        { "TCS", { InsType::Simple,  0xF9 } },
        { "WMP", { InsType::Simple,  0xE1 } },
        { "WR0", { InsType::Simple,  0xE4 } },
        { "WR1", { InsType::Simple,  0xE5 } },
        { "WR2", { InsType::Simple,  0xE6 } },
        { "WR3", { InsType::Simple,  0xE7 } },
        { "WRM", { InsType::Simple,  0xE0 } },
        { "WRR", { InsType::Simple,  0xE2 } },
        { "XCH", { InsType::Complex, 0xB0 } }
    }) {}

bool Assembler::assemble(const char* filename, std::uint8_t*& output, size_t& outputSize)
{
    std::fstream file(filename);
    if (!file.is_open())
        return false;

    m_symbolTable.clear();
    m_address = 0u;

    std::stringstream firstPass, secondPass;
    std::string line;
    while (std::getline(file, line)) {
        if (trimComments(line)) continue;
        if (trimWhiteSpaces(line)) continue;
        if (checkForSymbols(line)) continue;
        firstPass << line << '\n';
    }
    file.close();

    output = new uint8_t[m_address];
    outputSize = m_address;

    uint16_t outputIndex = 0u;
    while (std::getline(firstPass, line)) {
        if (!parseLine(line, output, outputIndex))
            return false;
    }

    return true;
}

bool Assembler::trimComments(std::string& line)
{
    line = line.substr(0, line.find_first_of(';'));
    return line.empty();
}

bool Assembler::trimWhiteSpaces(std::string& line)
{
    size_t off = line.find_first_not_of(" \t");
    if (off == line.npos) off = 0;
    size_t count = line.find_last_not_of(" \t") - off + 1;
    line = line.substr(off, count);
    return line.empty();
}

bool Assembler::checkForSymbols(std::string& line)
{
    size_t token1End = line.find_first_of(" ");
    std::string token = line.substr(0, token1End); // TODO: change token to string_view
    
    if (token[0] == '$')
        return true;

    if (token[0] == '*') {
        token = token.substr(2);
        uint16_t newAddress = std::atoi(token.c_str());
        uint16_t addressDiff = newAddress - m_address;
        m_address = newAddress;
        line = line.substr(0, 2) + std::to_string(addressDiff);
        return false;
    }

    if (token[0] == '.') {
        if (token == ".BYTE") {
            line = '.' + line.substr(5);
            ++m_address;
            return false;
        }
        else {
            // Error
            return true;
        }
    }

    MnemonicDesc desc;
    if (!isMnemonic(token, desc)) {
        size_t hasEqualSign = token.find('=');
        if (hasEqualSign == token.npos) {
            m_symbolTable.insert(std::make_pair<>(token, m_address));
            if (token1End != line.npos) {
                size_t token2Start = line.find_first_not_of(" ", token1End);
                line = line.substr(token2Start);
                return checkForSymbols(line);
            }
            else
                line = "";
        }
        else {
            uint16_t value = std::atoi(token.c_str() + hasEqualSign + 1);
            token = token.substr(0, hasEqualSign);
            m_symbolTable.insert(std::make_pair<>(token, value));
            line = "";
        }
    }
    else {
        m_address += desc.type == InsType::TwoByte ? 2 : 1;
    }

    return line.empty();
}

bool Assembler::parseLine(const std::string& line, uint8_t* output, uint16_t& outputIndex)
{
    // TODO: change line into string_view
    if (line[0] == '*') {
        uint16_t diff = std::atoi(line.c_str() + 2);
        for (uint16_t i = 0; i < diff; ++i)
            output[outputIndex++] = 0u;
        return true;
    }

    std::string token;

    if (line[0] == '.') {
        token = line.substr(1);
        trimWhiteSpaces(token);
        output[outputIndex++] = parseOperand(token);
        return true;
    }

    size_t token2Beg, token2End, token1End = line.find_first_of(" ");
    token = line.substr(0, token1End); // TODO: change token to string_view

    MnemonicDesc desc;
    if (isMnemonic(token, desc)) {
        switch (desc.type) {
        case InsType::Simple:
            if (token1End != line.npos) {
                return false; // Error: More than one instruction in line
            }
            output[outputIndex++] = desc.byte;
            break;
        case InsType::Complex:
            if (token1End == line.npos) {
                return false; // Error: Missing instruction operand
            }
            token2Beg = line.find_first_not_of(" ", token1End);
            token2End = line.find_first_of(" ", token2Beg);
            token = line.substr(token2Beg, token2End - token2Beg + 1);
            output[outputIndex++] = desc.byte | parseOperand(token);
            break;
        case InsType::TwoByte:
            if (token1End == line.npos) {
                return false; // Error: Missing instruction operand
            }
            token2Beg = line.find_first_not_of(" ", token1End);
            token2End = line.find_first_of(" ", token2Beg);
            token = line.substr(token2Beg, token2End - token2Beg);
            if (token[token.size() - 1] == ',') {
                // Should be 2 operands
                token = token.substr(0, token.size() - 1);
                auto ret = m_symbolTable.find(token);
                output[outputIndex++] = desc.byte | (ret != m_symbolTable.end() ? ret->second : parseOperand(token));
                token = line.substr(token2End + 1);
                ret = m_symbolTable.find(token);
                output[outputIndex++] = ret != m_symbolTable.end() ? ret->second : parseOperand(token);
            }
            else {
                // TODO: not sure if this could only be address for here
                auto ret = m_symbolTable.find(token);
                std::uint16_t addr = ret != m_symbolTable.end() ? ret->second : parseOperand(token);
                output[outputIndex + 1] = addr & 0x00FF;
                addr &= 0x0F00;
                addr >>= 8;
                output[outputIndex] = desc.byte | addr;
                outputIndex += 2;
            }
            break;
        }
    }
    else {
        return false; // Error
    }

    return true;
}

std::uint16_t Assembler::parseOperand(const std::string& token)
{
    std::uint16_t word = 0u;
    switch (token[0]) {
    case 'R':
        parseRegisterOperand(token, word);
        return word;
    case 'P':
        parseRegisterPairOperand(token, word);
        return word;
    case '$':
        parseHexNumberOperand(token, word);
        return word;
    case '0':
        if (token.size() == 1)
            break;

        parseOctNumberOperand(token, word);
        return word;
    case '%':
        parseBinNumberOperand(token, word);
        return word;
    }

    return std::atoi(token.c_str());
}

bool Assembler::parseRegisterOperand(const std::string& token, uint16_t& value)
{
    if (token.size() < 2 || token.size() > 3)
        return false;

    if (token[1] >= '0' && token[1] <= '9') {
        if (token.size() > 2)
            if (token[2] >= '0' && token[2] <= '9')
                value = (token[1] - '0') * 10 + (token[2] - '0');
            else
                return false;
        else 
            value = token[1] - '0';
    }

    if (token[1] >= 'A' && token[1] <= 'F')
        value = token[1] - 'A' + 10;
    else if (token[1] >= 'a' && token[1] <= 'f')
        value = token[1] - 'a' + 10;
    else
        return false;
    
    return true;
}

bool Assembler::parseRegisterPairOperand(const std::string& token, uint16_t& value)
{
    if (token.size() != 2)
        return false;

    if (token[1] < '0' || token[1] > '7')
        return false;


    value = 2 * (token[1] - '0');
    return true;
}

bool Assembler::parseHexNumberOperand(const std::string& token, uint16_t& value)
{
    size_t length = token.size();
    if (length == 6 || length == 0)
        return false; // String representation of a to big number or empty

    value = 0;
    for (size_t i = 1; i < length; ++i) {
        if (token[i] >= '0' && token[i] <= '9')
            value += (1 << 4 * (length - i - 1)) * (token[i] - '0');
        else if (token[i] >= 'a' && token[i] <= 'f')
            value += (1 << 4 * (length - i - 1)) * (token[i] - 'a' + 10);
        else if (token[i] >= 'A' && token[i] <= 'F')
            value += (1 << 4 * (length - i - 1)) * (token[i] - 'A' + 10);
        else
            return false; // Illformed bin number
    }

    return true;
}

bool Assembler::parseBinNumberOperand(const std::string& token, uint16_t& value)
{
    size_t length = token.size();
    if (length == 18 || length == 0)
        return false; // String representation of a to big number or empty

    value = 0;
    for (size_t i = 1; i < length; ++i) {
        if (token[i] == '1')
            value += 1 << (length - i - 1);
        else if (token[i] != '0')
            return false; // Illformed bin number
    }

    return true;
}

bool Assembler::parseOctNumberOperand(const std::string& token, uint16_t& value)
{
    size_t length = token.size();
    if (length == 8 || length == 0)
        return false; // String representation of a to big number or empty

    value = 0;
    for (size_t i = 0; i < length; ++i) {
        if (token[i] >= '0' && token[i] <= '7')
            value += (1 << 3 * (length - i - 1)) * (token[i] - '0');
        else
            return false; // Illformed bin number
    }

    return true;
}


bool Assembler::isMnemonic(const std::string& token, MnemonicDesc& desc)
{
    if (token.size() > 3) return false;

    auto x = m_mnemonics.find(token);
    if (x != m_mnemonics.end()) {
        desc = x->second;
        return true;
    }

    return false;
}
