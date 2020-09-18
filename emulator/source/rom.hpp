#pragma once
#include "cycle_types.hpp"
#include "types.hpp"
#include <cstring>

namespace mcs4 {

    /**
     *  \brief Represents 16 x intel4001 roms
     */
    class ROM
    {
    public:
        static constexpr Word ROM_SIZE = 256;

        ROM();

        void connect(Byte* bus);
        void cycle(CycleType currentCycle);
        void load(Byte startingAddress, Byte* objectCode, Byte objectCodeLength);
        void reset();

        const Byte* getRomContents() const { return m_rom; }
    private:
        Word m_address : 12;
        Byte m_rom[ROM_SIZE]; // TODO: Make it proper size - x16
        Byte* m_bus = nullptr;
    };

} // namespace mcs4
