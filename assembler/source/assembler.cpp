#include "assembler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

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

// TODO: refactor parsing of different number formats

bool Assembler::checkForSymbols(std::string& line)
{
    size_t token1End = line.find_first_of(" ");
    std::string token = line.substr(0, token1End); // TODO: change token to string_view
    
    if (token[0] == '*') {
        token = token.substr(2);
        uint16_t newAddress = std::atoi(token.c_str());
        uint16_t addressDiff = newAddress - m_address;
        m_address = newAddress;
        line = line.substr(0, 2) + std::to_string(addressDiff);
        return false;
    }

    MnemonicDesc desc;
    if (!isMnemonic(token, desc)) {
        size_t hasEqualSign = token.find('=');
        if (hasEqualSign == token.npos) {
            m_symbolTable.insert(std::make_pair<>(token, m_address));
            if (token1End != line.npos) {
                size_t token2Start = line.find_first_not_of(" ", token1End);
                line = line.substr(token2Start);
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
    }

    size_t token2Beg, token2End, token1End = line.find_first_of(" ");
    std::string token = line.substr(0, token1End); // TODO: change token to string_view
    std::uint8_t byte;

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
                byte = parseOperand(token);
                desc.byte |= byte;
                token = line.substr(token2End + 1);
                byte = parseOperand(token);
                line = std::to_string(desc.byte) + " " + std::to_string(byte);
            }
            else {
                // Should be address or sth
                // take as address for now
                std::uint16_t addr = m_symbolTable[token];
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

std::uint8_t Assembler::parseOperand(const std::string& token)
{
    switch (token[0]) {
    case 'R': { // Register operand
        if (token.size() != 2) {
            // Error
            return 0u;
        }

        if (token[1] < '0' || token[1] > '9') {
            if (token[1] < 'A' || token[1] > 'F') {
                // Error: Wrong register number
                return 0u;
            }

            return token[1] - 55; // to transform 'A'-'F' into 10-15
        }

        return token[1] - '0'; // to transform '0'-'9' into 0-9
    }
    case 'P': { // Registers pair operand
        if (token.size() != 2) {
            // Error
            return 0u;
        }

        if (token[1] < '0' || token[1] > '8') {
            // Error: Wrong registers pair number
            return 0u;
        }

        return 2 * (token[1] - '0');  // to transform '0'-'8' into 0, 2, 4, .., E
    }
    case '$': { // Hex value operand
        std::uint8_t byte = 0u;
        if (token[1] >= '0' && token[1] <= '9')
            byte = 16 * (token[1] - '0');
        else if (token[1] >= 'A' && token[1] <= 'F')
            byte = 16 * (token[1] - 55);
        else {
            // Error: Illformed hex number
            return 0u;
        }
        if (token[2] >= '0' && token[2] <= '9')
            byte += token[2] - '0';
        else if (token[2] >= 'A' && token[2] <= 'F')
            byte += token[2] - 55;
        else {
            // Error: Illformed hex number
            return 0u;
        }
        return byte;
    }
    }

    return 0u;
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

std::uint16_t Assembler::m_address;
std::unordered_map<std::string, std::uint16_t> Assembler::m_symbolTable;
std::unordered_map<std::string, Assembler::MnemonicDesc> Assembler::m_mnemonics = { // TODO: make it constexpr
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
    { "FIN", { InsType::TwoByte, 0x30 } },
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
    { "RD0", { InsType::Simple,  0xF6 } },
    { "RD1", { InsType::Simple,  0xF6 } },
    { "RD2", { InsType::Simple,  0xF6 } },
    { "RD3", { InsType::Simple,  0xF6 } },
    { "RDM", { InsType::Simple,  0xF6 } },
    { "RDR", { InsType::Simple,  0xF6 } },
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
};
