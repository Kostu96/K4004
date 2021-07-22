#include "emulator/source/instructions.hpp"
#include "emulator/source/ram.hpp"
#include "emulator/source/rom.hpp"

#include "shared/source/assembly.hpp"

uint8_t getRegisterValue(const uint8_t* registers, uint8_t reg)
{
    bool isOdd = reg % 2;
    uint8_t regPairValue = registers[reg / 2];
    return (regPairValue >> (isOdd ? 0u : 4u)) & 0x0Fu;
}

void setRegisterValue(uint8_t* registers, uint8_t reg, uint8_t value)
{
    bool isOdd = reg % 2;
    registers[reg / 2] &= (isOdd ? 0xF0u : 0x0Fu);
    registers[reg / 2] |= (value << (isOdd ? 0u : 4u)) & (isOdd ? 0x0Fu : 0xF0u);
}

void NOP()
{
}

void JCN(uint16_t& PC, uint8_t IR, uint8_t acc, uint8_t CY, uint8_t test, const ROM& rom)
{
    uint8_t con = IR & 0x0Fu;
    uint8_t address = rom.getByte(PC++);

    bool shouldJump = false;
    switch (con) {
    case +AsmCon::AEZ: shouldJump = acc == 0u; break;
    case +AsmCon::ANZ: shouldJump = acc != 0u; break;
    case +AsmCon::CEZ: shouldJump = CY == 0u; break;
    case +AsmCon::CNZ: shouldJump = CY != 0u; break;
    case +AsmCon::TEZ: shouldJump = test == 0u; break;
    case +AsmCon::TNZ: shouldJump = test != 0u; break;
    }

    if (shouldJump) {
        if ((PC & 0x00FFu) == 0xFE) PC += 2u;
        PC &= 0x0F00u;
        PC |= address;
    }
}

void FIM(uint16_t& PC, uint8_t* registers, uint8_t IR, const ROM& rom)
{
    uint8_t reg = (IR & 0x0Fu) >> 1;
    registers[reg] = rom.getByte(PC++);
}

void SRC(RAM& ram, ROM& rom, const uint8_t* registers, uint8_t IR)
{
    uint8_t reg = (IR & 0x0Fu) >> 1;
    uint8_t addr = registers[reg];
    rom.setSrcAddress(addr);
    ram.setSrcAddress(addr);
}

void FIN(uint8_t* registers, uint16_t PC, uint8_t IR, const ROM& rom)
{
    uint8_t reg = (IR & 0x0Fu) >> 1;
    uint8_t addr = registers[0];
    if ((PC & 0x00FFu) == 0xFF) addr += ROM::PAGE_SIZE;
    registers[reg] = rom.getByte(addr);
}

void JIN(uint16_t& PC, const uint8_t* registers, uint8_t IR)
{
    uint8_t reg = (IR & 0x0Fu) >> 1;
    uint8_t addr = registers[reg];
    if ((PC & 0x00FFu) == 0xFFu) ++PC;
    PC &= 0x0F00u;
    PC |= addr;
}

void JUN(uint16_t& PC, uint8_t IR, const ROM& rom)
{
    uint16_t address = (IR & 0x0Fu) << 8;
    PC = address | rom.getByte(PC++);
}

void JMS(uint16_t& PC, uint16_t* stack, uint8_t& stackDepth, uint8_t IR, const ROM& rom)
{
    uint16_t address = (IR & 0x0Fu) << 8;
    address |= rom.getByte(PC++);

    stack[2] = stack[1];
    stack[1] = stack[0];
    stack[0] = PC;
    PC = address & 0x03FFu;
    if (++stackDepth > 3)
        stackDepth = 3u;
}

void INC(uint8_t* registers, uint8_t IR)
{
    uint8_t reg = IR & 0x0Fu;
    uint8_t temp = getRegisterValue(registers, reg);
    setRegisterValue(registers, reg, temp + 1);
}

void ISZ(uint16_t& PC, uint8_t* registers, uint8_t IR, const ROM& rom)
{
    uint8_t reg = IR & 0x0Fu;
    uint8_t value = getRegisterValue(registers, reg);
    setRegisterValue(registers, reg, value + 1);
    uint8_t addr = rom.getByte(PC++);
    if (((value + 1) & 0x0Fu) != 0u) {
        if ((PC & 0x00FFu) == 0xFEu) PC += 2u;
        PC &= 0x0F00u;
        PC |= addr;
    }
}

void ADD(uint8_t& acc, uint8_t& CY, const uint8_t* registers, uint8_t IR)
{
    uint8_t temp = IR & 0x0Fu;
    temp = getRegisterValue(registers, temp) + CY;
    temp += acc;
    acc = temp & 0x0Fu;
    CY = (temp >> 4) & 1u;
}

void SUB(uint8_t& acc, uint8_t& CY, const uint8_t* registers, uint8_t IR)
{
    CY = CY == 0u ? 1u : 0u;
    uint8_t temp = IR & 0x0Fu;
    temp = (~getRegisterValue(registers, temp) & 0x0Fu) + CY;
    temp += acc;
    acc = temp & 0x0Fu;
    CY = (temp >> 4) & 1u;
}

void LD(uint8_t& acc, const uint8_t* registers, uint8_t IR)
{
    uint8_t reg = IR & 0x0Fu;
    acc = getRegisterValue(registers, reg);
}

