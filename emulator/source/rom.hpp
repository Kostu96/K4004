#pragma once
#include <cstring>

class ROM
{
public:
    static constexpr uint16_t ROM_SIZE = 256;

    ROM();

    void connect(uint8_t* bus);
    void load(uint8_t startingAddress, const uint8_t* objectCode, uint8_t objectCodeLength);
    void reset();
    uint8_t getByte(uint16_t address) const { return m_rom[address]; }

    const uint8_t* getRomContents() const { return m_rom; }
private:
    uint8_t m_rom[ROM_SIZE]; // TODO: Make it proper size - x16
};
