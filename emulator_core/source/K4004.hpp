#pragma once
#include <cstdint>

class ROM;
class RAM;

class K4004
{
public:
    static constexpr uint8_t REGISTERS_SIZE = 8u;
    static constexpr uint8_t STACK_SIZE = 4u;

    K4004(ROM& rom, RAM& ram);

    void reset();
    uint8_t clock();

    const uint16_t* getStack() const { return m_stack; }
    const uint8_t* getRegisters() const { return m_registers; }
    uint16_t getPC() const { return m_stack[m_SP]; }
    uint8_t getIR() const { return m_IR; }
    uint8_t getACC() const { return m_ACC; }
    uint8_t getCY() const { return m_ACC >> 4; }
    uint8_t getTest() const { return m_test; }
    void setTest(uint8_t test) { m_test = test & 1u; }
private:
    void incPC() { m_stack[m_SP] = ++m_stack[m_SP] & 0x03FFu; }

    uint8_t m_registers[REGISTERS_SIZE];
    uint16_t m_stack[STACK_SIZE];
    uint8_t m_SP;
    uint8_t m_IR;
    uint8_t m_ACC;
    uint8_t m_test;
    ROM& m_rom;
    RAM& m_ram;

    uint8_t m_CM_RAM;
};
