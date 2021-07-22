#pragma once
#include <cstdint>

enum class AsmIns : uint8_t {
    NOP = 0x00u,
    HLT = 0x01u,
    BBS = 0x02u,
    LCR = 0x03u,
    OR4 = 0x04u,
    OR5 = 0x05u,
    AN6 = 0x06u,
    AN7 = 0x07u,
    DB0 = 0x08u,
    DB1 = 0x09u,
    SB0 = 0x0Au,
    SB1 = 0x0Bu,
    EIN = 0x0Cu,
    DIN = 0x0Du,
    RPM = 0x0Eu,
    JCN = 0x10u, // Two-byte instruction
    FIM = 0x20u, // Two-byte instruction
    SRC = 0x21u, // Complex instruction
    FIN = 0x30u, // Complex instruction
    JIN = 0x31u, // Complex instruction
    JUN = 0x40u, // Two-byte instruction
    JMS = 0x50u, // Two-byte instruction
    INC = 0x60u, // Complex instruction
    ISZ = 0x70u, // Two-byte instruction
    ADD = 0x80u, // Complex instruction
    SUB = 0x90u, // Complex instruction
    LD  = 0xA0u, // Complex instruction
    XCH = 0xB0u, // Complex instruction
    BBL = 0xC0u, // Complex instruction
    LDM = 0xD0u, // Complex instruction
    WRM = 0xE0u,
    WMP = 0xE1u,
    WRR = 0xE2u,
    WPM = 0xE3u,
    WR0 = 0xE4u,
    WR1 = 0xE5u,
    WR2 = 0xE6u,
    WR3 = 0xE7u,
    SBM = 0xE8u,
    RDM = 0xE9u,
    RDR = 0xEAu,
    ADM = 0xEBu,
    RD0 = 0xECu,
    RD1 = 0xEDu,
    RD2 = 0xEEu,
    RD3 = 0xEFu,
    CLB = 0xF0u,
    CLC = 0xF1u,
    IAC = 0xF2u,
    CMC = 0xF3u,
    CMA = 0xF4u,
    RAL = 0xF5u,
    RAR = 0xF6u,
    TCC = 0xF7u,
    DAC = 0xF8u,
    TCS = 0xF9u,
    STC = 0xFAu,
    DAA = 0xFBu,
    KBP = 0xFCu,
    DCL = 0xFDu,

    Count = 60u
};

inline constexpr uint8_t operator+(const AsmIns val) { return static_cast<uint8_t>(val); }

enum class AsmReg : uint8_t {
    R0 = 0x00u,
    R1 = 0x01u,
    R2 = 0x02u,
    R3 = 0x03u,
    R4 = 0x04u,
    R5 = 0x05u,
    R6 = 0x06u,
    R7 = 0x07u,
    R8 = 0x08u,
    R9 = 0x09u,
    RA = 0x0Au,
    RB = 0x0Bu,
    RC = 0x0Cu,
    RD = 0x0Du,
    RE = 0x0Eu,
    RF = 0x0Fu,
    P0 = 0x00u,
    P1 = 0x02u,
    P2 = 0x04u,
    P3 = 0x06u,
    P4 = 0x08u,
    P5 = 0x0Au,
    P6 = 0x0Cu,
    P7 = 0x0Eu,

    Count = 24u
};

inline constexpr uint8_t operator+(const AsmReg val) { return static_cast<uint8_t>(val); }

enum class AsmCon : uint8_t {
    AEZ = 0b0100u,
    ANZ = 0b1100u,
    CEZ = 0b1010u,
    CNZ = 0b0010u,
    TEZ = 0b0001u,
    TNZ = 0b1001u,

    Count = 6u
};

inline constexpr uint8_t operator+(const AsmCon val) { return static_cast<uint8_t>(val); }

uint8_t getOpcodeFromByte(uint8_t byte);
