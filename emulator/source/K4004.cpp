#include "emulator/source/K4004.hpp"
#include "emulator/source/rom.hpp"

#include <cstring>

// TODO: Make it shared with assembler and tests
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

K4004::K4004(ROM& rom) :
    m_rom(rom)
{
    reset();
}

void K4004::step()
{
    m_IR = m_rom.getByte(m_PC++);
    uint8_t opcode;
    if (m_IR == 0x00u || m_IR > 0xDF)
        opcode = m_IR;
    else
        opcode = m_IR | 0x0F;

    switch (opcode) {
    case INS_NOP: break;
    case INS_WRM: WRM(); break;
    case INS_WMP: WMP(); break;
    case INS_WRR: WRR(); break;
    case INS_WR0: WR0(); break;
    case INS_WR1: WR1(); break;
    case INS_WR2: WR2(); break;
    case INS_WR3: WR3(); break;
    case INS_SBM: SBM(); break;
    case INS_RDM: RDM(); break;
    case INS_RDR: RDR(); break;
    case INS_ADM: ADM(); break;
    case INS_RD0: RD0(); break;
    case INS_RD1: RD1(); break;
    case INS_RD2: RD2(); break;
    case INS_RD3: RD3(); break;
    case INS_CLB: CLB(); break;
    case INS_CLC: CLC(); break;
    case INS_IAC: IAC(); break;
    case INS_CMC: CMC(); break;
    case INS_CMA: CMA(); break;
    case INS_RAL: RAL(); break;
    case INS_RAR: RAR(); break;
    case INS_TCC: TCC(); break;
    case INS_DAC: DAC(); break;
    case INS_TCS: TCS(); break;
    case INS_STC: STC(); break;
    case INS_DAA: DAA(); break;
    case INS_KBP: KBP(); break;
    case INS_DCL: DCL(); break;
    case INS_JCN_MASK: JCN(); break;
    case INS_FIM_MASK: FIM(); break;
    case INS_SRC_MASK: SRC(); break;
    case INS_FIN_MASK: FIN(); break;
    case INS_JIN_MASK: JIN(); break;
    case INS_JUN_MASK: JUN(); break;
    case INS_JMS_MASK: JMS(); break;
    case INS_INC_MASK: INC(); break;
    case INS_ISZ_MASK: ISZ(); break;
    case INS_ADD_MASK: ADD(); break;
    case INS_SUB_MASK: SUB(); break;
    case INS_LD_MASK:  LD();  break;
    case INS_XCH_MASK: XCH(); break;
    case INS_BBL_MASK: BBL(); break;
    case INS_LDM_MASK: LDM(); break;
    }
}

void K4004::reset()
{
    m_Acc = 0u;
    m_CY = 0u;
    std::memset(m_registers, 0, sizeof(m_registers) / sizeof(m_registers[0]));
    m_PC = m_stack[0] = m_stack[1] = m_stack[2] = 0u;
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
    m_stack[2] = 0u; // TODO: Could be useful to return that.
}

uint8_t K4004::getRegisterValue(uint8_t reg)
{
    uint8_t regPairValue = m_registers[reg / 2];
    return regPairValue >> (reg % 2 ? 0u : 4u);
}

void K4004::setRegisterValue(uint8_t reg, uint8_t value)
{
    bool isOdd = reg % 2;
    m_registers[reg / 2] &= (isOdd ? 0xF0 : 0x0F);
    m_registers[reg / 2] |= (value << (isOdd ? 0 : 4)) & (isOdd ? 0x0F : 0xF0);
}

void K4004::WRM()
{

}

void K4004::WMP()
{
}

void K4004::WRR()
{
}

void K4004::WR0()
{
}

void K4004::WR1()
{
}

void K4004::WR2()
{
}

void K4004::WR3()
{
}

void K4004::SBM()
{
}

void K4004::RDM()
{
}

void K4004::RDR()
{
}

void K4004::ADM()
{
}

void K4004::RD0()
{
}

void K4004::RD1()
{
}

void K4004::RD2()
{
}

void K4004::RD3()
{
}

void K4004::CLB()
{
    m_Acc = 0u;
    m_CY = 0u;
}

void K4004::CLC()
{
    m_CY = 0u;
}

void K4004::IAC()
{
    m_CY = ++m_Acc == 0u ? 1u : 0u;
}

void K4004::CMC()
{
    m_CY = m_CY == 0u ? 1u : 0u;
}

void K4004::CMA()
{
    m_Acc = ~m_Acc & 0x0F;
}

void K4004::RAL()
{
    uint8_t tempByte1 = m_CY;
    m_CY = (m_Acc & 0x8) >> 4;
    m_Acc = m_Acc << 1;
    m_Acc |= tempByte1;
}

void K4004::RAR()
{
    uint8_t tempByte1 = m_CY;
    m_CY = m_Acc & 0x1;
    m_Acc = m_Acc >> 1;
    m_Acc |= tempByte1 << 4;
}

void K4004::TCC()
{
}

void K4004::DAC()
{
}

void K4004::TCS()
{
}

void K4004::STC()
{
}

void K4004::DAA()
{
}

void K4004::KBP()
{
    if (m_Acc == 0b0000u) return;
    if (m_Acc == 0b0001u) return;
    if (m_Acc == 0b0010u) return;
    if (m_Acc == 0b0100u) { m_Acc = 0b0011u; return; }
    if (m_Acc == 0b1000u) { m_Acc = 0b0100u; return; }
    m_Acc = 0b1111u;
}

void K4004::DCL()
{
}

void K4004::JCN()
{
    // TODO: unfinished
    uint8_t address = m_rom.getByte(m_PC++);
}

void K4004::FIM()
{
    uint8_t reg = (m_IR & 0x0F) >> 1;
    m_registers[reg] = m_rom.getByte(m_PC++);
}

void K4004::SRC()
{
}

void K4004::FIN()
{
}

void K4004::JIN()
{
}

void K4004::JUN()
{
    m_PC = ((m_IR & 0x0F) << 8) & 0xF00;
    m_PC |= m_rom.getByte(m_PC++);
}

void K4004::JMS()
{
    uint16_t address = ((m_IR & 0x0F) << 8) & 0xF00;
    address |= m_rom.getByte(m_PC++);
    pushStack(address);
}

void K4004::INC()
{
}

void K4004::ISZ()
{
}

void K4004::ADD()
{
    uint8_t temp = m_IR & 0x0F;
    temp = getRegisterValue(temp) + m_CY;
    temp += m_Acc;
    m_Acc = temp & 0x0F;
    m_CY = temp >> 4;
}

void K4004::SUB()
{
    CMC();
    uint8_t temp = m_IR & 0x0F;
    temp = (~getRegisterValue(temp) & 0x0F) + m_CY;
    temp += m_Acc;
    m_Acc = temp & 0x0F;
    m_CY = (temp >> 4) & 1u;
}

void K4004::LD()
{
    uint8_t reg = m_IR & 0x0F;
    m_Acc = getRegisterValue(reg);
}

void K4004::XCH()
{
    uint8_t reg = m_IR & 0x0F;
    uint8_t temp = m_Acc;
    m_Acc = getRegisterValue(reg);
    setRegisterValue(reg, temp);
}

void K4004::BBL()
{
}

void K4004::LDM()
{
    m_Acc = m_IR & 0x0F;
}
