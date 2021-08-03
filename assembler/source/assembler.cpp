#include "assembler/source/assembler.hpp"
#include "assembler/source/conversions.hpp"

#include "shared/source/assembly.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <bitset>

Assembler::Assembler() :
    m_address(0u),
    m_metalMaskLength(0u),
    m_mnemonics({ // TODO: Make it constexpr
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
    }) {}

bool Assembler::assemble(const char* filename, std::vector<uint8_t>& output, bool i4004ModeEnabled)
{
    std::fstream file(filename);
    if (!file.is_open())
        return false;

    m_symbolTable.clear();
    m_address = 0u;
    m_metalMaskLength = 2u;

    std::stringstream ss;
    std::string line;
    while (std::getline(file, line)) {
        if (trimComments(line)) continue;
        if (trimWhiteSpaces(line)) continue;
        if (checkForSymbols(line)) continue;
        ss << line << '\n';
    }
    file.close();

    output.reserve(m_address + m_metalMaskLength);
    output.push_back(0xFE);
    while (std::getline(ss, line)) {
        if (m_metalMaskLength-- == 2u)
            output.push_back(0xFF);

        if (!parseLine(line, output))
            return false;
    }

    return true;
}

bool Assembler::disassemble(const std::vector<uint8_t>& bytecode, std::vector<std::string>& output)
{
    m_metalMaskLength = 1u;
    if (bytecode[0] != 0xFEu) {
        return false;
        // Error
    }
    size_t i = 0;
    while (bytecode[++i] != 0xFF);
    m_metalMaskLength += i;

    output.reserve(bytecode.size() - m_metalMaskLength);
    std::stringstream ss;
    ss << std::setfill('0') << std::hex << std::uppercase;
    bool twoByte = false;
    for (size_t i = m_metalMaskLength; i < bytecode.size(); ++i) {
        uint8_t opcode = getOpcodeFromByte(bytecode[i]);
        // TODO: Refactor
        switch (opcode) {
        case +AsmIns::NOP: ss << "NOP"; break;
        case +AsmIns::WRM: ss << "WRM"; break;
        case +AsmIns::WMP: ss << "WMP"; break;
        case +AsmIns::WRR: ss << "WRR"; break;
        case +AsmIns::WR0: ss << "WR0"; break;
        case +AsmIns::WR1: ss << "WR1"; break;
        case +AsmIns::WR2: ss << "WR2"; break;
        case +AsmIns::WR3: ss << "WR3"; break;
        case +AsmIns::SBM: ss << "SBM"; break;
        case +AsmIns::RDM: ss << "RDM"; break;
        case +AsmIns::RDR: ss << "RDR"; break;
        case +AsmIns::ADM: ss << "ADM"; break;
        case +AsmIns::RD0: ss << "RD0"; break;
        case +AsmIns::RD1: ss << "RD1"; break;
        case +AsmIns::RD2: ss << "RD2"; break;
        case +AsmIns::RD3: ss << "RD3"; break;
        case +AsmIns::CLB: ss << "CLB"; break;
        case +AsmIns::CLC: ss << "CLC"; break;
        case +AsmIns::IAC: ss << "IAC"; break;
        case +AsmIns::CMC: ss << "CMC"; break;
        case +AsmIns::CMA: ss << "CMA"; break;
        case +AsmIns::RAL: ss << "RAL"; break;
        case +AsmIns::RAR: ss << "RAR"; break;
        case +AsmIns::TCC: ss << "TCC"; break;
        case +AsmIns::DAC: ss << "DAC"; break;
        case +AsmIns::TCS: ss << "TCS"; break;
        case +AsmIns::STC: ss << "STC"; break;
        case +AsmIns::DAA: ss << "DAA"; break;
        case +AsmIns::KBP: ss << "KBP"; break;
        case +AsmIns::DCL: ss << "DCL"; break;
        case +AsmIns::JCN: {
            twoByte = true;
            ss << "JCN %";
            uint8_t reg = bytecode[i] & 0x0Fu;
            ss << std::bitset<4>(reg) << ", $";
            ss << std::setw(2) << +bytecode[++i];
        } break;
        case +AsmIns::FIM: {
            twoByte = true;
            ss << "FIM P";
            uint8_t regPair = (bytecode[i] & 0x0Fu) >> 1;
            ss << +regPair << ", ";
            ss << '$' << std::setw(2) << +bytecode[++i];
        } break;
        case +AsmIns::SRC: {
            ss << "SRC P";
            uint8_t regPair = (bytecode[i] & 0x0Fu) >> 1;
            ss << +regPair;
        } break;
        case +AsmIns::FIN: {
            ss << "FIN P";
            uint8_t regPair = (bytecode[i] & 0x0Fu) >> 1;
            ss << +regPair;
        } break;
        case +AsmIns::JIN: {
            ss << "JIN P";
            uint8_t regPair = (bytecode[i] & 0x0Fu) >> 1;
            ss << +regPair;
        } break;
        case +AsmIns::JUN: {
            twoByte = true;
            ss << "JUN $";
            uint8_t addrHP = bytecode[i] & 0x0Fu;
            ss << +addrHP << std::setw(2) << +bytecode[++i];
        } break;
        case +AsmIns::JMS: {
            twoByte = true;
            ss << "JMS $";
            uint8_t addrHP = bytecode[i] & 0x0Fu;
            ss << +addrHP << std::setw(2) << +bytecode[++i];
        } break;
        case +AsmIns::INC: {
            ss << "INC R";
            uint8_t reg = bytecode[i] & 0x0Fu;
            ss << +reg;
        } break;
        case +AsmIns::ISZ: {
            twoByte = true;
            ss << "ISZ R";
            uint8_t reg = bytecode[i] & 0x0Fu;
            ss << +reg << ", $";
            ss << std::setw(2) << +bytecode[++i];
        } break;
        case +AsmIns::ADD: {
            ss << "ADD R";
            uint8_t reg = bytecode[i] & 0x0Fu;
            ss << +reg;
        } break;
        case +AsmIns::SUB: {
            ss << "SUB R";
            uint8_t reg = bytecode[i] & 0x0Fu;
            ss << +reg;
        } break;
        case +AsmIns::LD: {
            ss << "LD R";
            uint8_t reg = bytecode[i] & 0x0Fu;
            ss << +reg;
        } break;
        case +AsmIns::XCH: {
            ss << "XCH R";
            uint8_t reg = bytecode[i] & 0x0Fu;
            ss << +reg;
        } break;
        case +AsmIns::BBL: {
            ss << "BBL $";
            uint8_t ch = bytecode[i] & 0x0Fu;
            ss << +ch;
        } break;
        case +AsmIns::LDM: {
            ss << "LDM $";
            uint8_t ch = bytecode[i] & 0x0Fu;
            ss << +ch;
        } break;
        default: {
            ss << "???";
        } break;
        }
        output.push_back(ss.str());
        ss.str(std::string());
        if (twoByte) {
            output.push_back(std::string());
            twoByte = false;
        }
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
    std::string token = line.substr(0, token1End);
    
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

        if (token == ".END")
            return true;

        return true; // Error
    }

    MnemonicDesc desc;
    if (!isMnemonic(token, desc)) {
        size_t hasEqualSign = token.find('=');
        if (hasEqualSign == token.npos) {
            m_symbolTable.insert(std::make_pair<>(token, static_cast<uint16_t>(m_address)));
            if (token1End != line.npos) {
                size_t token2Start = line.find_first_not_of(" ", token1End);
                line = line.substr(token2Start);
                return checkForSymbols(line);
            }
            
            return true;
        }

        uint16_t value = std::atoi(token.c_str() + hasEqualSign + 1);
        token = token.substr(0, hasEqualSign);
        m_symbolTable.insert(std::make_pair<>(token, value));
        return true;
    }

    m_address += desc.type == InsType::TwoByte ? 2 : 1;
    return false;
}

