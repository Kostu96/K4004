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

void K4004::reset()
{
    std::memset(m_registers, 0, REGISTERS_SIZE);
    std::memset(m_stack, 0, STACK_SIZE * 2);
    m_SP = 0u;
    m_ACC = 0u;
    m_test = 0u;
    m_CM_RAM = 0u;
    m_ram.reset();
}

uint8_t K4004::step()
{
    uint8_t cycles = 0u;

    m_IR = m_rom.readByte(getPC());
    incPC();

    uint8_t opcode = getOpcodeFromByte(m_IR);
    switch (opcode) {
    case +AsmIns::NOP: cycles = 1u; NOP(); break;
    case +AsmIns::WRM: cycles = 1u; WRM(m_ram, m_ACC); break;
    case +AsmIns::WMP: cycles = 1u; WMP(m_ram, m_ACC); break;
    case +AsmIns::WRR: cycles = 1u; WRR(m_rom, m_ACC); break;
    case +AsmIns::WR0: cycles = 1u; WR0(m_ram, m_ACC); break;
    case +AsmIns::WR1: cycles = 1u; WR1(m_ram, m_ACC); break;
    case +AsmIns::WR2: cycles = 1u; WR2(m_ram, m_ACC); break;
    case +AsmIns::WR3: cycles = 1u; WR3(m_ram, m_ACC); break;
    case +AsmIns::SBM: cycles = 1u; SBM(m_ACC, m_ram); break;
    case +AsmIns::RDM: cycles = 1u; RDM(m_ACC, m_ram); break;
    case +AsmIns::RDR: cycles = 1u; RDR(m_ACC, m_rom); break;
    case +AsmIns::ADM: cycles = 1u; ADM(m_ACC, m_ram); break;
    case +AsmIns::RD0: cycles = 1u; RD0(m_ACC, m_ram); break;
    case +AsmIns::RD1: cycles = 1u; RD1(m_ACC, m_ram); break;
    case +AsmIns::RD2: cycles = 1u; RD2(m_ACC, m_ram); break;
    case +AsmIns::RD3: cycles = 1u; RD3(m_ACC, m_ram); break;
    case +AsmIns::CLB: cycles = 1u; CLB(m_ACC); break;
    case +AsmIns::CLC: cycles = 1u; CLC(m_ACC); break;
    case +AsmIns::IAC: cycles = 1u; IAC(m_ACC); break;
    case +AsmIns::CMC: cycles = 1u; CMC(m_ACC); break;
    case +AsmIns::CMA: cycles = 1u; CMA(m_ACC); break;
    case +AsmIns::RAL: cycles = 1u; RAL(m_ACC); break;
    case +AsmIns::RAR: cycles = 1u; RAR(m_ACC); break;
    case +AsmIns::TCC: cycles = 1u; TCC(m_ACC); break;
    case +AsmIns::DAC: cycles = 1u; DAC(m_ACC); break;
    case +AsmIns::TCS: cycles = 1u; TCS(m_ACC); break;
    case +AsmIns::STC: cycles = 1u; STC(m_ACC); break;
    case +AsmIns::DAA: cycles = 1u; DAA(m_ACC); break;
    case +AsmIns::KBP: cycles = 1u; KBP(m_ACC); break;
    case +AsmIns::DCL: cycles = 1u; DCL(m_ram, m_ACC); break;
    case +AsmIns::JCN: cycles = 2u; JCN(m_stack, m_SP, m_IR, m_ACC, m_test, m_rom); break;
    case +AsmIns::FIM: cycles = 2u; FIM(m_stack, m_SP, m_registers, m_IR, m_rom); break;
    case +AsmIns::SRC: cycles = 1u; SRC(m_ram, m_rom, m_registers, m_IR); break;
    case +AsmIns::FIN: cycles = 2u; FIN(m_registers, getPC(), m_IR, m_rom); break;
    case +AsmIns::JIN: cycles = 1u; JIN(m_stack, m_SP, m_registers, m_IR); break;
    case +AsmIns::JUN: cycles = 2u; JUN(m_stack, m_SP, m_IR, m_rom); break;
    case +AsmIns::JMS: cycles = 2u; JMS(m_stack, m_SP, m_IR, m_rom); break;
    case +AsmIns::INC: cycles = 1u; INC(m_registers, m_IR); break;
    case +AsmIns::ISZ: cycles = 2u; ISZ(m_stack, m_SP, m_registers, m_IR, m_rom); break;
    case +AsmIns::ADD: cycles = 1u; ADD(m_ACC, m_registers, m_IR); break;
    case +AsmIns::SUB: cycles = 1u; SUB(m_ACC, m_registers, m_IR); break;
    case +AsmIns::LD:  cycles = 1u; LD(m_ACC, m_registers, m_IR);  break;
    case +AsmIns::XCH: cycles = 1u; XCH(m_ACC, m_registers, m_IR); break;
    case +AsmIns::BBL: cycles = 1u; BBL(m_stack, m_SP, m_ACC, m_registers, m_IR); break;
    case +AsmIns::LDM: cycles = 1u; LDM(m_ACC, m_IR); break;
    }

    return cycles;
}
