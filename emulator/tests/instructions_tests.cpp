#include "emulator/source/instructions.hpp"
#include "emulator/source/ram.hpp"
#include "emulator/source/rom.hpp"

#include <gtest/gtest.h>

TEST(InstructionsTests, WRMTest) {
    uint8_t acc = 0x07u;
    RAM ram;
    ram.writeSrcAddress(0b00100111u); // chip 0 | reg 2 | char 7

    WRM(ram, acc);

    EXPECT_EQ(ram.readRAM(), acc);
}

TEST(InstructionsTests, WMPTest) {
    uint8_t acc = 0x07u;
    RAM ram;
    ram.writeSrcAddress(0b10000000u); // chip 2

    WMP(ram, acc);

    EXPECT_EQ(ram.readOutputPort(), acc);
}

TEST(InstructionsTests, WRRTest) {
    uint8_t acc = 0x07u;
    ROM rom;
    rom.writeSrcAddress(0u);

    WRR(rom, acc);

    EXPECT_EQ(rom.getIOPort(0u), acc);
}

TEST(InstructionsTests, WR0Test) {
    uint8_t acc = 0x07u;
    RAM ram;
    ram.writeSrcAddress(0b10100000u); // chip 2 | reg 2
    
    WR0(ram, acc);

    EXPECT_EQ(ram.getStatusContents()[40], acc);
}

TEST(InstructionsTests, WR1Test) {
    uint8_t acc = 0x07u;
    RAM ram;
    ram.writeSrcAddress(0b10100000u); // chip 2 | reg 2

    WR1(ram, acc);

    EXPECT_EQ(ram.getStatusContents()[41], acc);
}

TEST(InstructionsTests, WR2Test) {
    uint8_t acc = 0x07u;
    RAM ram;
    ram.writeSrcAddress(0b10100000u); // chip 2 | reg 2

    WR2(ram, acc);

    EXPECT_EQ(ram.getStatusContents()[42], acc);
}

TEST(InstructionsTests, WR3Test) {
    uint8_t acc = 0x07u;
    RAM ram;
    ram.writeSrcAddress(0b10100000u); // chip 2 | reg 2

    WR3(ram, acc);

    EXPECT_EQ(ram.getStatusContents()[43], acc);
}

TEST(InstructionsTests, SBMTest) {
    uint8_t acc = 0x07u;
    RAM ram;
    ram.writeSrcAddress(0b00100111u); // chip 0 | reg 2 | char 7
    ram.writeRAM(0x02u);

    SBM(acc, ram);

    EXPECT_EQ(acc, (0x07u - 0x02u) | 1u << 4);

    acc &= 0x0Fu;
    ram.writeRAM(0x0Fu);

    SBM(acc, ram);

    EXPECT_EQ(acc, (0x05u - 0x0Fu) & 0x0F);

    acc |= 1u << 4;
    ram.writeRAM(0x03u);

    SBM(acc, ram);

    EXPECT_EQ(acc, ((0x06u - 0x03u - 1u) | 1u << 4) & 0x1Fu);
}

TEST(InstructionsTests, RDMTest) {
    uint8_t acc = 0u;
    RAM ram;
    ram.writeSrcAddress(0b00100111u); // chip 0 | reg 2 | char 7
    ram.writeRAM(0x07u);
    
    RDM(acc, ram);

    EXPECT_EQ(acc, 0x07u);
}

TEST(InstructionsTests, RDRTest) {
    uint8_t acc = 0x0Fu;
    ROM rom;

    RDR(acc, rom);

    EXPECT_EQ(acc, 0u);
}

TEST(InstructionsTests, ADMTest) {
    uint8_t acc = 0x07u;
    RAM ram;
    ram.writeSrcAddress(0b00100111u); // chip 0 | reg 2 | char 7
    ram.writeRAM(0x02u);
    
    ADM(acc, ram);

    EXPECT_EQ(acc, 0x02u + 0x07u);

    ram.writeRAM(0x0Fu);
    
    ADM(acc, ram);

    EXPECT_EQ(acc, ((0x09u + 0x0Fu) & 0x0F) | 1u << 4);

    ram.writeRAM(0x02u);

    ADM(acc, ram);

    EXPECT_EQ(acc, 0x08u + 0x02u + 1u);
}

