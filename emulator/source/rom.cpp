#include "K4004.hpp"
#include "rom.hpp"

namespace mcs4 {

    ROM::ROM()
    {
        reset();
    }

    void ROM::connect(Byte* bus)
    {
        m_bus = bus;
    }

    void ROM::cycle(CycleType currentCycle)
    {
        switch (currentCycle)
        {
        case CycleType::A1:
            m_address = *m_bus & 0x0F;
            break;
        case CycleType::A2:
            m_address |= (*m_bus << 4) & 0xF0;
            break;
        case CycleType::A3:
            m_address = (static_cast<Word>(*m_bus) << 8) & 0x0F00;
            break;
        case CycleType::M1:
            *m_bus = (m_rom[m_address] >> 4) & 0x0F;
            break;
        case CycleType::M2:
            *m_bus = m_rom[m_address] & 0x0F;
            break;
        case CycleType::X1: break;
        case CycleType::X2: break;
        case CycleType::X3: break;
        }
    }

    void ROM::load(Byte startingAddress, Byte* objectCode, Byte objectCodeLength)
    {
        for (Byte i = 0; i < objectCodeLength; ++i)
            m_rom[startingAddress++] = objectCode[i];
    }

    void ROM::reset()
    {
        std::memset(m_rom, 0, 256);
    }

} // namespace mcs4
