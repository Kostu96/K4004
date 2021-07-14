#pragma once
#include "emulator/source/K4004.hpp"
#include "emulator/source/rom.hpp"

#include "shared/source/whitebox.hpp"

class Emulator
{
public:
    Emulator();
    bool loadProgram(const char* filename);
    void loadProgram(const uint8_t* bytecode, std::size_t codeSize);
    void step(size_t times = 1u);
    void reset();

    const uint8_t* getROMData() const { return m_rom.getRomContents(); }
    constexpr static uint16_t getROMSize() { return ROM::ROM_SIZE; }
    const K4004& getCPU() const { return m_cpu; }
private:
    ROM m_rom;
    K4004 m_cpu;

    ALLOW_WHITEBOX(Emulator);
};
