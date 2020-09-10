#include "K4004.hpp"
#include "cycle_types.hpp"
#include <cstring>

namespace mcs4 {

    constexpr Byte INS_NOP = 0x00;
    constexpr Byte INS_WRM = 0xE0;
    constexpr Byte INS_WRP = 0xE1;
    constexpr Byte INS_WRR = 0xE2;
    constexpr Byte INS_WR0 = 0xE4;
    constexpr Byte INS_WR1 = 0xE5;
    constexpr Byte INS_WR2 = 0xE6;
    constexpr Byte INS_WR3 = 0xE7;
    constexpr Byte INS_SBM = 0xE8;
    constexpr Byte INS_RDM = 0xE9;
    constexpr Byte INS_RDR = 0xEA;
    constexpr Byte INS_ADM = 0xEB;
    constexpr Byte INS_RD0 = 0xEC;
    constexpr Byte INS_RD1 = 0xED;
    constexpr Byte INS_RD2 = 0xEE;
    constexpr Byte INS_RD3 = 0xEF;
    constexpr Byte INS_CLB = 0xF0;
    constexpr Byte INS_CLC = 0xF1;
    constexpr Byte INS_IAC = 0xF2;
    constexpr Byte INS_CMC = 0xF3;
    constexpr Byte INS_CMA = 0xF4;
    constexpr Byte INS_RAL = 0xF5;
    constexpr Byte INS_RAR = 0xF6;
    constexpr Byte INS_TCC = 0xF7;
    constexpr Byte INS_DAC = 0xF8;
    constexpr Byte INS_TCS = 0xF9;
    constexpr Byte INS_STC = 0xFA;
    constexpr Byte INS_DAA = 0xFB;
    constexpr Byte INS_KBP = 0xFC;
    constexpr Byte INS_DCL = 0xFD;
    constexpr Byte INS_JCN_MASK = 0x1F;
    constexpr Byte INS_FIM_MASK = 0x2E;
    constexpr Byte INS_SRC_MASK = 0x2F;
    constexpr Byte INS_FIN_MASK = 0x3E;
    constexpr Byte INS_JIN_MASK = 0x3F;
    constexpr Byte INS_JUN_MASK = 0x4F;
    constexpr Byte INS_JMS_MASK = 0x5F;
    constexpr Byte INS_INC_MASK = 0x6F;
    constexpr Byte INS_ISZ_MASK = 0x7F;
    constexpr Byte INS_ADD_MASK = 0x8F;
    constexpr Byte INS_SUB_MASK = 0x9F;
    constexpr Byte INS_LD_MASK = 0xAF;
    constexpr Byte INS_XCH_MASK = 0xBF;
    constexpr Byte INS_BBL_MASK = 0xCF;
    constexpr Byte INS_LDM_MASK = 0xDF;

    K4004::K4004()
    {
        reset();
    }

    void K4004::connect(Byte* bus)
    {
        m_bus = bus;
    }

