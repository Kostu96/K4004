#include "K4004.hpp"
#include "rom.hpp"

ROM::ROM()
{
    reset();
}

void ROM::load(uint8_t startingAddress, const uint8_t* objectCode, uint8_t objectCodeLength)
{
    // TODO: add out of bounds check
    for (uint8_t i = 0; i < objectCodeLength; ++i)
        m_rom[startingAddress++] = objectCode[i];
}

void ROM::reset()
{
    std::memset(m_rom, 0, ROM_SIZE);
    std::memset(m_ioPorts, 0, NUM_ROM_CHIPS);
}
