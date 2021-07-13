#pragma once
#include "emulator/source/cycle_types.hpp"

#include <cstring>

class ROM
{
public:
    static constexpr uint16_t ROM_SIZE = 256;

    ROM();

    void connect(uint8_t* bus);
    void cycle(CycleType currentCycle);
    void load(uint8_t startingAddress, const uint8_t* objectCode, uint8_t objectCodeLength);
    void reset();

    const uint8_t* getRomContents() const { return m_rom; }
private:
    uint16_t m_address : 12;
    uint8_t m_rom[ROM_SIZE]; // TODO: Make it proper size - x16
    uint8_t* m_bus = nullptr;
};
