#pragma once
#include "emulator_core/source/K4004.hpp"
#include "emulator_core/source/ram.hpp"
#include "emulator_core/source/rom.hpp"

class Emulator
{
public:
    Emulator();
    // TODO: add load from binary
    bool loadProgramFromSource(const char* filename);
    bool loadProgramFromObjectCode(const char* filename);
    bool loadProgramFromMemory(const uint8_t* bytecode, size_t codeSize);
    void step(size_t times = 1u);
    void reset(bool resetROM = false);

    const RAM& getRAM() const { return m_ram; }
    const ROM& getROM() const { return m_rom; }
    const K4004& getCPU() const { return m_cpu; }
private:
    RAM m_ram;
    ROM m_rom;
    K4004 m_cpu;
};
