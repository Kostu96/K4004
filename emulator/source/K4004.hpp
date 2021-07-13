#pragma once
#include "shared/source/whitebox.hpp"

#include <cstdint>

static constexpr uint16_t BITMASK_12BITS = 0x0FFF;
static constexpr uint8_t BITMASK_4BITS = 0x0F;
static constexpr uint8_t BITMASK_1BIT = 0x01;

enum class CycleType : uint8_t;

class K4004
{
public:
    K4004();

    void connect(uint8_t* bus);
    void cycle(CycleType currentCycle);
    void reset();
    void pushStack(uint16_t address);
    void pullStack();

    uint8_t getAcc() const { return m_Acc; }
    uint8_t getCY() const { return m_CY; }
    uint8_t getIR() const { return m_IR; }
    const uint16_t* getStack() const { return m_stack; }
    uint16_t getPC() const { return m_PC; }
    const uint8_t* getRegisters() const { return m_registers; }
private:
    static constexpr uint8_t REGISTERS_SIZE = 8; // Must always be 8.
    uint8_t m_registers[REGISTERS_SIZE];
    uint16_t m_stack[3];
    uint16_t m_PC;
    uint8_t* m_bus = nullptr;
    uint8_t m_IR;
    uint8_t m_IRCopyFor2ByteIns;
    uint8_t m_Acc;
    uint8_t m_CY;
    uint8_t m_is2ByteIns;

    ALLOW_WHITEBOX(K4004);
};
