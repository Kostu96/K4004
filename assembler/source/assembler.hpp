#pragma once
#include "assembler/source/global.hpp"

#include <cinttypes>
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
        InsType type = InsType::Invalid;
        uint8_t byte = 0;
    };

    bool trimComments(std::string& line);
    bool trimWhiteSpaces(std::string& line);
    bool checkForSymbols(std::string& line);
    bool parseLine(const std::string& line, uint8_t* output, uint16_t& outputIndex);
    uint16_t parseOperand(const std::string& token);
    bool parseRegisterOperand(const std::string& token, uint16_t& value);
    bool parseRegisterPairOperand(const std::string& token, uint16_t& value);
    bool parseHexNumberOperand(const std::string& token, uint16_t& value);
    bool parseBinNumberOperand(const std::string& token, uint16_t& value);
    bool parseOctNumberOperand(const std::string& token, uint16_t& value);
    bool isMnemonic(const std::string& token, MnemonicDesc& desc);

    uint16_t m_address;
    std::unordered_map<std::string, uint16_t> m_symbolTable;
    std::unordered_map<std::string, MnemonicDesc> m_mnemonics;

    ALLOW_WHITEBOX(Assembler);
};
