#pragma once
#include "emulator/source/K4004.hpp"
#include "emulator/source/ram.hpp"
#include "emulator/source/rom.hpp"

#include "shared/source/whitebox.hpp"

class Emulator
{
public:
    Emulator();
    // TODO: add load from binary and change names accordingly
    bool loadProgram(const char* filename);
    void loadProgram(const uint8_t* bytecode, size_t codeSize);
    void step(size_t times = 1u);
    void reset();

    const ROM& getROM() const { return m_rom; }
    const K4004& getCPU() const { return m_cpu; }
private:
    RAM m_ram;
    ROM m_rom;
    K4004 m_cpu;

    ALLOW_WHITEBOX(Emulator);
};
