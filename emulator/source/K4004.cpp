#include "emulator/source/K4004.hpp"
#include "emulator/source/ram.hpp"
#include "emulator/source/rom.hpp"

#include "shared/source/assembly.hpp"

#include <cstring>

K4004::K4004(ROM& rom, RAM& ram) :
    m_rom(rom),
    m_ram(ram)
{
    reset();
}

void K4004::step()
{
    m_IR = m_rom.getByte(m_PC++);
    uint8_t opcode = getOpcodeFromByte(m_IR);
    switch (opcode) {
    case ASM_NOP: break;
    case ASM_WRM: WRM(); break;
    case ASM_WMP: WMP(); break;
    case ASM_WRR: WRR(); break;
    case ASM_WR0: WR0(); break;
    case ASM_WR1: WR1(); break;
    case ASM_WR2: WR2(); break;
    case ASM_WR3: WR3(); break;
    case ASM_SBM: SBM(); break;
    case ASM_RDM: RDM(); break;
    case ASM_RDR: RDR(); break;
    case ASM_ADM: ADM(); break;
    case ASM_RD0: RD0(); break;
    case ASM_RD1: RD1(); break;
    case ASM_RD2: RD2(); break;
    case ASM_RD3: RD3(); break;
    case ASM_CLB: CLB(); break;
    case ASM_CLC: CLC(); break;
    case ASM_IAC: IAC(); break;
    case ASM_CMC: CMC(); break;
    case ASM_CMA: CMA(); break;
    case ASM_RAL: RAL(); break;
    case ASM_RAR: RAR(); break;
    case ASM_TCC: TCC(); break;
    case ASM_DAC: DAC(); break;
    case ASM_TCS: TCS(); break;
    case ASM_STC: STC(); break;
    case ASM_DAA: DAA(); break;
    case ASM_KBP: KBP(); break;
    case ASM_DCL: DCL(); break;
    case ASM_JCN_MASK: JCN(); break;
    case ASM_FIM_MASK: FIM(); break;
    case ASM_SRC_MASK: SRC(); break;
    case ASM_FIN_MASK: FIN(); break;
    case ASM_JIN_MASK: JIN(); break;
    case ASM_JUN_MASK: JUN(); break;
    case ASM_JMS_MASK: JMS(); break;
    case ASM_INC_MASK: INC(); break;
    case ASM_ISZ_MASK: ISZ(); break;
    case ASM_ADD_MASK: ADD(); break;
    case ASM_SUB_MASK: SUB(); break;
    case ASM_LD_MASK:  LD();  break;
    case ASM_XCH_MASK: XCH(); break;
    case ASM_BBL_MASK: BBL(); break;
    case ASM_LDM_MASK: LDM(); break;
    }

    if (m_PC > BITMASK16_12BITS)
        m_PC = 0u;
}

void K4004::reset()
{
    m_Acc = 0u;
    m_CY = 0u;
    std::memset(m_registers, 0, sizeof(m_registers) / sizeof(m_registers[0]));
    m_PC = m_stack[0] = m_stack[1] = m_stack[2] = 0u;
    m_stackDepth = 0u;
    m_CM_RAM = 0u;
    m_test = 0u;
}

void K4004::pushStack(uint16_t address)
{
    m_stack[2] = m_stack[1] & BITMASK16_12BITS;
    m_stack[1] = m_stack[0] & BITMASK16_12BITS;
    m_stack[0] = m_PC & BITMASK16_12BITS;
    m_PC = address & BITMASK16_12BITS;
    if (++m_stackDepth > 3)
        m_stackDepth = 3u;
}

void K4004::pullStack()
{
    if (m_stackDepth > 0) {
        --m_stackDepth;
        m_PC = m_stack[0] & BITMASK16_12BITS;
        m_stack[0] = m_stack[1] & BITMASK16_12BITS;
        m_stack[1] = m_stack[2] & BITMASK16_12BITS;
        m_stack[2] = 0u; // TODO: Could be useful to return that.
    }
}

uint8_t K4004::getRegisterValue(uint8_t reg)
{
    bool isOdd = reg % 2;
    uint8_t regPairValue = m_registers[reg / 2];
    return (regPairValue >> (isOdd ? 0u : 4u)) & BITMASK8_4BITS;
}

