#pragma once
#include "emulator/source/emulator.hpp"
#include "emulator/source/rom.hpp"

#include "assembler/source/assembler.hpp"

Emulator::Emulator() :
    m_ram(),
    m_rom(),
    m_cpu(m_rom, m_ram) {}

bool Emulator::loadProgram(const char* filename)
{
    Assembler assembler;
    std::uint8_t* bytecode;
    std::size_t codeSize;
    bool ret = assembler.assemble(filename, bytecode, codeSize);

    if (ret) {
        m_rom.load(0x00, bytecode, codeSize);
        assembler.freeOutput(bytecode);
    }

    return ret;
}

void Emulator::loadProgram(const uint8_t* bytecode, size_t codeSize)
{
    m_rom.load(0x00, bytecode, codeSize);
}

void Emulator::step(size_t times)
{
    while (times--) {
        m_cpu.step();
    }
}

void Emulator::reset(bool resetROM)
{
    m_cpu.reset();
    m_ram.reset();

    if (resetROM)
        m_rom.reset();
}
