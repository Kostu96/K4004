#include "assembler/source/assembler.hpp"
#include "assembler/source/conversion_functions.hpp"

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

    output = new std::uint8_t[m_address];
    outputSize = m_address;

    while (std::getline(firstPass, line)) {
        parseLine(line);
        secondPass << line << ' ';
    }

    std::uint16_t byte;
    std::string token;
    for (size_t i = 0; i < m_address; ++i) {
        secondPass >> byte;
        output[i] = byte & 0x00FF;
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

void Assembler::parseLine(std::string& line)
{
    if (line[0] == '*') {
        uint16_t diff = std::atoi(line.c_str() + 2);
        line = "";
        for (uint16_t i = 0; i < diff - 1; ++i)
            line += "0 ";
        line += '0';
        return;
    }

    std::uint8_t byte;
    if (line[0] == '.') {
        line = line.substr(1);
        trimWhiteSpaces(line);
        byte = parseOperand(line);
        line = std::to_string(byte);
        return;
    }

    size_t token2Beg, token2End, token1End = line.find_first_of(" ");
    std::string token = line.substr(0, token1End); // TODO: change token to string_view

    MnemonicDesc desc;
    if (isMnemonic(token, desc)) {
        switch (desc.type) {
        case InsType::Simple:
            if (token1End != line.npos) {
                // Error: More than one instruction in line
            }
            line = std::to_string(desc.byte);
            break;
        case InsType::Complex:
            if (token1End == line.npos) {
                // Error: Missing instruction operand
            }
            token2Beg = line.find_first_not_of(" ", token1End);
            token2End = line.find_first_of(" ", token2Beg);
            token = line.substr(token2Beg, token2End - token2Beg + 1);
            byte = parseOperand(token);
            line = std::to_string(desc.byte | byte);
            break;
        case InsType::TwoByte:
            if (token1End == line.npos) {
                // Error: Missing instruction operand
            }
            token2Beg = line.find_first_not_of(" ", token1End);
            token2End = line.find_first_of(" ", token2Beg);
            token = line.substr(token2Beg, token2End - token2Beg);
            if (token[token.size() - 1] == ',') {
                // Should be 2 operands
                token = token.substr(0, token.size() - 1);
                auto ret = m_symbolTable.find(token);
                byte = ret != m_symbolTable.end() ? ret->second : parseOperand(token);
                desc.byte |= byte;
                token = line.substr(token2End + 1);
                ret = m_symbolTable.find(token);
                byte = ret != m_symbolTable.end() ? ret->second : parseOperand(token);
                line = std::to_string(desc.byte) + " " + std::to_string(byte);
            }
            else {
                // TODO: not sure if this could only be address for here
                auto ret = m_symbolTable.find(token);
                std::uint16_t addr = ret != m_symbolTable.end() ? ret->second : parseOperand(token);
                byte = addr & 0x00FF;
                addr &= 0x0F00;
                addr >>= 8;
                line = std::to_string(desc.byte | addr) + " " + std::to_string(byte);
            }
            break;
        }
    }
    else {
        // Error
    }
}

std::uint16_t Assembler::parseOperand(const std::string& token)
{
    switch (token[0]) {
    case 'R': {
        if (token.size() < 2 || token.size() > 3)
            return 0u; // Error
        // TODO: do sth better here
        if (token[1] >= '0' && token[1] <= '9') {
            if (token.size() > 2)
                return (token[1] - '0') * 10 + (token[2] - '0');
            return token[1] - '0';
        }
        if (token[1] >= 'A' && token[1] <= 'F')
            return token[1] - 'A' + 10;
        if (token[1] >= 'a' && token[1] <= 'f')
            return token[1] - 'a' + 10;

        return 0u; // Error: Wrong register number
    }
    case 'P': {
        if (token.size() != 2)
            return 0u; // Error

        if (token[1] < '0' || token[1] > '8')
            return 0u; // Error: Wrong registers pair number

        return 2 * (token[1] - '0');
    }
    case '$': {
        std::uint16_t word = 0u;
        if (hexStrToUint8(token.c_str() + 1, word))
            return word;
        else {
            // Error
            return 0u;
        }
    }
    case '0': {
        if (token.size() == 1)
            break;

        std::uint16_t word = 0u;
        if (octStrToUint8(token.c_str() + 1, word))
            return word;
        else {
            // Error
            return 0u;
        }
    }
    case '%': {
        std::uint16_t word = 0u;
        if (binStrToUint8(token.c_str() + 1, word))
            return word;
        else {
            // Error
            return 0u;
        }
    }
    }

    return std::atoi(token.c_str());
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