void K4004::setRegisterValue(uint8_t reg, uint8_t value)
{
    bool isOdd = reg % 2;
    m_registers[reg / 2] &= (isOdd ? BITMASK8_4BITS_Q1 : BITMASK8_4BITS);
    m_registers[reg / 2] |= (value << (isOdd ? 0u : 4u)) & (isOdd ? BITMASK8_4BITS : BITMASK8_4BITS_Q1);
}

void K4004::WRM()
{
    m_ram.writeRAM(m_Acc);
}

void K4004::WMP()
{
    m_ram.writeOutputPort(m_Acc);
}

void K4004::WRR()
{
}

void K4004::WR0()
{
    m_ram.writeStatus(m_Acc, 0u);
}

void K4004::WR1()
{
    m_ram.writeStatus(m_Acc, 1u);
}

void K4004::WR2()
{
    m_ram.writeStatus(m_Acc, 2u);
}

void K4004::WR3()
{
    m_ram.writeStatus(m_Acc, 3u);
}

void K4004::SBM()
{
    m_CY = m_CY == 0u ? 1u : 0u;
    uint8_t temp = (~m_ram.readRAM() & BITMASK8_4BITS) + m_CY;
    temp += m_Acc;
    m_Acc = temp & BITMASK8_4BITS;
    m_CY = (temp >> 4) & BITMASK8_1BIT;
}

void K4004::RDM()
{
    m_Acc = m_ram.readRAM() & BITMASK8_4BITS;
}

void K4004::RDR()
{
}

void K4004::ADM()
{
    uint8_t temp = (m_ram.readRAM() & BITMASK8_4BITS) + m_CY;
    temp += m_Acc;
    m_Acc = temp & BITMASK8_4BITS;
    m_CY = (temp >> 4) & BITMASK8_1BIT;
}

void K4004::RD0()
{
    m_Acc = m_ram.readStatus(0u) & BITMASK8_4BITS;
}

void K4004::RD1()
{
    m_Acc = m_ram.readStatus(1u) & BITMASK8_4BITS;
}

void K4004::RD2()
{
    m_Acc = m_ram.readStatus(2u) & BITMASK8_4BITS;
}

void K4004::RD3()
{
    m_Acc = m_ram.readStatus(3u) & BITMASK8_4BITS;
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
    ++m_Acc;
    m_Acc &= BITMASK8_4BITS;
    m_CY = m_Acc == 0u ? 1u : 0u;
}

void K4004::CMC()
{
    m_CY = m_CY == 0u ? 1u : 0u;
}

void K4004::CMA()
{
    m_Acc = ~m_Acc & BITMASK8_4BITS;
}

void K4004::RAL()
{
    uint8_t tempByte1 = m_CY;
    m_CY = (m_Acc >> 3) & BITMASK8_1BIT;
    m_Acc = (m_Acc << 1) & BITMASK8_4BITS;
    m_Acc |= tempByte1;
}

void K4004::RAR()
{
    uint8_t tempByte1 = m_CY;
    m_CY = m_Acc & BITMASK8_1BIT;
    m_Acc = (m_Acc >> 1) & BITMASK8_4BITS;
    m_Acc |= tempByte1 << 3;
}

void K4004::TCC()
{
    m_Acc = m_CY;
    m_CY = 0u;
}

void K4004::DAC()
{
    --m_Acc;
    m_Acc &= BITMASK8_4BITS;
    m_CY = m_Acc == BITMASK8_4BITS ? 0u : 1u;
}

void K4004::TCS()
{
    m_Acc = m_CY == 0 ? 9u : 10u;
    m_CY = 0u;
}

void K4004::STC()
{
    m_CY = 1u;
}

