#include "assembler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

std::unordered_map<std::string, std::uint16_t> Assembler::m_symbolTable;
std::uint16_t Assembler::m_address;


bool Assembler::assemble(const char* filename, std::uint8_t*& output, size_t& outputSize)
{
    std::fstream file(filename);
    if (!file.is_open())
        return false;

    m_symbolTable.clear();
    m_address = 0u;

    std::stringstream ss;
    std::string line;
    while (std::getline(file, line)) {
        line = line.substr(0, line.find_first_of(';')); // trim comments
        if (line.empty()) continue;

        size_t off = line.find_first_not_of(" \t");
        if (off == line.npos) off = 0;
        size_t count = line.find_last_not_of(" \t") - off + 1;
        line = line.substr(off, count); // trim whitespace
        if (line.empty()) continue;

        parseLine(line);
        if (line.empty()) continue;

        ss << line << '\n';
    }
    
    std::cout << ss.str();
    return true;
}

void Assembler::parseLine(std::string& line)
{
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
            }
            else {
                // Should be address
            }
            break;
        }
        // For Debug:
        line = std::to_string(m_address) + " " + line;

        m_address += desc.type == InsType::TwoByte ? 2 : 1;
    }
    else {
        // take as label for now:
        m_symbolTable.insert(std::make_pair<>(token, m_address));
        line = "";
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
    case '$': {

    }
    }

    return 0u;
}

bool Assembler::isMnemonic(const std::string& token, MnemonicDesc& desc)
{
    if (token == "LD") {
        desc.type = InsType::Complex;
        desc.byte = 0xA0;
        return true; // LD
    }

    if (token.size() != 3) return false;

    switch (token[0]) {
    case 'A':
        if (token[1] == 'D') {
            if (token[2] == 'D') {
                desc.type = InsType::Complex;
                desc.byte = 0x80;
                return true; // ADD
            }
            if (token[2] == 'M') {
                desc.type = InsType::Simple;
                desc.byte = 0xEB;
                return true; // ADM
            }
        }
        break;
    case 'B':
        if (token[1] == 'B' && token[2] == 'L') {
            desc.type = InsType::Complex;
            desc.byte = 0xC0;
            return true; // BBL
        }
        break;
    case 'C':
        if (token[1] == 'L') {
            if (token[2] == 'B') {
                desc.type = InsType::Simple;
                desc.byte = 0xF0;
                return true; // CLB
            }
            if (token[2] == 'C') {
                desc.type = InsType::Simple;
                desc.byte = 0xF1;
                return true; // CLC
            }
        }
        else if (token[1] == 'M') {
            if (token[2] == 'A') {
                desc.type = InsType::Simple;
                desc.byte = 0xF4;
                return true; // CMA
            }
            if (token[2] == 'C') {
                desc.type = InsType::Simple;
                desc.byte = 0xF3;
                return true; // CMC
            }
        }
        break;
    case 'D':
        if (token[1] == 'A') {
            if (token[2] == 'A') {
                desc.type = InsType::Simple;
                desc.byte = 0xFB;
                return true; // DAA
            }
            if (token[2] == 'C') {
                desc.type = InsType::Simple;
                desc.byte = 0xF8;
                return true; // DAC
            }
        }
        else if (token[1] == 'C' && token[2] == 'L') {
            desc.type = InsType::Simple;
            desc.byte = 0xFD;
            return true; // DCL
        }
        break;
    case 'F':
        if (token[1] == 'I') {
            if (token[2] == 'M') {
                desc.type = InsType::TwoByte;
                desc.byte = 0x20;
                return true; // FIM
            }
            if (token[2] == 'N') {
                desc.type = InsType::Complex;
                desc.byte = 0x30;
                return true; // FIN
            }
        }
        break;
    case 'I':
        switch (token[1]) {
        case 'A':
            if (token[2] == 'C') {
                desc.type = InsType::Simple;
                desc.byte = 0xF2;
                return true; // IAC
            }
            break;
        case 'N':
            if (token[2] == 'C') {
                desc.type = InsType::Complex;
                desc.byte = 0x60;
                return true; // INC
            }
            break;
        case 'S':
            if (token[2] == 'Z') {
                desc.type = InsType::TwoByte;
                desc.byte = 0x70;
                return true; // ISZ
            }
            break;
        }
        break;
    case 'J':
        if (token[1] == 'C' && token[2] == 'N') {

            return true; // JCN
        }
        if (token[1] == 'I' && token[2] == 'N') {

            return true; // JIN
        }
        if (token[1] == 'M' && token[2] == 'S') {
            
            return true; // JMS
        }
        if (token[1] == 'U' && token[2] == 'N') {
            
            return true; // JUN
        }
        break;
    case 'K':
        if (token[1] == 'B' && token[2] == 'P') {
            
            return true; // KBP
        }
        break;
    case 'L':
        if (token[1] == 'D' && token[2] == 'M') {
            
            return true; // LDM
        }
        break;
    case 'N':
        if (token[1] == 'O' && token[2] == 'P') {
            
            return true; // NOP
        }
        break;
    case 'R':
        if (token[1] == 'A') {
            if (token[2] == 'L') {
                
                return true; // RAL
            }
            if (token[2] == 'R') {
                
                return true; // RAR
            }
        }
        else if (token[1] == 'D') {
            switch (token[2]) {
            case '0':
                
                return true; // RD0
            case '1':
                
                return true; // RD1
            case '2':
                
                return true; // RD2
            case '3':
                
                return true; // RD3
            case 'M':
                
                return true; // RDM
            case 'R':
                
                return true; // RDR
            }
        }
        break;
    case 'S':
        switch (token[1]) {
        case 'B':
            if (token[2] == 'M') {
                
                return true; // SBM
            }
            break;
        case 'R':
            if (token[2] == 'C') {
                
                return true; // SRC
            }
            break;
        case 'T':
            if (token[2] == 'C') {
                
                return true; // STC
            }
            break;
        case 'U':
            if (token[2] == 'B') {
                
                return true; // SUB
            }
            break;
        }
        break;
    case 'T':
        if (token[1] == 'C') {
            if (token[2] == 'C') {
                
                return true; // TCC
            }
            if (token[2] == 'S') {
                
                return true; // TCS
            }
        }
        break;
    case 'W':
        if (token[1] == 'M' && token[2] == 'P') {
            
            return true; // WMP
        }
        if (token[1] == 'R') {
            switch (token[2]) {
            case '0':
                
                return true; // WR0
            case '1':
                
                return true; // WR1
            case '2':
                
                return true; // WR2
            case '3':
                
                return true; // WR3
            case 'M':
                
                return true; // WRM
            case 'R':
                
                return true; // WRR
            }
        }
        break;
    case 'X':
        if (token[1] == 'C' && token[2] == 'H') {
            
            return true; // XCH
        }
        break;
    }

    return false;
}
