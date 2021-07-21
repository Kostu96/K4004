#include "assembler/source/assembler.hpp"

#include "shared/source/assembly.hpp"

#include <fstream>
#include <sstream>
#include <iomanip>
#include <bitset>

Assembler::Assembler() :
    m_address(0u),
    m_metalMaskLength(0u),
    m_mnemonics({ // TODO: Make it constexpr
        { "ADM", { +AsmIns::ADM, InsType::Simple } },
        { "CLB", { +AsmIns::CLB, InsType::Simple } },
        { "CLC", { +AsmIns::CLC, InsType::Simple } },
        { "CMA", { +AsmIns::CMA, InsType::Simple } },
        { "CMC", { +AsmIns::CMC, InsType::Simple } },
        { "DAA", { +AsmIns::DAA, InsType::Simple } },
        { "DAC", { +AsmIns::DAC, InsType::Simple } },
        { "DCL", { +AsmIns::DCL, InsType::Simple } },
        { "IAC", { +AsmIns::IAC, InsType::Simple } },
        { "KBP", { +AsmIns::KBP, InsType::Simple } },
        { "NOP", { +AsmIns::NOP, InsType::Simple } },
        { "RAL", { +AsmIns::RAL, InsType::Simple } },
        { "RAR", { +AsmIns::RAR, InsType::Simple } },
        { "RD0", { +AsmIns::RD0, InsType::Simple } },
        { "RD1", { +AsmIns::RD1, InsType::Simple } },
        { "RD2", { +AsmIns::RD2, InsType::Simple } },
        { "RD3", { +AsmIns::RD3, InsType::Simple } },
        { "RDM", { +AsmIns::RDM, InsType::Simple } },
        { "RDR", { +AsmIns::RDR, InsType::Simple } },
        { "SBM", { +AsmIns::SBM, InsType::Simple } },
        { "STC", { +AsmIns::STC, InsType::Simple } },
        { "TCC", { +AsmIns::TCC, InsType::Simple } },
        { "TCS", { +AsmIns::TCS, InsType::Simple } },
        { "WMP", { +AsmIns::WMP, InsType::Simple } },
        { "WR0", { +AsmIns::WR0, InsType::Simple } },
        { "WR1", { +AsmIns::WR1, InsType::Simple } },
        { "WR2", { +AsmIns::WR2, InsType::Simple } },
        { "WR3", { +AsmIns::WR3, InsType::Simple } },
        { "WRM", { +AsmIns::WRM, InsType::Simple } },
        { "WRR", { +AsmIns::WRR, InsType::Simple } },
        { "ADD", { +AsmIns::ADD, InsType::Complex } },
        { "BBL", { +AsmIns::BBL, InsType::Complex } },
        { "FIN", { +AsmIns::FIN, InsType::Complex } },
        { "INC", { +AsmIns::INC, InsType::Complex } },
        { "JIN", { +AsmIns::JIN, InsType::Complex } },
        { "LD",  { +AsmIns::LD,  InsType::Complex } },
        { "LDM", { +AsmIns::LDM, InsType::Complex } },
        { "SRC", { +AsmIns::SRC, InsType::Complex } },
        { "SUB", { +AsmIns::SUB, InsType::Complex } },
        { "XCH", { +AsmIns::XCH, InsType::Complex } },
        { "ISZ", { +AsmIns::ISZ, InsType::TwoByte } },
        { "JCN", { +AsmIns::JCN, InsType::TwoByte } },
        { "JMS", { +AsmIns::JMS, InsType::TwoByte } },
        { "JUN", { +AsmIns::JUN, InsType::TwoByte } },
        { "FIM", { +AsmIns::FIM, InsType::TwoByte } }
    }) {}

bool Assembler::assemble(const char* filename, std::vector<uint8_t>& output)
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

void Assembler::disassemble(const std::vector<uint8_t>& bytecode, std::vector<std::string>& output)
{
    output.reserve(bytecode.size());
    std::stringstream ss;
    ss << std::setfill('0') << std::hex << std::uppercase;
    bool twoByte = false;
    for (size_t i = 0; i < bytecode.size(); ++i) {
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
            m_symbolTable.insert(std::make_pair<>(token, m_address));
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
        parseHexNumber(token, word);
        return word;
    case '0':
        if (token.size() == 1)
            break;

        parseOctNumber(token, word);
        return word;
    case '%':
        parseBinNumber(token, word);
        return word;
    }

    parseDecNumber(token, word);
    return word;
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

bool Assembler::parseHexNumber(const std::string_view& str, uint16_t& value)
{
    size_t length = str.size();
    if (length == 5 || length == 0)
        return false; // String representation of a to big number or empty

    value = 0;
    for (size_t i = 1; i < length; ++i) {
        if (str[i] >= '0' && str[i] <= '9')
            value += (1 << 4 * (length - i - 1)) * (str[i] - '0');
        else if (str[i] >= 'a' && str[i] <= 'f')
            value += (1 << 4 * (length - i - 1)) * (str[i] - 'a' + 10);
        else if (str[i] >= 'A' && str[i] <= 'F')
            value += (1 << 4 * (length - i - 1)) * (str[i] - 'A' + 10);
        else
            return false; // Illformed bin number
    }

    return true;
}

bool Assembler::parseBinNumber(const std::string_view& str, uint16_t& value)
{
    size_t length = str.size();
    if (length == 18 || length == 0)
        return false; // String representation of a to big number or empty

    value = 0;
    for (size_t i = 1; i < length; ++i) {
        if (str[i] == '1')
            value += 1 << (length - i - 1);
        else if (str[i] != '0')
            return false; // Illformed bin number
    }

    return true;
}

bool Assembler::parseOctNumber(const std::string_view& str, uint16_t& value)
{
    size_t length = str.size();
    if (length == 8 || length == 0)
        return false; // String representation of a to big number or empty

    value = 0;
    for (size_t i = 0; i < length; ++i) {
        if (str[i] >= '0' && str[i] <= '7')
            value += (1 << 3 * (length - i - 1)) * (str[i] - '0');
        else
            return false; // Illformed bin number
    }

    return true;
}

bool Assembler::parseDecNumber(const std::string_view& str, uint16_t& value)
{
    size_t length = str.size();
    if (length == 6 || length == 0)
        return false;

    value = 0;
    uint16_t base = 1;
    for (size_t i = length; i > 0; --i) {
        if (str[i - 1] >= '0' && str[i - 1] <= '9') {
            value += base * (str[i - 1] - '0');
            base *= 10;
        }
        else
            return false; // Illformed dec number
    }

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
