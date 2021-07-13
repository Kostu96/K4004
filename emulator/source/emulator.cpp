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

void Emulator::loadProgram(const uint8_t* bytecode, std::size_t codeSize)
{
    m_rom.load(0x00, bytecode, codeSize);
}

void Emulator::step(size_t times)
{
    size_t value = 8 * times;
    while (value--) {
        switch (m_currentCycle)
        {
        case CycleType::A1:
        case CycleType::A2:
        case CycleType::A3:
            m_cpu.cycle(m_currentCycle);
            m_rom.cycle(m_currentCycle);
            break;
        case CycleType::M1:
        case CycleType::M2:
            m_rom.cycle(m_currentCycle);
            m_cpu.cycle(m_currentCycle);
            break;
        case CycleType::X1:
        case CycleType::X2:
        case CycleType::X3:
            m_cpu.cycle(m_currentCycle);
            break;
        }
        ++m_currentCycle;
    }
}