    void K4004::cycle(CycleType currentCycle)
    {
        Byte tempByte1, tempByte2;
        switch (currentCycle) {
        case CycleType::A1:
            *m_bus = PC & 0x00F;
            break;
        case CycleType::A2:
            *m_bus = (PC >> 4) & 0x00F;
            break;
        case CycleType::A3:
            *m_bus = (PC >> 8) & 0x00F;
            ++PC;
            break;
        case CycleType::M1:
            IR = (*m_bus << 4) & 0xF0;
            break;
        case CycleType::M2:
            IR |= *m_bus & 0x0F;
            break;
        case CycleType::X1:
        {
            Byte opcode = m_is2ByteIns == 0x1 ? m_IRCopyFor2ByteIns : IR;
            switch (opcode) {
            case INS_NOP: break;
            case INS_WRM: break;
            case INS_WRP: break;
            case INS_WRR: break;
            case INS_WR0: break;
            case INS_WR1: break;
            case INS_WR2: break;
            case INS_WR3: break;
            case INS_SBM: break;
            case INS_RDM: break;
            case INS_RDR: break;
            case INS_ADM: break;
            case INS_RD0: break;
            case INS_RD1: break;
            case INS_RD2: break;
            case INS_RD3: break;
            case INS_CLB:
                Acc = 0x0; CY = 0x0;
                break;
            case INS_CLC:
                CY = 0x0;
                break;
            case INS_IAC:
                CY = ++Acc == 0x0 ? 0x1 : 0x0;
                break;
            case INS_CMC:
                ++CY;
                break;
            case INS_CMA:
                Acc = ~Acc;
                break;
            case INS_RAL:
                tempByte1 = CY;
                CY = (Acc & 0x8) >> 4;
                Acc = Acc << 1;
                Acc |= tempByte1;
                break;
            case INS_RAR:
                tempByte1 = CY;
                CY = Acc & 0x1;
                Acc = Acc >> 1;
                Acc |= tempByte1 << 4;
                break;
            case INS_TCC: break;
            case INS_DAC: break;
            case INS_TCS: break;
            case INS_STC: break;
            case INS_DAA: break;
            case INS_KBP: break;
            case INS_DCL: break;
            default:
                if ((opcode & INS_JCN_MASK) == opcode) {
                    if (m_is2ByteIns == 0x0) {
                        m_is2ByteIns = 0x1;
                        m_IRCopyFor2ByteIns = IR;
                    }
                    else {
                        m_is2ByteIns = 0x0;
                    }
                }
                else if ((opcode & INS_FIM_MASK) == opcode) {
                    if (m_is2ByteIns == 0x0) {
                        m_is2ByteIns = 0x1;
                        m_IRCopyFor2ByteIns = IR;
                    }
                    else {
                        m_is2ByteIns = 0x0;
                        tempByte1 = (opcode & 0x0F) >> 1;
                        m_registers[tempByte1] = IR;
                    }
                }
                else if ((opcode & INS_SRC_MASK) == opcode) {

                }
                else if ((opcode & INS_FIN_MASK) == opcode) {

                }
                else if ((opcode & INS_JIN_MASK) == opcode) {

                }
                else if ((opcode & INS_JUN_MASK) == opcode) {
                    if (m_is2ByteIns == 0x0) {
                        m_is2ByteIns = 0x1;
                        m_IRCopyFor2ByteIns = IR;
                    }
                    else {
                        m_is2ByteIns = 0x0;
                        PC = ((opcode & 0x0F) << 8) & 0xF00;
                        PC |= IR;
                    }
                }
                else if ((opcode & INS_JMS_MASK) == opcode) {
                    if (m_is2ByteIns == 0x0) {
                        m_is2ByteIns = 0x1;
                        m_IRCopyFor2ByteIns = IR;
                    }
                    else {
                        m_is2ByteIns = 0x0;
                    }
                }
                else if ((opcode & INS_INC_MASK) == opcode) {

                }
                else if ((opcode & INS_ISZ_MASK) == opcode) {
                    if (m_is2ByteIns == 0x0) {
                        m_is2ByteIns = 0x1;
                        m_IRCopyFor2ByteIns = IR;
                    }
                    else {
                        m_is2ByteIns = 0x0;
                    }
                }
                else if ((opcode & INS_ADD_MASK) == opcode) {
                    tempByte1 = opcode & 0x0F;
                    tempByte1 = m_registers[tempByte1 / 2] >> (tempByte1 % 2 ? 4 : 0);
                    tempByte1 += Acc;
                    Acc = tempByte1 & 0x0F;
                    CY = tempByte1 >> 4;
                }
                else if ((opcode & INS_SUB_MASK) == opcode) {

                }
                else if ((opcode & INS_LD_MASK) == opcode) {
                    tempByte1 = opcode & 0x0F;
                    Acc = m_registers[tempByte1 / 2] >> (tempByte1 % 2 ? 4 : 0);
                }
                else if ((opcode & INS_XCH_MASK) == opcode) {
                    tempByte1 = opcode & 0x0F;
                    tempByte2 = Acc;
                    Acc = m_registers[tempByte1 / 2] >> (tempByte1 % 2 ? 4 : 0);
                    m_registers[tempByte1 / 2] &= (tempByte1 % 2 ? 0x0F : 0xF0);
                    m_registers[tempByte1 / 2] |= (tempByte2 << (tempByte1 % 2 ? 4 : 0)) & (tempByte1 % 2 ? 0xF0 : 0x0F);
                }
                else if ((opcode & INS_BBL_MASK) == opcode) {

                }
                else if ((opcode & INS_LDM_MASK) == opcode) {

                }
            }
            break;
        }
        case CycleType::X2: break;
        case CycleType::X3: break;
        }
    }

    void K4004::reset()
    {
        Acc = 0x0;
        CY = 0x0;
        m_is2ByteIns = 0x0;
        PC = 0x000;
        m_IRCopyFor2ByteIns = IR = 0x00;
        std::memset(m_registers, 0, sizeof m_registers / sizeof m_registers[0]);
        m_stack.reset();
    }

    K4004::Stack::Stack()
    {
        reset();
    }

    void K4004::Stack::reset()
    {
        m_pointer = 0x0;
        m_stack[0] = m_stack[1] = m_stack[2] = 0x000;
    }

    void K4004::Stack::push(Word address)
    {
        m_stack[m_pointer++] = address;
    }

    Word K4004::Stack::pull()
    {
        return m_stack[--m_pointer];
    }

} // namespace mcs4
