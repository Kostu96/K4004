#pragma once
#include "K4004.hpp"
#include "rom.hpp"

namespace mcs4 {

    enum class CycleType : Byte;

    /**
     *  \brief Where the magic happens...
     */
    class MCS4
    {
    public:
        MCS4();
        bool loadProgram(const char* filename);

        const Byte* getROMData() const { return m_rom.getRomContents(); }
        Word getROMSize() const { return m_rom.ROM_SIZE; }
    private:
        CycleType m_currentCycle;
        Byte m_bus = 0x0;
        ROM m_rom{};
        K4004 m_cpu{};
    };

} // namespace mcs4
