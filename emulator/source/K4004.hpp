#pragma once
#include "shared/source/assembly.hpp"
#include "shared/source/whitebox.hpp"

#include <cstdint>

class ROM;
class RAM;

class K4004
{
public:
    K4004(ROM& rom, RAM& ram);

    void step();
    void reset();

    uint8_t getAcc() const { return m_Acc; }
    uint8_t getCY() const { return m_CY; }
    uint8_t getIR() const { return m_IR; }
    const uint16_t* getStack() const { return m_stack; }
    uint16_t getPC() const { return m_PC; }
    const uint8_t* getRegisters() const { return m_registers; }
    void setTest(uint8_t test) { m_test = test & 1u; }
    uint8_t getTest() const { return m_test; }
private:
    ROM& m_rom;
    RAM& m_ram;
    uint8_t m_registers[8];
    uint16_t m_stack[3];
    uint8_t m_stackDepth;
    uint16_t m_PC;
    uint8_t m_IR;
    uint8_t m_Acc;
    uint8_t m_CY;
    uint8_t m_CM_RAM;
    uint8_t m_test;

    ALLOW_WHITEBOX(K4004);
};
