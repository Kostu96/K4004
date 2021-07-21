#pragma once
#include "shared/source/whitebox.hpp"

#include <cstdint>

class ROM
{
public:
    static constexpr uint16_t PAGE_SIZE = 256u;
    static constexpr uint16_t NUM_ROM_CHIPS = 16u;
    static constexpr uint16_t ROM_SIZE = PAGE_SIZE * NUM_ROM_CHIPS;

    ROM();

    void load(size_t startingAddress, const uint8_t* objectCode, size_t objectCodeLength);
    void reset();
    uint8_t getByte(uint16_t address) const { return m_rom[address]; }

    void setSrcAddress(uint8_t address) { m_srcAddress = address >> 4; }
    uint8_t getSrcAddress() const { return m_srcAddress; }
    const uint8_t* getRomContents() const { return m_rom; }
private:
    uint8_t m_srcAddress;
    uint8_t m_rom[ROM_SIZE];
    uint8_t m_ioPorts[NUM_ROM_CHIPS];

    ALLOW_WHITEBOX(ROM);
};
