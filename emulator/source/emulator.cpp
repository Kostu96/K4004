#pragma once
#include "emulator/source/emulator.hpp"
#include "emulator/source/cycle_types.hpp"
#include "emulator/source/rom.hpp"

#include "assembler/source/assembler.hpp"

Emulator::Emulator() :
    m_currentCycle(CycleType::A1)
{
    m_rom.connect(&m_bus);
    m_cpu.connect(&m_bus);
}

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
