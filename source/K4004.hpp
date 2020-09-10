#pragma once
#include "types.hpp"

namespace mcs4 {

    enum class CycleType : Byte;

    class K4004
    {
    private:
        class Stack
        {
        public:
            static constexpr Byte STACK_SIZE = 3;

            Stack();
            void reset();
            void push(Word address);
            Word pull();

            const Word* getStackContents() const { return m_stack; }
            Byte getSP() const { return m_pointer; }
        private:
            Byte m_pointer : 2;
            Word m_stack[STACK_SIZE];
        };
    public:
        static constexpr Byte REGISTERS_SIZE = 8;

        K4004();

        void connect(Byte* bus);
        void cycle(CycleType currentCycle);
        void reset();

        Byte getAcc() const { return Acc; }
        Byte getCY() const { return CY; }
        Word getPC() const { return PC; }
        Byte getIR() const { return IR; }
        const Stack& getStack() const { return m_stack; }
        const Byte* getRegisters() const { return m_registers; }
    private:
        Byte Acc : 4;
        Byte CY : 1;
        Byte m_is2ByteIns : 1;
        Byte m_IRCopyFor2ByteIns;
        Word PC : 12;
        Byte IR;
        Byte m_registers[REGISTERS_SIZE];
        Stack m_stack;
        Byte* m_bus;
    };

} // namespace mcs4
