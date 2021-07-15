#pragma once
#include "shared/source/whitebox.hpp"

#include <cstdint>

static constexpr uint16_t BITMASK_12BITS = 0x0FFF;
static constexpr uint8_t BITMASK_4BITS = 0x0F;
static constexpr uint8_t BITMASK_1BIT = 0x01;

class ROM;

class K4004
{
public:
    K4004(ROM& rom);

    void step();
    void reset();
    void pushStack(uint16_t address);
    void pullStack();

    uint8_t getAcc() const { return m_Acc; }
    uint8_t getCY() const { return m_CY; }
    uint8_t getIR() const { return m_IR; }
    const uint16_t* getStack() const { return m_stack; }
    uint16_t getPC() const { return m_PC; }
    const uint8_t* getRegisters() const { return m_registers; }
private:
    uint8_t getRegisterValue(uint8_t reg);
    void setRegisterValue(uint8_t reg, uint8_t value);
    void WRM();
    void WMP();
    void WRR();
    void WR0();
    void WR1();
    void WR2();
    void WR3();
    void SBM();
    void RDM();
    void RDR();
    void ADM();
    void RD0();
    void RD1();
    void RD2();
    void RD3();
    void CLB();
    void CLC();
    void IAC();
    void CMC();
    void CMA();
    void RAL();
    void RAR();
    void TCC();
    void DAC();
    void TCS();
    void STC();
    void DAA();
    void KBP();
    void DCL();
    void JCN();
    void FIM();
    void SRC();
    void FIN();
    void JIN();
    void JUN();
    void JMS();
    void INC();
    void ISZ();
    void ADD();
    void SUB();
    void LD();
    void XCH();
    void BBL();
    void LDM();

    const ROM& m_rom;
    uint8_t m_registers[8];
    uint16_t m_stack[3];
    uint16_t m_PC;
    uint8_t m_IR;
    uint8_t m_Acc;
    uint8_t m_CY;
    uint8_t m_CM_RAM;

    ALLOW_WHITEBOX(K4004);
};