TEST(InstructionsTests, RD0Test) {
    uint8_t acc = 0u;
    RAM ram;
    ram.writeSrcAddress(0b10100000u); // chip 2 | reg 2
    ram.writeStatus(0x07u, 0u);
    
    RD0(acc, ram);

    EXPECT_EQ(acc, 0x07u);
}

TEST(InstructionsTests, RD1Test) {
    uint8_t acc = 0u;
    RAM ram;
    ram.writeSrcAddress(0b10100000u); // chip 2 | reg 2
    ram.writeStatus(0x07u, 1u);

    RD1(acc, ram);

    EXPECT_EQ(acc, 0x07u);
}

TEST(InstructionsTests, RD2Test) {
    uint8_t acc = 0u;
    RAM ram;
    ram.writeSrcAddress(0b10100000u); // chip 2 | reg 2
    ram.writeStatus(0x07u, 2u);

    RD2(acc, ram);

    EXPECT_EQ(acc, 0x07u);
}

TEST(InstructionsTests, RD3Test) {
    uint8_t acc = 0u;
    RAM ram;
    ram.writeSrcAddress(0b10100000u); // chip 2 | reg 2
    ram.writeStatus(0x07u, 3u);

    RD3(acc, ram);

    EXPECT_EQ(acc, 0x07u);
}

TEST(InstructionsTests, CLBTest) {
    uint8_t acc = 0x17u;

    CLB(acc);

    EXPECT_EQ(acc, 0x00u);
}

TEST(InstructionsTests, CLCTest) {
    uint8_t acc = 0x17u;
    
    CLC(acc);

    EXPECT_EQ(acc, 0x07u);
}

TEST(InstructionsTests, IACTest) {
    uint8_t acc = 0x0Eu;

    IAC(acc);

    EXPECT_EQ(acc, 0x0Fu);

    IAC(acc);

    EXPECT_EQ(acc, 0x10u);

    IAC(acc);

    EXPECT_EQ(acc, 0x01u);
}

TEST(InstructionsTests, CMCTest) {
    uint8_t acc = 0x07u;

    CMC(acc);

    EXPECT_EQ(acc, 0x17u);

    CMC(acc);

    EXPECT_EQ(acc, 0x07u);
}

TEST(InstructionsTests, CMATest) {
    uint8_t acc = 0x04u;

    CMA(acc);

    EXPECT_EQ(acc, ~0x04u & 0x0F);
}

TEST(InstructionsTests, RALTest) {
    uint8_t acc = 0x04u;

    RAL(acc);

    EXPECT_EQ(acc, 0x08u);

    acc = 0x0Au;

    RAL(acc);

    EXPECT_EQ(acc, 0x14u);

    RAL(acc);

    EXPECT_EQ(acc, 0x09u);
}

TEST(InstructionsTests, RARTest) {
    uint8_t acc = 0x04u;

    RAR(acc);

    EXPECT_EQ(acc, 0x02u);

    acc = 0x05u;

    RAR(acc);

    EXPECT_EQ(acc, 0x12u);

    RAR(acc);

    EXPECT_EQ(acc, 0x09u);
}

TEST(InstructionsTests, TCCTest) {
    uint8_t acc = 0x07u;
    
    TCC(acc);

    EXPECT_EQ(acc, 0x00u);

    acc = 0x17u;

    TCC(acc);

    EXPECT_EQ(acc, 0x01u);
}

TEST(InstructionsTests, DACTest) {
    uint8_t acc = 0x07u;
    
    DAC(acc);

    EXPECT_EQ(acc, 0x16u);

    acc = 0x00u;
    
    DAC(acc);

    EXPECT_EQ(acc, 0x0Fu);
}