void XCH(uint8_t& acc, uint8_t* registers, uint8_t IR)
{
    uint8_t reg = IR & 0x0Fu;
    uint8_t temp = acc;
    acc = getRegisterValue(registers, reg);
    setRegisterValue(registers, reg, temp);
}

void BBL(uint16_t& PC, uint16_t* stack, uint8_t& stackDepth, uint8_t& acc, const uint8_t* registers, uint8_t IR)
{
    if (stackDepth > 0) {
        --stackDepth;
        PC = stack[0];
        stack[0] = stack[1];
        stack[1] = stack[2];
        stack[2] = 0u;
    }

    uint8_t reg = IR & 0x0Fu;
    acc = getRegisterValue(registers, reg);
}

void LDM(uint8_t& acc, uint8_t IR)
{
    acc = IR & 0x0Fu;
}

void WRM(RAM& ram, uint8_t acc)
{
    ram.writeRAM(acc);
}

void WMP(RAM& ram, uint8_t acc)
{
    ram.writeOutputPort(acc);
}

void WRR(ROM& rom, uint8_t acc)
{
    rom.setIOPort(acc);
}

void WR0(RAM& ram, uint8_t acc)
{
    ram.writeStatus(acc, 0u);
}

void WR1(RAM& ram, uint8_t acc)
{
    ram.writeStatus(acc, 1u);
}

void WR2(RAM& ram, uint8_t acc)
{
    ram.writeStatus(acc, 2u);
}

void WR3(RAM& ram, uint8_t acc)
{
    ram.writeStatus(acc, 3u);
}

void SBM(uint8_t& acc, uint8_t& CY, const RAM& ram)
{
    CY = CY == 0u ? 1u : 0u;
    uint8_t temp = (~ram.readRAM() & 0x0Fu) + CY;
    temp += acc;
    acc = temp & 0x0Fu;
    CY = (temp >> 4) & 1u;
}

void RDM(uint8_t& acc, const RAM& ram)
{
    acc = ram.readRAM() & 0x0Fu;
}

void RDR(uint8_t& acc, const ROM& rom)
{
    acc = rom.getIOPort() & 0x0Fu;
}

void ADM(uint8_t& acc, uint8_t& CY, const RAM& ram)
{
    uint8_t temp = (ram.readRAM() & 0x0Fu) + CY;
    temp += acc;
    acc = temp & 0x0Fu;
    CY = (temp >> 4) & 1u;
}

void RD0(uint8_t& acc, const RAM& ram)
{
    acc = ram.readStatus(0u) & 0x0Fu;
}

void RD1(uint8_t& acc, const RAM& ram)
{
    acc = ram.readStatus(1u) & 0x0Fu;
}

void RD2(uint8_t& acc, const RAM& ram)
{
    acc = ram.readStatus(2u) & 0x0Fu;
}

void RD3(uint8_t& acc, const RAM& ram)
{
    acc = ram.readStatus(3u) & 0x0Fu;
}

void CLB(uint8_t& acc, uint8_t& CY)
{
    acc = 0u;
    CY = 0u;
}

void CLC(uint8_t& CY)
{
    CY = 0u;
}

void IAC(uint8_t& acc, uint8_t& CY)
{
    ++acc;
    acc &= 0x0Fu;
    CY = acc == 0u ? 1u : 0u;
}

void CMC(uint8_t& CY)
{
    CY = CY == 0u ? 1u : 0u;
}

void CMA(uint8_t& acc)
{
    acc = ~acc & 0x0Fu;
}

void RAL(uint8_t& acc, uint8_t& CY)
{
    uint8_t tempByte1 = CY;
    CY = (acc >> 3) & 1u;
    acc = (acc << 1) & 0x0Fu;
    acc |= tempByte1;
}

void RAR(uint8_t& acc, uint8_t& CY)
{
    uint8_t tempByte1 = CY;
    CY = acc & 1u;
    acc = (acc >> 1) & 0x0Fu;
    acc |= tempByte1 << 3;
}

void TCC(uint8_t& acc, uint8_t& CY)
{
    acc = CY;
    CY = 0u;
}

void DAC(uint8_t& acc, uint8_t& CY)
{
    --acc;
    acc &= 0x0Fu;
    CY = acc == 0x0Fu ? 0u : 1u;
}

void TCS(uint8_t& acc, uint8_t& CY)
{
    acc = CY == 0 ? 9u : 10u;
    CY = 0u;
}

void STC(uint8_t& CY)
{
    CY = 1u;
}

void DAA(uint8_t& acc, uint8_t& CY)
{
    if (CY == 1u || acc > 9u) {
        uint8_t temp = acc + 6u;
        acc = temp & 0x0Fu;
        if ((temp >> 4) & 1u)
            CY = 1u;
    }
}

void KBP(uint8_t& acc)
{
    if (acc == 0b0000u) return;
    if (acc == 0b0001u) return;
    if (acc == 0b0010u) return;
    if (acc == 0b0100u) { acc = 0b0011u; return; }
    if (acc == 0b1000u) { acc = 0b0100u; return; }
    acc = 0b1111u;
}

void DCL(RAM& ram, uint8_t acc)
{
    uint8_t temp = acc & 0x07u;
    switch (temp) {
    case 0b000:
        ram.setRAMBank(0u);
        break;
    case 0b001:
        ram.setRAMBank(1u);
        break;
    case 0b010:
        ram.setRAMBank(2u);
        break;
    case 0b100:
        ram.setRAMBank(3u);
        break;
    }
}
