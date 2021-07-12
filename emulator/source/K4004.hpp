#pragma once
#include <cstdint>

enum class CycleType : uint8_t;

class K4004
{
private:
    class Stack
    {
    public:
        Stack();
        void reset();
        void push(uint16_t address);
        void pull();

        const uint16_t* getStackContents() const { return m_stack; }
        uint16_t getPC() const { return m_PC; }
    private:
        static constexpr uint16_t ADDRESS_BITMASK = 0x0FFF;
        union {
            struct {
                uint16_t m_level1;
                uint16_t m_level2;
                uint16_t m_level3;
            };
            uint16_t m_stack[3];
        };
        uint16_t m_PC : 12;
    };
public:
    K4004();

    void connect(uint8_t* bus);
    void cycle(CycleType currentCycle);
    void reset();

    uint8_t getAcc() const { return m_Acc; }
    uint8_t getCY() const { return m_CY; }
    uint8_t getIR() const { return m_IR; }
    const Stack& getStack() const { return m_stack; }
    const uint8_t* getRegisters() const { return m_registers; }
private:
    static constexpr uint8_t REGISTERS_SIZE = 8; // Must always be 8.
    uint8_t m_registers[REGISTERS_SIZE];
    uint8_t* m_bus = nullptr;
    Stack m_stack;
    uint8_t m_IR;
    uint8_t m_IRCopyFor2ByteIns;
    uint8_t m_Acc : 4;
    uint8_t m_CY : 1;
    uint8_t m_is2ByteIns : 1;
};
