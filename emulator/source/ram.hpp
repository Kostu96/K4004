#pragma once
#include "shared/source/whitebox.hpp"

#include <cstdint>

class RAM
{
public:
    static constexpr uint16_t NUM_RAM_BANKS = 4u;
    static constexpr uint16_t NUM_RAM_CHIPS = 4u;
    static constexpr uint16_t NUM_RAM_REGS = 4u;
    static constexpr uint16_t NUM_REG_CHARS = 16u;
    static constexpr uint16_t NUM_STAUS_CHARS = 4u;
    static constexpr uint16_t RAM_SIZE = NUM_RAM_BANKS * NUM_RAM_CHIPS * NUM_RAM_REGS * NUM_REG_CHARS;
    static constexpr uint16_t STATUS_SIZE = NUM_RAM_BANKS * NUM_RAM_CHIPS * NUM_RAM_REGS * NUM_STAUS_CHARS;
    static constexpr uint16_t OUTPUT_SIZE = NUM_RAM_BANKS * NUM_RAM_CHIPS;

    RAM();

    void reset();
    void writeRAM(uint8_t character);
    uint8_t readRAM() const { return m_ram[m_srcAddress] & 0x0F; }
    void writeStatus(uint8_t character, uint8_t index);
    uint8_t readStatus(uint8_t index) const;
    void writeOutputPort(uint8_t character);
    uint8_t readOutputPort() const;

    void setRAMBank(uint8_t index);
    void setSrcAddress(uint8_t address);
    uint16_t getSrcAddress() const { return m_srcAddress & 0x3FFu; }
    const uint8_t* getRamContents() const { return m_ram; }
    const uint8_t* getStatusContents() const { return m_status; }
    const uint8_t* getOutputContents() const { return m_oPorts; }
    
    RAM(const RAM&) = delete;
    RAM& operator=(const RAM&) = delete;
private:
    uint16_t m_srcAddress;
    uint8_t m_ram[RAM_SIZE];
    uint8_t m_status[STATUS_SIZE];
    uint8_t m_oPorts[OUTPUT_SIZE];

    ALLOW_WHITEBOX(RAM);
};
