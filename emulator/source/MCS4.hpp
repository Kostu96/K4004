#pragma once
#include "emulator/source/K4004.hpp"
#include "emulator/source/rom.hpp"

enum class CycleType : Byte;

class MCS4
{
public:
    MCS4();
    bool loadProgram(const char* filename);

    const Byte* getROMData() const { return m_rom.getRomContents(); }
    constexpr static Word getROMSize() { return ROM::ROM_SIZE; }
private:
    CycleType m_currentCycle;
    Byte m_bus = 0x0;
    ROM m_rom{};
    K4004 m_cpu{};
};
