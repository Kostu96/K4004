#pragma once
#include <cinttypes>
#include <string>
#include <unordered_map>

class Assembler
{
public:
    Assembler();
    bool assemble(const char* filename, std::uint8_t*& output, size_t& outputSize);
    void freeOutput(std::uint8_t* output) { delete[] output; }
private:
    enum class InsType {
        Invalid,
        Simple,
        Complex,
        TwoByte
    };

    struct MnemonicDesc {
        InsType type = InsType::Invalid;
        std::uint8_t byte = 0;
    };

    bool trimComments(std::string& line);
    bool trimWhiteSpaces(std::string& line);
    bool checkForSymbols(std::string& line);
    void parseLine(std::string& line);
    std::uint8_t parseOperand(const std::string& token);
    bool isMnemonic(const std::string& token, MnemonicDesc& desc);

    std::uint16_t m_address;
    std::unordered_map<std::string, std::uint16_t> m_symbolTable;
    std::unordered_map<std::string, MnemonicDesc> m_mnemonics;
};