TEST(InstructionsTests, TCSTest) {
    uint8_t acc = 0x07u;

    TCS(acc);

    EXPECT_EQ(acc, 0x09u);

    acc = 0x19u;
    
    TCS(acc);

    EXPECT_EQ(acc, 0x0Au);
}

TEST(InstructionsTests, STCTest) {
    uint8_t acc = 0x07u;

    STC(acc);

    EXPECT_EQ(acc, 0x17u);
}

TEST(InstructionsTests, DAATest) {
    uint8_t acc = 0x05u;
    
    DAA(acc);

    EXPECT_EQ(acc, 0x05u);

    acc = 0x0Au;
    
    DAA(acc);

    EXPECT_EQ(acc, 0x10u);

    acc = 0x12u;

    DAA(acc);

    EXPECT_EQ(acc, 0x18u);
}

TEST(InstructionsTests, KBPTest) {
    uint8_t acc = 0u;

    KBP(acc);

    EXPECT_EQ(acc, 0b0000u);

    acc = 0b0001u;

    KBP(acc);

    EXPECT_EQ(acc, 0b0001u);

    acc = 0b0010u;
    
    KBP(acc);

    EXPECT_EQ(acc, 0b0010u);

    acc = 0b0100u;
    
    KBP(acc);

    EXPECT_EQ(acc, 0b0011u);

    acc = 0b1000u;
    
    KBP(acc);

    EXPECT_EQ(acc, 0b0100u);

    acc = 0b0011u;
    
    KBP(acc);

    EXPECT_EQ(acc, 0b1111u);

    acc = 0b1111u;
    
    KBP(acc);

    EXPECT_EQ(acc, 0b1111u);
}

// TODO:
/*TEST_F(EmulatorInstructionsTests, DCLTest) {
    rom[0] = +AsmIns::DCL;
    *acc = 0x02u;
    *ramSrcAddr = 0b01010101u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(*acc, 0x02u);
    EXPECT_EQ(*ramSrcAddr, 0b1001010101u);
}*/

TEST(InstructionsTests, LDMTest) {
    uint8_t acc = 0x10u;
    
    LDM(acc, 0xF7u);
   
    EXPECT_EQ(acc, 0x17u);
}

TEST(InstructionsTests, LDTest) {
    uint8_t acc = 0x10u;
    uint8_t registers[8];
    registers[1] = 0x20;

    LD(acc, registers, 0xF2u);

    EXPECT_EQ(acc, 0x12u);
}

TEST(InstructionsTests, XCHTest) {
    uint8_t acc = 0x07u;
    uint8_t registers[8];
    registers[1] = 0x24;
    
    XCH(acc, registers, 0xF2u);

    EXPECT_EQ(acc, 0x02u);
    EXPECT_EQ(registers[1], 0x74u);
}

TEST(InstructionsTests, ADDTest) {
    uint8_t acc = 0x07u;
    uint8_t registers[8];
    registers[1] = 0x20u;
    
    ADD(acc, registers, 0xF2u);

    EXPECT_EQ(acc, 0x02u + 0x07u);

    registers[1] = 0xF0u;
    
    ADD(acc, registers, 0xF2u);

    EXPECT_EQ(acc, 0x09u + 0x0Fu);

    registers[1] = 0x20u;
    
    ADD(acc, registers, 0xF2u);

    EXPECT_EQ(acc, 0x08u + 0x02u + 1u);
}

TEST(InstructionsTests, SUBTest) {
    uint8_t acc = 0x07u;
    uint8_t registers[8];
    registers[1] = 0x20u;
    
    SUB(acc, registers, 0xF2u);

    EXPECT_EQ(acc, (0x07u - 0x02u) | 1u << 4);

    acc &= 0x0Fu;
    registers[1] = 0xF0u;
    
    SUB(acc, registers, 0xF2u);

    EXPECT_EQ(acc, (0x05u - 0x0Fu) & 0x0F);

    acc |= 1u << 4;
    registers[1] = 0x30u;
    
    SUB(acc, registers, 0xF2u);

    EXPECT_EQ(acc, (0x06u - 0x03u - 1u) | 1u << 4);
}

