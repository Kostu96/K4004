#include "emulator_core/source/emulator.hpp"
#include "assembler/source/assembler.hpp"

#include <fstream>

Emulator::Emulator() :
    m_ram(),
    m_rom(),
    m_cpu(m_rom, m_ram) {}

bool Emulator::loadProgramFromSource(const char* filename)
{
    Assembler assembler;
    std::vector<uint8_t> bytecode;
    bool ret = assembler.assemble(filename, bytecode);

    if (ret) {
        ret = m_rom.load(bytecode.data(), bytecode.size());
    }

    return ret;
}

bool Emulator::loadProgramFromObjectCode(const char* filename)
{
    std::fstream file(filename);
    if (!file.is_open())
        return false;

    std::vector<uint8_t> bytecode;
    uint16_t byte;
    file >> std::hex;
    while (file >> byte)
        bytecode.push_back(byte);

    file.close();

    return m_rom.load(bytecode.data(), bytecode.size());
}

bool Emulator::loadProgramFromMemory(const uint8_t* bytecode, size_t codeSize)
{
    return m_rom.load(bytecode, codeSize);
}

void Emulator::step(size_t times)
{
    while (times--) {
        m_cpu.clock();
    }
}

void Emulator::reset(bool resetROM)
{
    m_cpu.reset();
    m_ram.reset();

    if (resetROM)
        m_rom.reset();
}
