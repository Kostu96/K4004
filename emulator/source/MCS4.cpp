#pragma once
#include "rom.hpp"
#include "cycle_types.hpp"
#include "MCS4.hpp"

namespace mcs4 {

    MCS4::MCS4() :
        m_currentCycle(CycleType::A1)
    {
        m_rom.connect(&m_bus);
        m_cpu.connect(&m_bus);
    }

    bool MCS4::loadProgram(const char* filename)
    {
        return false;
    }

} // namespace mcs4
