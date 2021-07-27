#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

class Assembler
{
public:
    Assembler();
    bool assemble(const char* filename, std::vector<uint8_t>& output, bool i4004ModeEnabled = false);
    bool disassemble(const std::vector<uint8_t>& bytecode, std::vector<std::string>& output);
private:
    enum class InsType {
        Invalid,
        Simple,
        Complex,
        TwoByte
    };

    struct MnemonicDesc {
        uint8_t byte = 0;
        InsType type = InsType::Invalid;
    };

    bool trimComments(std::string& line);
    bool trimWhiteSpaces(std::string& line);
    bool checkForSymbols(std::string& line);
    bool parseLine(const std::string& line, std::vector<uint8_t>& output);
    uint16_t parseOperand(const std::string& token);
    bool parseRegister(const std::string_view& str, uint16_t& value);
    bool parseRegisterPair(const std::string_view& str, uint16_t& value);
    bool parseHexNumber(const std::string_view& str, uint16_t& value);
    bool parseBinNumber(const std::string_view& str, uint16_t& value);
    bool parseOctNumber(const std::string_view& str, uint16_t& value);
    bool parseDecNumber(const std::string_view& str, uint16_t& value);
    bool isMnemonic(std::string& token, MnemonicDesc& desc);

    size_t m_address;
    size_t m_metalMaskLength;
    std::unordered_map<std::string, uint16_t> m_symbolTable;
    std::unordered_map<std::string, MnemonicDesc> m_mnemonics;
};
