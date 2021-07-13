#include "K4004.hpp"
#include "cycle_types.hpp"
#include <cstring>

constexpr uint8_t INS_NOP = 0x00;
constexpr uint8_t INS_WRM = 0xE0;
constexpr uint8_t INS_WMP = 0xE1;
constexpr uint8_t INS_WRR = 0xE2;
constexpr uint8_t INS_WR0 = 0xE4;
constexpr uint8_t INS_WR1 = 0xE5;
constexpr uint8_t INS_WR2 = 0xE6;
constexpr uint8_t INS_WR3 = 0xE7;
constexpr uint8_t INS_SBM = 0xE8;
constexpr uint8_t INS_RDM = 0xE9;
constexpr uint8_t INS_RDR = 0xEA;
constexpr uint8_t INS_ADM = 0xEB;
constexpr uint8_t INS_RD0 = 0xEC;
constexpr uint8_t INS_RD1 = 0xED;
constexpr uint8_t INS_RD2 = 0xEE;
constexpr uint8_t INS_RD3 = 0xEF;
constexpr uint8_t INS_CLB = 0xF0;
constexpr uint8_t INS_CLC = 0xF1;
constexpr uint8_t INS_IAC = 0xF2;
constexpr uint8_t INS_CMC = 0xF3;
constexpr uint8_t INS_CMA = 0xF4;
constexpr uint8_t INS_RAL = 0xF5;
constexpr uint8_t INS_RAR = 0xF6;
constexpr uint8_t INS_TCC = 0xF7;
constexpr uint8_t INS_DAC = 0xF8;
constexpr uint8_t INS_TCS = 0xF9;
constexpr uint8_t INS_STC = 0xFA;
constexpr uint8_t INS_DAA = 0xFB;
constexpr uint8_t INS_KBP = 0xFC;
constexpr uint8_t INS_DCL = 0xFD;
constexpr uint8_t INS_JCN_MASK = 0x1F;
constexpr uint8_t INS_FIM_MASK = 0x2E;
constexpr uint8_t INS_SRC_MASK = 0x2F;
constexpr uint8_t INS_FIN_MASK = 0x3E;
constexpr uint8_t INS_JIN_MASK = 0x3F;
constexpr uint8_t INS_JUN_MASK = 0x4F;
constexpr uint8_t INS_JMS_MASK = 0x5F;
constexpr uint8_t INS_INC_MASK = 0x6F;
constexpr uint8_t INS_ISZ_MASK = 0x7F;
constexpr uint8_t INS_ADD_MASK = 0x8F;
constexpr uint8_t INS_SUB_MASK = 0x9F;
constexpr uint8_t INS_LD_MASK = 0xAF;
constexpr uint8_t INS_XCH_MASK = 0xBF;
constexpr uint8_t INS_BBL_MASK = 0xCF;
constexpr uint8_t INS_LDM_MASK = 0xDF;

K4004::K4004()
{
    reset();
}

void K4004::connect(uint8_t* bus)
{
    m_bus = bus;
}

