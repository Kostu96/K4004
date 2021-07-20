#pragma once
#include <cstdint>

constexpr uint8_t ASM_NOP = 0x00u;
constexpr uint8_t ASM_JCN = 0x10u; // Two-byte instruction
constexpr uint8_t ASM_FIM = 0x20u; // Two-byte instruction
constexpr uint8_t ASM_SRC = 0x21u; // Complex instruction
constexpr uint8_t ASM_FIN = 0x30u; // Complex instruction
constexpr uint8_t ASM_JIN = 0x31u; // Complex instruction
constexpr uint8_t ASM_JUN = 0x40u; // Two-byte instruction
constexpr uint8_t ASM_JMS = 0x50u; // Two-byte instruction
constexpr uint8_t ASM_INC = 0x60u; // Complex instruction
constexpr uint8_t ASM_ISZ = 0x70u; // Two-byte instruction
constexpr uint8_t ASM_ADD = 0x80u; // Complex instruction
constexpr uint8_t ASM_SUB = 0x90u; // Complex instruction
constexpr uint8_t ASM_LD  = 0xA0u; // Complex instruction
constexpr uint8_t ASM_XCH = 0xB0u; // Complex instruction
constexpr uint8_t ASM_BBL = 0xC0u; // Complex instruction
constexpr uint8_t ASM_LDM = 0xD0u; // Complex instruction
constexpr uint8_t ASM_WRM = 0xE0u;
constexpr uint8_t ASM_WMP = 0xE1u;
constexpr uint8_t ASM_WRR = 0xE2u;
constexpr uint8_t ASM_WR0 = 0xE4u;
constexpr uint8_t ASM_WR1 = 0xE5u;
constexpr uint8_t ASM_WR2 = 0xE6u;
constexpr uint8_t ASM_WR3 = 0xE7u;
constexpr uint8_t ASM_SBM = 0xE8u;
constexpr uint8_t ASM_RDM = 0xE9u;
constexpr uint8_t ASM_RDR = 0xEAu;
constexpr uint8_t ASM_ADM = 0xEBu;
constexpr uint8_t ASM_RD0 = 0xECu;
constexpr uint8_t ASM_RD1 = 0xEDu;
constexpr uint8_t ASM_RD2 = 0xEEu;
constexpr uint8_t ASM_RD3 = 0xEFu;
constexpr uint8_t ASM_CLB = 0xF0u;
constexpr uint8_t ASM_CLC = 0xF1u;
constexpr uint8_t ASM_IAC = 0xF2u;
constexpr uint8_t ASM_CMC = 0xF3u;
constexpr uint8_t ASM_CMA = 0xF4u;
constexpr uint8_t ASM_RAL = 0xF5u;
constexpr uint8_t ASM_RAR = 0xF6u;
constexpr uint8_t ASM_TCC = 0xF7u;
constexpr uint8_t ASM_DAC = 0xF8u;
constexpr uint8_t ASM_TCS = 0xF9u;
constexpr uint8_t ASM_STC = 0xFAu;
constexpr uint8_t ASM_DAA = 0xFBu;
constexpr uint8_t ASM_KBP = 0xFCu;
constexpr uint8_t ASM_DCL = 0xFDu;

constexpr uint8_t ASM_R0 = 0x00u;
constexpr uint8_t ASM_R1 = 0x01u;
constexpr uint8_t ASM_R2 = 0x02u;
constexpr uint8_t ASM_R3 = 0x03u;
constexpr uint8_t ASM_R4 = 0x04u;
constexpr uint8_t ASM_R5 = 0x05u;
constexpr uint8_t ASM_R6 = 0x06u;
constexpr uint8_t ASM_R7 = 0x07u;
constexpr uint8_t ASM_R8 = 0x08u;
constexpr uint8_t ASM_R9 = 0x09u;
constexpr uint8_t ASM_RA = 0x0Au;
constexpr uint8_t ASM_RB = 0x0Bu;
constexpr uint8_t ASM_RC = 0x0Cu;
constexpr uint8_t ASM_RD = 0x0Du;
constexpr uint8_t ASM_RE = 0x0Eu;
constexpr uint8_t ASM_RF = 0x0Fu;
constexpr uint8_t ASM_P0 = 0x00u;
constexpr uint8_t ASM_P1 = 0x02u;
constexpr uint8_t ASM_P2 = 0x04u;
constexpr uint8_t ASM_P3 = 0x06u;
constexpr uint8_t ASM_P4 = 0x08u;
constexpr uint8_t ASM_P5 = 0x0Au;
constexpr uint8_t ASM_P6 = 0x0Cu;
constexpr uint8_t ASM_P7 = 0x0Eu;

constexpr uint8_t ASM_CON_AEZ = 0b0100u;
constexpr uint8_t ASM_CON_ANZ = 0b1100u;
constexpr uint8_t ASM_CON_CEZ = 0b1010u;
constexpr uint8_t ASM_CON_CNZ = 0b0010u;
constexpr uint8_t ASM_CON_TEZ = 0b0001u;
constexpr uint8_t ASM_CON_TNZ = 0b1001u;

constexpr uint8_t ASM_JCN_MASK = 0x1F;
constexpr uint8_t ASM_FIM_MASK = 0x2E;
constexpr uint8_t ASM_SRC_MASK = 0x2F;
constexpr uint8_t ASM_FIN_MASK = 0x3E;
constexpr uint8_t ASM_JIN_MASK = 0x3F;
constexpr uint8_t ASM_JUN_MASK = 0x4F;
constexpr uint8_t ASM_JMS_MASK = 0x5F;
constexpr uint8_t ASM_INC_MASK = 0x6F;
constexpr uint8_t ASM_ISZ_MASK = 0x7F;
constexpr uint8_t ASM_ADD_MASK = 0x8F;
constexpr uint8_t ASM_SUB_MASK = 0x9F;
constexpr uint8_t ASM_LD_MASK  = 0xAF;
constexpr uint8_t ASM_XCH_MASK = 0xBF;
constexpr uint8_t ASM_BBL_MASK = 0xCF;
constexpr uint8_t ASM_LDM_MASK = 0xDF;

constexpr uint8_t  BITMASK8_1BIT       = 0x01u;
constexpr uint8_t  BITMASK8_3BIT       = 0x07u;
constexpr uint8_t  BITMASK8_4BITS      = 0x0Fu;
constexpr uint8_t  BITMASK8_4BITS_Q1   = 0xF0u;
constexpr uint16_t BITMASK16_4BITS_Q2  = 0x0F00u;
constexpr uint16_t BITMASK16_8BITS     = 0x00FFu;
constexpr uint16_t BITMASK16_12BITS    = 0x0FFFu;

uint8_t getOpcodeFromByte(uint8_t byte);
