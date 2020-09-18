#pragma once
#include "MCS4.hpp"

#include "assembler.hpp"
#include "cycle_types.hpp"
#include "rom.hpp"

namespace mcs4 {

    MCS4::MCS4() :
        m_currentCycle(CycleType::A1)
    {
        m_rom.connect(&m_bus);
        m_cpu.connect(&m_bus);
    }

    bool MCS4::loadProgram(const char* filename)
    {
        std::uint8_t* bytecode;
        std::size_t codeSize;
        bool ret = Assembler::assemble(filename, bytecode, codeSize);

        if (ret) {
            m_rom.load(0x00, bytecode, codeSize);
            Assembler::freeOutput(bytecode);
        }

        return ret;
    }

} // namespace mcs4