void K4004::cycle(CycleType currentCycle)
{
    uint8_t tempByte1, tempByte2;
    switch (currentCycle) {
    case CycleType::A1:
        *m_bus = m_PC & 0x00F;
        break;
    case CycleType::A2:
        *m_bus = (m_PC >> 4) & 0x00F;
        break;
    case CycleType::A3:
        *m_bus = (m_PC >> 8) & 0x00F;
        ++m_PC;
        break;
    case CycleType::M1:
        m_IR = (*m_bus << 4) & 0xF0;
        break;
    case CycleType::M2:
        m_IR |= *m_bus & 0x0F;
        break;
    case CycleType::X1:
    {
        uint8_t opcode = m_is2ByteIns == 0x1 ? m_IRCopyFor2ByteIns : m_IR;
        switch (opcode) {
        case INS_NOP: break;
        case INS_WRM: break;
        case INS_WMP: break;
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
            m_Acc = 0x0; m_CY = 0x0;
            break;
        case INS_CLC:
            m_CY = 0x0;
            break;
        case INS_IAC:
            m_CY = ++m_Acc == 0x0 ? 0x1 : 0x0;
            break;
        case INS_CMC:
            ++m_CY;
            break;
        case INS_CMA:
            m_Acc = ~m_Acc;
            break;
        case INS_RAL:
            tempByte1 = m_CY;
            m_CY = (m_Acc & 0x8) >> 4;
            m_Acc = m_Acc << 1;
            m_Acc |= tempByte1;
            break;
        case INS_RAR:
            tempByte1 = m_CY;
            m_CY = m_Acc & 0x1;
            m_Acc = m_Acc >> 1;
            m_Acc |= tempByte1 << 4;
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
                    m_IRCopyFor2ByteIns = m_IR;
                }
                else {
                    m_is2ByteIns = 0x0;
                }
            }
            else if ((opcode & INS_FIM_MASK) == opcode) {
                if (m_is2ByteIns == 0x0) {
                    m_is2ByteIns = 0x1;
                    m_IRCopyFor2ByteIns = m_IR;
                }
                else {
                    m_is2ByteIns = 0x0;
                    tempByte1 = (opcode & 0x0F) >> 1;
                    m_registers[tempByte1] = m_IR;
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
                    m_IRCopyFor2ByteIns = m_IR;
                }
                else {
                    m_is2ByteIns = 0x0;
                    m_PC = ((opcode & 0x0F) << 8) & 0xF00;
                    m_PC |= m_IR;
                }
            }
            else if ((opcode & INS_JMS_MASK) == opcode) {
                if (m_is2ByteIns == 0x0) {
                    m_is2ByteIns = 0x1;
                    m_IRCopyFor2ByteIns = m_IR;
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
                    m_IRCopyFor2ByteIns = m_IR;
                }
                else {
                    m_is2ByteIns = 0x0;
                }
            }
            else if ((opcode & INS_ADD_MASK) == opcode) {
                tempByte1 = opcode & 0x0F;
                tempByte1 = m_registers[tempByte1 / 2] >> (tempByte1 % 2 ? 4 : 0);
                tempByte1 += m_Acc;
                m_Acc = tempByte1 & 0x0F;
                m_CY = tempByte1 >> 4;
            }
            else if ((opcode & INS_SUB_MASK) == opcode) {

            }
            else if ((opcode & INS_LD_MASK) == opcode) {
                tempByte1 = opcode & 0x0F;
                m_Acc = m_registers[tempByte1 / 2] >> (tempByte1 % 2 ? 4 : 0);
            }
            else if ((opcode & INS_XCH_MASK) == opcode) {
                tempByte1 = opcode & 0x0F;
                tempByte2 = m_Acc;
                m_Acc = m_registers[tempByte1 / 2] >> (tempByte1 % 2 ? 4 : 0);
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
    m_Acc = 0x0;
    m_CY = 0x0;
    m_is2ByteIns = 0x0;
    m_IRCopyFor2ByteIns = m_IR = 0x00;
    std::memset(m_registers, 0, sizeof m_registers / sizeof m_registers[0]);
    m_PC = m_stack[0] = m_stack[1] = m_stack[2] = 0x000;
}

void K4004::pushStack(uint16_t address)
{
    m_stack[2] = m_stack[1] & BITMASK_12BITS;
    m_stack[1] = m_stack[0] & BITMASK_12BITS;
    m_stack[0] = m_PC & BITMASK_12BITS;
    m_PC = address & BITMASK_12BITS;
}

void K4004::pullStack()
{
    m_PC = m_stack[0] & BITMASK_12BITS;
    m_stack[0] = m_stack[1] & BITMASK_12BITS;
    m_stack[1] = m_stack[2] & BITMASK_12BITS;
    m_stack[2] = 0x0; // TODO: Could be useful to return that.
}
