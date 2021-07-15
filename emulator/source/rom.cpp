#include "emulator/source/rom.hpp"

#include <cstring>

ROM::ROM()
{
    reset();
}

void ROM::load(size_t startingAddress, const uint8_t* objectCode, size_t objectCodeLength)
{
    // TODO: add out of bounds check
    for (size_t i = 0; i < objectCodeLength; ++i)
        m_rom[startingAddress++] = objectCode[i];
}

void ROM::reset()
{
    m_srcAddress = 0u;
    std::memset(m_rom, 0, ROM_SIZE);
    std::memset(m_ioPorts, 0, NUM_ROM_CHIPS);
}
