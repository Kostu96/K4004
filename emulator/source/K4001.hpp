#pragma once
#include "cycle_types.hpp"
#include "types.hpp"
#include <cstring>

namespace mcs4 {

    class K4001
    {
    public:
        static constexpr Word ROM_SIZE = 256;

        K4001(Byte metalMaskedNumber) : m_metalMaskedNumber(metalMaskedNumber) { reset(); }

        void connect(Byte* bus) {
            m_bus = bus;
        }

        void cycle(CycleType currentCycle) {
            switch (currentCycle)
            {
            case CycleType::A1:
                m_address = *m_bus & 0x0F;
                break;
            case CycleType::A2:
                m_address |= (*m_bus << 4) & 0xF0;
                break;
            case CycleType::A3:
                m_romSelect = *m_bus & 0x0F;
                break;
            case CycleType::M1:
                if (m_romSelect == m_metalMaskedNumber)
                    *m_bus = (m_rom[m_address] >> 4) & 0x0F;
                break;
            case CycleType::M2:
                if (m_romSelect == m_metalMaskedNumber)
                    *m_bus = m_rom[m_address] & 0x0F;
                break;
            case CycleType::X1: break;
            case CycleType::X2: break;
            case CycleType::X3: break;
            }
        }

        void load(Byte startingAddress, Byte* objectCode, Byte objectCodeLength) {
            for (Byte i = 0; i < objectCodeLength; ++i)
                m_rom[startingAddress++] = objectCode[i];
        }

        void reset() {
            std::memset(m_rom, 0, 256);
        }

        const Byte* getRomContents() const { return m_rom; }
    private:
        Byte m_metalMaskedNumber : 4;
        Byte m_romSelect : 4;
        Byte m_address = 0x00;
        Byte m_rom[ROM_SIZE];
        Byte* m_bus = nullptr;
    };

} // namespace mcs4
