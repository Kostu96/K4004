#include "emulator_core/source/K4040.hpp"
#include "emulator_core/source/instructions.hpp"
#include "emulator_core/source/ram.hpp"
#include "emulator_core/source/rom.hpp"

#include "shared/source/assembly.hpp"

#include <cstring>

K4040::K4040(ROM& rom, RAM& ram) :
    m_rom(rom),
    m_ram(ram)
{
    reset();
}

void K4040::reset()
{
    std::memset(m_registers, 0, REGISTERS_SIZE);
    std::memset(m_stack, 0, STACK_SIZE * 2);

    m_ram.reset();
}

void K4040::step()
{
    m_IR = m_rom.readByte(getPC());
    incStack();

    uint8_t opcode = getOpcodeFromByte(m_IR);
    switch (opcode) {
    case +AsmIns::NOP: NOP(); break;
    case +AsmIns::WRM: WRM(m_ram, m_ACC); break;
    case +AsmIns::WMP: WMP(m_ram, m_ACC); break;
    case +AsmIns::WRR: WRR(m_rom, m_ACC); break;
    case +AsmIns::WR0: WR0(m_ram, m_ACC); break;
    case +AsmIns::WR1: WR1(m_ram, m_ACC); break;
    case +AsmIns::WR2: WR2(m_ram, m_ACC); break;
    case +AsmIns::WR3: WR3(m_ram, m_ACC); break;
    case +AsmIns::SBM: SBM(m_ACC, m_ram); break;
    case +AsmIns::RDM: RDM(m_ACC, m_ram); break;
    case +AsmIns::RDR: RDR(m_ACC, m_rom); break;
    case +AsmIns::ADM: ADM(m_ACC, m_ram); break;
    case +AsmIns::RD0: RD0(m_ACC, m_ram); break;
    case +AsmIns::RD1: RD1(m_ACC, m_ram); break;
    case +AsmIns::RD2: RD2(m_ACC, m_ram); break;
    case +AsmIns::RD3: RD3(m_ACC, m_ram); break;
    case +AsmIns::CLB: CLB(m_ACC); break;
    case +AsmIns::CLC: CLC(m_ACC); break;
    case +AsmIns::IAC: IAC(m_ACC); break;
    case +AsmIns::CMC: CMC(m_ACC); break;
    case +AsmIns::CMA: CMA(m_ACC); break;
    case +AsmIns::RAL: RAL(m_ACC); break;
    case +AsmIns::RAR: RAR(m_ACC); break;
    case +AsmIns::TCC: TCC(m_ACC); break;
    case +AsmIns::DAC: DAC(m_ACC); break;
    case +AsmIns::TCS: TCS(m_ACC); break;
    case +AsmIns::STC: STC(m_ACC); break;
    case +AsmIns::DAA: DAA(m_ACC); break;
    case +AsmIns::KBP: KBP(m_ACC); break;
    case +AsmIns::DCL: DCL(m_ram, m_ACC); break;
    case +AsmIns::JCN: JCN(m_stack, m_SP, m_IR, m_ACC, m_test, m_rom); break;
    case +AsmIns::FIM: FIM(m_stack, m_SP, m_registers, m_IR, m_rom); break;
    case +AsmIns::SRC: SRC(m_ram, m_rom, m_registers, m_IR); break;
    case +AsmIns::FIN: FIN(m_registers, getPC(), m_IR, m_rom); break;
    case +AsmIns::JIN: JIN(m_stack, m_SP, m_registers, m_IR); break;
    case +AsmIns::JUN: JUN(m_stack, m_SP, m_IR, m_rom); break;
    case +AsmIns::JMS: JMS(m_stack, m_SP, m_IR, m_rom); break;
    case +AsmIns::INC: INC(m_registers, m_IR); break;
    case +AsmIns::ISZ: ISZ(m_stack, m_SP, m_registers, m_IR, m_rom); break;
    case +AsmIns::ADD: ADD(m_ACC, m_registers, m_IR); break;
    case +AsmIns::SUB: SUB(m_ACC, m_registers, m_IR); break;
    case +AsmIns::LD:  LD(m_ACC, m_registers, m_IR);  break;
    case +AsmIns::XCH: XCH(m_ACC, m_registers, m_IR); break;
    case +AsmIns::BBL: BBL(m_stack, m_SP, m_ACC, m_registers, m_IR); break;
    case +AsmIns::LDM: LDM(m_ACC, m_IR); break;
    }
}