bool Assembler::parseLine(const std::string& line, std::vector<uint8_t>& output)
{
    // TODO: change line into string_view
    if (line[0] == '*') {
        uint16_t diff = std::atoi(line.c_str() + 2);
        for (uint16_t i = 0; i < diff; ++i)
            output.push_back(0u);
        return true;
    }

    std::string token;

    if (line[0] == '.') {
        token = line.substr(1);
        trimWhiteSpaces(token);
        output.push_back(parseOperand(token));
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
            output.push_back(desc.byte);
            break;
        case InsType::Complex:
            if (token1End == line.npos) {
                return false; // Error: Missing instruction operand
            }
            token2Beg = line.find_first_not_of(" ", token1End);
            token2End = line.find_first_of(" ", token2Beg);
            token = line.substr(token2Beg, token2End - token2Beg + 1);
            output.push_back(desc.byte | parseOperand(token));
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
                output.push_back(desc.byte | (ret != m_symbolTable.end() ? ret->second : parseOperand(token)));
                token = line.substr(token2End + 1);
                ret = m_symbolTable.find(token);
                output.push_back(ret != m_symbolTable.end() ? ret->second : parseOperand(token));
            }
            else {
                // TODO: not sure if this could only be address for here
                auto ret = m_symbolTable.find(token);
                std::uint16_t addr = ret != m_symbolTable.end() ? ret->second : parseOperand(token);
                uint16_t addrHP = addr & 0x0F00u;
                addrHP >>= 8;
                output.push_back(desc.byte | addrHP);
                output.push_back(addr & 0x00FFu);
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
        parseRegister(token, word);
        return word;
    case 'P':
        parseRegisterPair(token, word);
        return word;
    case '$':
        return textToHex(token.substr(1));
    case '0':
        if (token.size() == 1)
            break;

        return textToOct(token.substr(1));
    case '%':
        return textToBin(token.substr(1));
    }

    return textToDec(token);
}

bool Assembler::parseRegister(const std::string_view& str, uint16_t& value)
{
    if (str.size() < 2 || str.size() > 3)
        return false;

    if (str[1] >= '0' && str[1] <= '9') {
        if (str.size() > 2)
            if (str[2] >= '0' && str[2] <= '9')
                value = (str[1] - '0') * 10 + (str[2] - '0');
            else
                return false;
        else 
            value = str[1] - '0';
    }

    if (str[1] >= 'A' && str[1] <= 'F')
        value = str[1] - 'A' + 10;
    else if (str[1] >= 'a' && str[1] <= 'f')
        value = str[1] - 'a' + 10;
    else
        return false;
    
    return true;
}

bool Assembler::parseRegisterPair(const std::string_view& str, uint16_t& value)
{
    if (str.size() != 2)
        return false;

    if (str[1] < '0' || str[1] > '7')
        return false;


    value = 2 * (str[1] - '0');
    return true;
}

bool Assembler::isMnemonic(std::string& token, MnemonicDesc& desc)
{
    if (token.size() > 3) return false;

    for (size_t i = 0; i < token.size(); ++i)
        if (token[i] >= 'a' && token[i] <= 'z')
            token[i] -= 'a' - 'A';

    auto x = m_mnemonics.find(token);
    if (x != m_mnemonics.end()) {
        desc = x->second;
        return true;
    }

    return false;
}
