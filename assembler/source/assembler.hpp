#pragma once
#include <cinttypes>
#include <string>
#include <unordered_map>

class Assembler
{
public:
    static bool assemble(const char* filename, std::uint8_t*& output, size_t& outputSize);
    static void freeOutput(std::uint8_t* output) { delete[] output; }
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

    static bool trimComments(std::string& line);
    static bool trimWhiteSpaces(std::string& line);
    static bool checkForSymbols(std::string& line);
    static void parseLine(std::string& line);
    static std::uint8_t parseOperand(const std::string& token);
    static bool isMnemonic(const std::string& token, MnemonicDesc& desc);

    static std::uint16_t m_address;
    static std::unordered_map<std::string, std::uint16_t> m_symbolTable;
    static std::unordered_map<std::string, MnemonicDesc> m_mnemonics;
};
