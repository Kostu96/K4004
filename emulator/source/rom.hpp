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

    bool load(const uint8_t* objectCode, size_t objectCodeLength);
    void reset();

    uint8_t readByte(uint16_t address) const { return m_rom[address]; }
    void writeIOPort(uint8_t value);
    uint8_t readIOPort() const;
    
    void setSrcAddress(uint8_t address) { m_srcAddress = address >> 4; }
    uint8_t getSrcAddress() const { return m_srcAddress; }
    const uint8_t* getRomContents() const { return m_rom; }
    uint8_t getIOPort(uint8_t idx) const { return m_ioPorts[idx]; }
private:
    uint8_t m_srcAddress;
    uint8_t m_rom[ROM_SIZE];
    uint8_t m_ioPorts[NUM_ROM_CHIPS];
    uint8_t m_ioPortsMasks[NUM_ROM_CHIPS];

    ALLOW_WHITEBOX(ROM);
};