TEST(InstructionsTests, INCTest) {
    uint8_t registers[8];
    registers[1] = 0xE0u;
    
    INC(registers, 0xF2u);

    EXPECT_EQ(registers[1], 0xF0u);

    INC(registers, 0xF2u);

    EXPECT_EQ(registers[1], 0x00u);
}

TEST(InstructionsTests, BBLTest) {
    uint8_t acc;
    uint8_t registers[8];
    uint16_t stack[4];
    uint8_t sp = 1u;
    registers[1] = 0x20u;
    stack[0] = 0x010u;
    stack[sp] = 0u;
    
    BBL(stack, sp, acc, registers, 0xF2u);

    EXPECT_EQ(sp, 0u);
    EXPECT_EQ(stack[sp], 0x010u);
    EXPECT_EQ(acc, 0x02u);
}

// TODO: add means to edit rom
/*TEST(InstructionsTests, JUNTest) {
    rom[1] = 0x42u;
    ROM rom;
    uint16_t stack[4];
    uint8_t sp = 1u;

    JUN(stack, sp, 0xF1u, rom);

    EXPECT_EQ(stack[sp], 0x142u);
}

TEST(InstructionsTests, JMSTest) {
    rom[0] = +AsmIns::JMS | 0x01u;
    rom[1] = 0x42u;
    emulator.step();

    EXPECT_EQ(*pc, 0x142u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], i == 0 ? 0x002u : 0x000u);
    EXPECT_EQ(*stackDepth, 1u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, FIMTest) {
    rom[0] = +AsmIns::FIM | +AsmReg::P1;
    rom[1] = 0x42;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1u ? 0x42u : 0x00u);
    EXPECT_EQ(*acc, 0u);
    EXPECT_EQ(*CY, 0u);
}*/

TEST(InstructionsTests, SRCTest) {
    uint8_t registers[8];
    ROM rom;
    RAM ram;
    registers[2] = 0x42u;
    
    SRC(ram, rom, registers, 0xF4u);

    EXPECT_EQ(rom.getSrcAddress(), 0x04u);
    EXPECT_EQ(ram.getSrcAddress(), 0x042u);
}

// TODO: add means to edit rom
/*TEST_F(EmulatorInstructionsTests, JCNTest) {
    rom[0] = +AsmIns::JCN | +AsmCon::AEZ;
    rom[1] = 0x42u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *acc = 0x01u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);

    rom[0] = +AsmIns::JCN | +AsmCon::ANZ;
    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *acc = 0x00u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);

    rom[0] = +AsmIns::JCN | +AsmCon::CEZ;
    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *CY = 1u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);

    rom[0] = +AsmIns::JCN | +AsmCon::CNZ;
    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *CY = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);

    rom[0] = +AsmIns::JCN | +AsmCon::TEZ;
    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *test = 1u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);

    rom[0] = +AsmIns::JCN | +AsmCon::TNZ;
    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *test = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);
}

TEST_F(EmulatorInstructionsTests, FINTest) {
    rom[0] = +AsmIns::FIN | +AsmReg::P2;
    rom[0x42] = 0x21u;
    registers[0] = 0x42u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    EXPECT_EQ(registers[2], 0x21u);
}*/

TEST(InstructionsTests, JINTest) {
    uint8_t registers[8];
    uint16_t stack[4];
    uint8_t sp = 1u;
    registers[1] = 0x42u;
    stack[sp] = 0u;
    
    JIN(stack, sp, registers, 0xF2u);

    EXPECT_EQ(stack[sp], 0x042u);
}

// TODO: add means to edit rom
/*TEST_F(EmulatorInstructionsTests, ISZTest) {
    rom[0] = +AsmIns::ISZ | +AsmReg::R1;
    rom[1] = 0x42u;
    registers[0] = 0x0Eu;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);
    EXPECT_EQ(registers[0], 0x0Fu);

    *pc = 0u;
    emulator.step();
    EXPECT_EQ(*pc, 0x002u);
    EXPECT_EQ(registers[0], 0x00u);
}*/

// TODO: Make tests for instructions that operates on page boundary