void K4004::DAA()
{
    if (m_CY == 1u || m_Acc > 9u) {
        uint8_t temp = m_Acc + 6u;
        m_Acc = temp & BITMASK8_4BITS;
        if ((temp >> 4) & BITMASK8_1BIT)
            m_CY = 1u;
    }
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
    uint8_t temp = m_Acc & BITMASK8_3BIT;
    switch (temp) {
    case 0b000:
        m_ram.setRAMBank(0u);
        break;
    case 0b001:
        m_ram.setRAMBank(1u);
        break;
    case 0b010:
        m_ram.setRAMBank(2u);
        break;
    case 0b100:
        m_ram.setRAMBank(3u);
        break;
    }
}

void K4004::JCN()
{
    uint8_t con = m_IR & BITMASK8_4BITS;
    uint8_t address = m_rom.getByte(m_PC++);
    
    bool shouldJump = false;
    switch (con) {
    case ASM_CON_AEZ: shouldJump = m_Acc == 0u; break;
    case ASM_CON_ANZ: shouldJump = m_Acc != 0u; break;
    case ASM_CON_CEZ: shouldJump = m_CY == 0u; break;
    case ASM_CON_CNZ: shouldJump = m_CY != 0u; break;
    case ASM_CON_TEZ: shouldJump = m_test == 0u; break;
    case ASM_CON_TNZ: shouldJump = m_test != 0u; break;
    }

    if (shouldJump) {
        if ((m_PC & BITMASK16_8BITS) == 0xFE) m_PC += 2u;
        m_PC &= BITMASK16_4BITS_Q2;
        m_PC |= address;
    }
}

void K4004::FIM()
{
    uint8_t reg = (m_IR & BITMASK8_4BITS) >> 1;
    m_registers[reg] = m_rom.getByte(m_PC++);
}

void K4004::SRC()
{
    uint8_t reg = (m_IR & BITMASK8_4BITS) >> 1;
    uint8_t addr = m_registers[reg];
    m_rom.setSrcAddress(addr);
    m_ram.setSrcAddress(addr);
}

void K4004::FIN()
{
    uint8_t reg = (m_IR & BITMASK8_4BITS) >> 1;
    uint8_t addr = m_registers[0];
    if ((m_PC & BITMASK16_8BITS) == 0xFF) addr += ROM::PAGE_SIZE;
    m_registers[reg] = m_rom.getByte(addr);
}

void K4004::JIN()
{
}

void K4004::JUN()
{
    uint16_t address = ((m_IR & BITMASK8_4BITS) << 8) & BITMASK16_4BITS_Q2;
    m_PC = address | m_rom.getByte(m_PC++);
}

void K4004::JMS()
{
    uint16_t address = ((m_IR & BITMASK8_4BITS) << 8) & BITMASK16_4BITS_Q2;
    address |= m_rom.getByte(m_PC++);
    pushStack(address);
}

void K4004::INC()
{
    uint8_t reg = m_IR & BITMASK8_4BITS;
    uint8_t temp = getRegisterValue(reg);
    setRegisterValue(reg, temp + 1);
}

void K4004::ISZ()
{
}

void K4004::ADD()
{
    uint8_t temp = m_IR & BITMASK8_4BITS;
    temp = getRegisterValue(temp) + m_CY;
    temp += m_Acc;
    m_Acc = temp & BITMASK8_4BITS;
    m_CY = temp >> 4;
}

void K4004::SUB()
{
    m_CY = m_CY == 0u ? 1u : 0u;
    uint8_t temp = m_IR & BITMASK8_4BITS;
    temp = (~getRegisterValue(temp) & BITMASK8_4BITS) + m_CY;
    temp += m_Acc;
    m_Acc = temp & BITMASK8_4BITS;
    m_CY = (temp >> 4) & BITMASK8_1BIT;
}

void K4004::LD()
{
    uint8_t reg = m_IR & BITMASK8_4BITS;
    m_Acc = getRegisterValue(reg);
}

void K4004::XCH()
{
    uint8_t reg = m_IR & BITMASK8_4BITS;
    uint8_t temp = m_Acc;
    m_Acc = getRegisterValue(reg);
    setRegisterValue(reg, temp);
}

void K4004::BBL()
{
    pullStack();
    uint8_t reg = m_IR & BITMASK8_4BITS;
    m_Acc = getRegisterValue(reg);
}

void K4004::LDM()
{
    m_Acc = m_IR & BITMASK8_4BITS;
}
