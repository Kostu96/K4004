#include "emulator/source/K4004.hpp"
#include "emulator/source/ram.hpp"
#include "emulator/source/rom.hpp"
#include "emulator/source/instructions.hpp"

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
    case +AsmIns::NOP: NOP(); break;
    case +AsmIns::WRM: WRM(m_ram, m_Acc); break;
    case +AsmIns::WMP: WMP(m_ram, m_Acc); break;
    case +AsmIns::WRR: WRR(m_rom, m_Acc); break;
    case +AsmIns::WR0: WR0(m_ram, m_Acc); break;
    case +AsmIns::WR1: WR1(m_ram, m_Acc); break;
    case +AsmIns::WR2: WR2(m_ram, m_Acc); break;
    case +AsmIns::WR3: WR3(m_ram, m_Acc); break;
    case +AsmIns::SBM: SBM(m_Acc, m_CY, m_ram); break;
    case +AsmIns::RDM: RDM(m_Acc, m_ram); break;
    case +AsmIns::RDR: RDR(m_Acc, m_rom); break;
    case +AsmIns::ADM: ADM(m_Acc, m_CY, m_ram); break;
    case +AsmIns::RD0: RD0(m_Acc, m_ram); break;
    case +AsmIns::RD1: RD1(m_Acc, m_ram); break;
    case +AsmIns::RD2: RD2(m_Acc, m_ram); break;
    case +AsmIns::RD3: RD3(m_Acc, m_ram); break;
    case +AsmIns::CLB: CLB(m_Acc, m_CY); break;
    case +AsmIns::CLC: CLC(m_CY); break;
    case +AsmIns::IAC: IAC(m_Acc, m_CY); break;
    case +AsmIns::CMC: CMC(m_CY); break;
    case +AsmIns::CMA: CMA(m_Acc); break;
    case +AsmIns::RAL: RAL(m_Acc, m_CY); break;
    case +AsmIns::RAR: RAR(m_Acc, m_CY); break;
    case +AsmIns::TCC: TCC(m_Acc, m_CY); break;
    case +AsmIns::DAC: DAC(m_Acc, m_CY); break;
    case +AsmIns::TCS: TCS(m_Acc, m_CY); break;
    case +AsmIns::STC: STC(m_CY); break;
    case +AsmIns::DAA: DAA(m_Acc, m_CY); break;
    case +AsmIns::KBP: KBP(m_Acc); break;
    case +AsmIns::DCL: DCL(m_ram, m_Acc); break;
    case +AsmIns::JCN: JCN(m_PC, m_IR, m_Acc, m_CY, m_test, m_rom); break;
    case +AsmIns::FIM: FIM(m_PC, m_registers, m_IR, m_rom); break;
    case +AsmIns::SRC: SRC(m_ram, m_rom, m_registers, m_IR); break;
    case +AsmIns::FIN: FIN(m_registers, m_PC, m_IR, m_rom); break;
    case +AsmIns::JIN: JIN(m_PC, m_registers, m_IR); break;
    case +AsmIns::JUN: JUN(m_PC, m_IR, m_rom); break;
    case +AsmIns::JMS: JMS(m_PC, m_stack, m_stackDepth, m_IR, m_rom); break;
    case +AsmIns::INC: INC(m_registers, m_IR); break;
    case +AsmIns::ISZ: ISZ(m_PC, m_registers, m_IR, m_rom); break;
    case +AsmIns::ADD: ADD(m_Acc, m_CY, m_registers, m_IR); break;
    case +AsmIns::SUB: SUB(m_Acc, m_CY, m_registers, m_IR); break;
    case +AsmIns::LD:  LD(m_Acc, m_registers, m_IR);  break;
    case +AsmIns::XCH: XCH(m_Acc, m_registers, m_IR); break;
    case +AsmIns::BBL: BBL(m_PC, m_stack, m_stackDepth, m_Acc, m_registers, m_IR); break;
    case +AsmIns::LDM: LDM(m_Acc, m_IR); break;
    }

    if (m_PC > 0x03FFu)
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

