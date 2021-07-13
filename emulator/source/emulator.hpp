#pragma once
#include "emulator/source/K4004.hpp"
#include "emulator/source/rom.hpp"

#include "shared/source/whitebox.hpp"

enum class CycleType : uint8_t;

class Emulator
{
public:
    Emulator();
    bool loadProgram(const char* filename);
    void loadProgram(const uint8_t* bytecode, std::size_t codeSize);
    void step(size_t times = 1u);

    const uint8_t* getROMData() const { return m_rom.getRomContents(); }
    constexpr static uint16_t getROMSize() { return ROM::ROM_SIZE; }
    const K4004& getCPU() const { return m_cpu; }
private:
    CycleType m_currentCycle;
    uint8_t m_bus = 0x0;
    ROM m_rom;
    K4004 m_cpu;

    ALLOW_WHITEBOX(Emulator);
};
