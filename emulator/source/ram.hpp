#pragma once
#include "shared/source/whitebox.hpp"

#include <cstdint>

class RAM
{
public:
    static constexpr uint16_t NUM_RAM_CHIPS = 16u;
    static constexpr uint16_t NUM_RAM_REGS = 4u;
    static constexpr uint16_t NUM_REG_CHARS = 16u;
    static constexpr uint16_t RAM_SIZE = NUM_RAM_CHIPS * NUM_RAM_REGS * NUM_REG_CHARS;

    RAM();

    void reset();

    void setSrcAddress(uint8_t address) { m_srcAddress = address; }
    uint8_t getSrcAddress() const { return m_srcAddress; }
private:
    uint8_t m_srcAddress;
    uint8_t m_ram[RAM_SIZE];

    ALLOW_WHITEBOX(RAM);
};
