#pragma once
#include <cinttypes>
#include <string>
#include <unordered_map>

class Assembler
{
public:
    static bool assemble(const char* filename, std::uint8_t* output, size_t* outputSize);
private:
    enum class InsType {
        Simple,
        Complex,
        TwoByte
    };

    struct MnemonicDesc {
        InsType type;
        std::uint8_t byte;
    };

    static void parseLine(std::string& line);
    static std::uint8_t parseOperand(const std::string& token);
    static bool isMnemonic(const std::string& token, MnemonicDesc& desc);

    static std::unordered_map<std::string, std::uint16_t> m_symbolTable;
    static std::uint16_t m_address;
};
