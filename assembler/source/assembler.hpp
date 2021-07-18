#pragma once
#include "shared/source/whitebox.hpp"

#include <cstdint>
#include <string>
#include <unordered_map>

class Assembler
{
public:
    Assembler();
    bool assemble(const char* filename, std::uint8_t*& output, size_t& outputSize);
    void freeOutput(uint8_t* output) { delete[] output; }
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
    bool parseLine(const std::string& line, uint8_t* output, uint16_t& outputIndex);
    uint16_t parseOperand(const std::string& token);
    bool parseRegister(const std::string_view& str, uint16_t& value);
    bool parseRegisterPair(const std::string_view& str, uint16_t& value);
    bool parseHexNumber(const std::string_view& str, uint16_t& value);
    bool parseBinNumber(const std::string_view& str, uint16_t& value);
    bool parseOctNumber(const std::string_view& str, uint16_t& value);
    bool parseDecNumber(const std::string_view& str, uint16_t& value);
    bool isMnemonic(std::string& token, MnemonicDesc& desc);

    uint16_t m_address;
    std::unordered_map<std::string, uint16_t> m_symbolTable;
    std::unordered_map<std::string, MnemonicDesc> m_mnemonics;

    ALLOW_WHITEBOX(Assembler);
};
