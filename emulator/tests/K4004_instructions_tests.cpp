#include "emulator/source/emulator.hpp"

#include "shared/source/assembly.hpp"

#include <gtest/gtest.h>

template<>
struct WhiteBox<RAM> {
    static uint8_t* getRAMOutputPorts(RAM& ram) {
        return ram.m_oPorts;
    }

    static uint8_t* getRAMStatus(RAM& ram) {
        return ram.m_status;
    }

    static uint8_t* getRAMData(RAM& ram) {
        return ram.m_ram;
    }

    static uint16_t* getSrcAddress(RAM& ram) {
        return &ram.m_srcAddress;
    }
};

template<>
struct WhiteBox<ROM> {
    static uint8_t* getROMData(ROM& rom) {
        return rom.m_rom;
    }

    static uint8_t* getSrcAddress(ROM& rom) {
        return &rom.m_srcAddress;
    }
};

template<>
struct WhiteBox<K4004> {
    static uint8_t* getAcc(K4004& cpu) {
        return &cpu.m_Acc;
    }

    static uint8_t* getCY(K4004& cpu) {
        return &cpu.m_CY;
    }

    static uint8_t* getTest(K4004& cpu) {
        return &cpu.m_test;
    }

    static uint16_t* getPC(K4004& cpu) {
        return &cpu.m_PC;
    }

    static uint16_t* getStack(K4004& cpu) {
        return cpu.m_stack;
    }

    static uint8_t* getStackDepth(K4004& cpu) {
        return &cpu.m_stackDepth;
    }

    static uint8_t* gerRegisters(K4004& cpu) {
        return cpu.m_registers;
    }
};

template<>
struct WhiteBox<Emulator> {
    static RAM& getRAM(Emulator& emulator) {
        return emulator.m_ram;
    }

    static ROM& getROM(Emulator& emulator) {
        return emulator.m_rom;
    }

    static K4004& getCPU(Emulator& emulator) {
        return emulator.m_cpu;
    }
};

struct EmulatorInstructionsTests : public testing::Test {
    void SetUp() override {
        auto& ramObj = WhiteBox<Emulator>::getRAM(emulator);
        ram = WhiteBox<RAM>::getRAMData(ramObj);
        ramStatus = WhiteBox<RAM>::getRAMStatus(ramObj);
        ramOutput = WhiteBox<RAM>::getRAMOutputPorts(ramObj);
        ramSrcAddr = WhiteBox<RAM>::getSrcAddress(ramObj);
        ASSERT_NE(ram, nullptr);
        ASSERT_NE(ramStatus, nullptr);
        ASSERT_NE(ramOutput, nullptr);
        ASSERT_NE(ramSrcAddr, nullptr);

        auto& romObj = WhiteBox<Emulator>::getROM(emulator);
        rom = WhiteBox<ROM>::getROMData(romObj);
        romSrcAddr = WhiteBox<ROM>::getSrcAddress(romObj);
        ASSERT_NE(rom, nullptr);
        ASSERT_NE(romSrcAddr, nullptr);

        auto& cpu = WhiteBox<Emulator>::getCPU(emulator);
        acc = WhiteBox<K4004>::getAcc(cpu);
        CY = WhiteBox<K4004>::getCY(cpu);
        test = WhiteBox<K4004>::getTest(cpu);
        pc = WhiteBox<K4004>::getPC(cpu);
        stack = WhiteBox<K4004>::getStack(cpu);
        stackDepth = WhiteBox<K4004>::getStackDepth(cpu);
        registers = WhiteBox<K4004>::gerRegisters(cpu);
        ASSERT_NE(acc, nullptr);
        ASSERT_NE(CY, nullptr);
        ASSERT_NE(test, nullptr);
        ASSERT_NE(pc, nullptr);
        ASSERT_NE(stack, nullptr);
        ASSERT_NE(stackDepth, nullptr);
        ASSERT_NE(registers, nullptr);
    }

    void TearDown() override {
        emulator.reset();
    }

    Emulator emulator;
    uint8_t* rom = nullptr;
    uint8_t* romSrcAddr = nullptr;
    uint8_t* ram = nullptr;
    uint8_t* ramStatus = nullptr;
    uint8_t* ramOutput = nullptr;
    uint16_t* ramSrcAddr = nullptr;
    uint8_t* acc = nullptr;
    uint8_t* CY = nullptr;
    uint8_t* test = nullptr;
    uint16_t* pc = nullptr;
    uint16_t* stack = nullptr;
    uint8_t* stackDepth = nullptr;
    uint8_t* registers = nullptr;
};

TEST_F(EmulatorInstructionsTests, NOPTest) {
    rom[0] = ASM_NOP;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x00u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, WRMTest) {
    rom[0] = ASM_WRM;
    *acc = 0x07u;
    *ramSrcAddr = 0b00100111u; // chip 0 | reg 2 | char 7
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ram[*ramSrcAddr], *acc);
}

TEST_F(EmulatorInstructionsTests, WMPTest) {
    rom[0] = ASM_WMP;
    *acc = 0x07u;
    *ramSrcAddr = 0b10000000u; // chip 2
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ramOutput[2], *acc);
}

TEST_F(EmulatorInstructionsTests, WRRTest) {
    rom[0] = ASM_WRR;
    emulator.step();

    // TODO: Decide how to handle I/O metal config
}

TEST_F(EmulatorInstructionsTests, WR0Test) {
    rom[0] = ASM_WR0;
    *acc = 0x07u;
    *ramSrcAddr = 0b10100000u; // chip 2 | reg 2
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ramStatus[40], *acc);
}

TEST_F(EmulatorInstructionsTests, WR1Test) {
    rom[0] = ASM_WR1;
    *acc = 0x07u;
    *ramSrcAddr = 0b10100000u; // chip 2 | reg 2
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ramStatus[41], *acc);
}

TEST_F(EmulatorInstructionsTests, WR2Test) {
    rom[0] = ASM_WR2;
    *acc = 0x07u;
    *ramSrcAddr = 0b10100000u; // chip 2 | reg 2
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ramStatus[42], *acc);
}

TEST_F(EmulatorInstructionsTests, WR3Test) {
    rom[0] = ASM_WR3;
    *acc = 0x07u;
    *ramSrcAddr = 0b10100000u; // chip 2 | reg 2
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ramStatus[43], *acc);
}

TEST_F(EmulatorInstructionsTests, SBMTest) {
    rom[0] = ASM_SBM;
    *acc = 0x07u;
    *ramSrcAddr = 0b00100111u; // chip 0 | reg 2 | char 7
    ram[*ramSrcAddr] = 0x02u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u - 0x02u);
    EXPECT_EQ(*CY, 1u);

    *pc = 0u;
    *CY = 0u;
    ram[*ramSrcAddr] = 0x0Fu;
    emulator.step();

    EXPECT_EQ(*acc, (0x05u - 0x0Fu) & 0x0F);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    *CY = 1u;
    ram[*ramSrcAddr] = 0x03u;
    emulator.step();

    EXPECT_EQ(*acc, 0x06u - 0x03u - 1u);
    EXPECT_EQ(*CY, 1u);
}

TEST_F(EmulatorInstructionsTests, RDMTest) {
    rom[0] = ASM_RDM;
    *ramSrcAddr = 0b00100111u; // chip 0 | reg 2 | char 7
    ram[*ramSrcAddr] = 0x07u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ram[*ramSrcAddr], *acc);
}

TEST_F(EmulatorInstructionsTests, RDRTest) {
    rom[0] = ASM_RDR;
    emulator.step();

    // TODO: Decide how to handle I/O metal config
}

TEST_F(EmulatorInstructionsTests, ADMTest) {
    rom[0] = ASM_ADM;
    *acc = 0x07u;
    *ramSrcAddr = 0b00100111u; // chip 0 | reg 2 | char 7
    ram[*ramSrcAddr] = 0x02u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x02u + 0x07u);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    ram[*ramSrcAddr] = 0x0Fu;
    emulator.step();

    EXPECT_EQ(*acc, (0x09u + 0x0Fu) & 0x0F);
    EXPECT_EQ(*CY, 1u);

    *pc = 0u;
    ram[*ramSrcAddr] = 0x02u;
    emulator.step();

    EXPECT_EQ(*acc, 0x08u + 0x02u + 1u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, RD0Test) {
    rom[0] = ASM_RD0;
    *ramSrcAddr = 0b10100000u; // chip 2 | reg 2
    ramStatus[40] = 0x07u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ramStatus[40], *acc);
}

TEST_F(EmulatorInstructionsTests, RD1Test) {
    rom[0] = ASM_RD1;
    *ramSrcAddr = 0b10100000u; // chip 2 | reg 2
    ramStatus[41] = 0x07u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ramStatus[41], *acc);
}

TEST_F(EmulatorInstructionsTests, RD2Test) {
    rom[0] = ASM_RD2;
    *ramSrcAddr = 0b10100000u; // chip 2 | reg 2
    ramStatus[42] = 0x07u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ramStatus[42], *acc);
}

TEST_F(EmulatorInstructionsTests, RD3Test) {
    rom[0] = ASM_RD3;
    *ramSrcAddr = 0b10100000u; // chip 2 | reg 2
    ramStatus[43] = 0x07u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x07u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(ramStatus[43], *acc);
}

TEST_F(EmulatorInstructionsTests, CLBTest) {
    rom[0] = ASM_CLB;
    *acc = 0x07u;
    *CY = 1u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x00u);
    EXPECT_EQ(*CY, 0x00u);
}

TEST_F(EmulatorInstructionsTests, CLCTest) {
    rom[0] = ASM_CLC;
    *CY = 1u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x00u);
    EXPECT_EQ(*CY, 0x00u);
}

TEST_F(EmulatorInstructionsTests, IACTest) {
    rom[0] = ASM_IAC;
    *acc = 0x07u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x08u);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    *acc = 0x0Fu;
    emulator.step();

    EXPECT_EQ(*acc, 0x0u);
    EXPECT_EQ(*CY, 1u);
}

TEST_F(EmulatorInstructionsTests, CMCTest) {
    rom[0] = ASM_CMC;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x00u);
    EXPECT_EQ(*CY, 1u);

    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, CMATest) {
    rom[0] = ASM_CMA;
    *acc = 0x04u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, ~0x04u & 0x0F);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, RALTest) {
    rom[0] = ASM_RAL;
    *acc = 0x04u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x08u);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    *acc = 0x0Au;
    emulator.step();

    EXPECT_EQ(*acc, 0x04u);
    EXPECT_EQ(*CY, 1u);

    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*acc, 0x09u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, RARTest) {
    rom[0] = ASM_RAR;
    *acc = 0x04u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x02u);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    *acc = 0x05u;
    emulator.step();

    EXPECT_EQ(*acc, 0x02u);
    EXPECT_EQ(*CY, 1u);

    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*acc, 0x09u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, TCCTest) {
    rom[0] = ASM_TCC;
    *acc = 0x07u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x00u);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    *acc = 0x07u;
    *CY = 1u;
    emulator.step();

    EXPECT_EQ(*acc, 0x01u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, DACTest) {
    rom[0] = ASM_DAC;
    *acc = 0x07u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x06u);
    EXPECT_EQ(*CY, 1u);

    *pc = 0u;
    *acc = 0x00u;
    emulator.step();

    EXPECT_EQ(*acc, 0x0Fu);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, TCSTest) {
    rom[0] = ASM_TCS;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 9u);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    *CY = 1u;
    emulator.step();

    EXPECT_EQ(*acc, 10u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, STCTest) {
    rom[0] = ASM_STC;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x00u);
    EXPECT_EQ(*CY, 1u);
}

TEST_F(EmulatorInstructionsTests, DAATest) {
    rom[0] = ASM_DAA;
    *acc = 5u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 5u);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    *acc = 10u;
    emulator.step();

    EXPECT_EQ(*acc, 0x00u);
    EXPECT_EQ(*CY, 1u);

    *pc = 0u;
    *acc = 2u;
    emulator.step();

    EXPECT_EQ(*acc, 8u);
    EXPECT_EQ(*CY, 1u);
}

TEST_F(EmulatorInstructionsTests, KBPTest) {
    rom[0] = ASM_KBP;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(*acc, 0b0000u);

    *pc = 0u;
    *acc = 0b0001u;
    emulator.step();

    EXPECT_EQ(*acc, 0b0001u);

    *pc = 0u;
    *acc = 0b0010u;
    emulator.step();

    EXPECT_EQ(*acc, 0b0010u);

    *pc = 0u;
    *acc = 0b0100u;
    emulator.step();

    EXPECT_EQ(*acc, 0b0011u);

    *pc = 0u;
    *acc = 0b1000u;
    emulator.step();

    EXPECT_EQ(*acc, 0b0100u);

    *pc = 0u;
    *acc = 0b0011u;
    emulator.step();

    EXPECT_EQ(*acc, 0b1111u);

    *pc = 0u;
    *acc = 0b1111u;
    emulator.step();

    EXPECT_EQ(*acc, 0b1111u);
}

TEST_F(EmulatorInstructionsTests, DCLTest) {
    rom[0] = ASM_DCL;
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
}

TEST_F(EmulatorInstructionsTests, LDMTest) {
    rom[0] = ASM_LDM | 0x0Fu;
    rom[1] = ASM_LDM | 0x02u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0x0Fu);
    EXPECT_EQ(*CY, 0u);

    emulator.step();

    EXPECT_EQ(*pc, 0x002u);
    EXPECT_EQ(*acc, 0x02u);
}

TEST_F(EmulatorInstructionsTests, LDTest) {
    rom[0] = ASM_LD | ASM_R2;
    registers[1] = 0x20;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0x20 : 0x00u);
    EXPECT_EQ(*acc, 0x02u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, XCHTest) {
    rom[0] = ASM_XCH | ASM_R2;
    *acc = 0x07u;
    registers[1] = 0x24;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0x74 : 0x00u);
    EXPECT_EQ(*acc, 0x02u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, ADDTest) {
    rom[0] = ASM_ADD | ASM_R2;
    *acc = 0x07u;
    registers[1] = 0x20u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0x20 : 0x00u);
    EXPECT_EQ(*acc, 0x02u + 0x07u);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    registers[1] = 0xF0u;
    emulator.step();

    EXPECT_EQ(*acc, (0x09u + 0x0Fu) & 0x0F);
    EXPECT_EQ(*CY, 1u);

    *pc = 0u;
    registers[1] = 0x20u;
    emulator.step();

    EXPECT_EQ(*acc, 0x08u + 0x02u + 1u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, SUBTest) {
    rom[0] = ASM_SUB | ASM_R2;
    *acc = 0x07u;
    registers[1] = 0x20u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0x20 : 0x00u);
    EXPECT_EQ(*acc, 0x07u - 0x02u);
    EXPECT_EQ(*CY, 1u);

    *pc = 0u;
    *CY = 0u;
    registers[1] = 0xF0u;
    emulator.step();

    EXPECT_EQ(*acc, (0x05u - 0x0Fu) & 0x0F);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    *CY = 1u;
    registers[1] = 0x30u;
    emulator.step();

    EXPECT_EQ(*acc, 0x06u - 0x03u - 1u);
    EXPECT_EQ(*CY, 1u);
}

TEST_F(EmulatorInstructionsTests, INCTest) {
    rom[0] = ASM_INC | ASM_R2;
    registers[1] = 0xE0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0xF0 : 0x00u);
    EXPECT_EQ(*acc, 0u);
    EXPECT_EQ(*CY, 0u);

    *pc = 0u;
    emulator.step();

    EXPECT_EQ(registers[1], 0x00u);
}

TEST_F(EmulatorInstructionsTests, BBLTest) {
    rom[0] = ASM_BBL | ASM_R2;
    registers[1] = 0x10u;
    stack[0] = 0x010u;
    *stackDepth = 1u;
    emulator.step();

    EXPECT_EQ(*pc, 0x010u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*stackDepth, 0u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0x10u : 0x00u);
    EXPECT_EQ(*acc, 1u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, JUNTest) {
    rom[0] = ASM_JUN | 0x01u;
    rom[1] = 0x42u;
    emulator.step();

    EXPECT_EQ(*pc, 0x142u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, JMSTest) {
    rom[0] = ASM_JMS | 0x01u;
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
    rom[0] = ASM_FIM | ASM_P1;
    rom[1] = 0x42;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1u ? 0x42u : 0x00u);
    EXPECT_EQ(*acc, 0u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, SRCTest) {
    rom[0] = ASM_SRC | ASM_P2;
    registers[2] = 0x42u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 2u ? 0x42u : 0x00u);
    EXPECT_EQ(*acc, 0u);
    EXPECT_EQ(*CY, 0u);
    EXPECT_EQ(*romSrcAddr, 0x42u);
    EXPECT_EQ(*ramSrcAddr, 0x42u);
}

TEST_F(EmulatorInstructionsTests, JCNTest) {
    rom[0] = ASM_JCN | ASM_CON_AEZ;
    rom[1] = 0x42u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *acc = 0x01u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);

    rom[0] = ASM_JCN | ASM_CON_ANZ;
    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *acc = 0x00u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);

    rom[0] = ASM_JCN | ASM_CON_CEZ;
    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *CY = 1u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);

    rom[0] = ASM_JCN | ASM_CON_CNZ;
    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *CY = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);

    rom[0] = ASM_JCN | ASM_CON_TEZ;
    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *test = 1u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);

    rom[0] = ASM_JCN | ASM_CON_TNZ;
    *pc = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);

    *pc = 0u;
    *test = 0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);
}

TEST_F(EmulatorInstructionsTests, FINTest) {
    rom[0] = ASM_FIN | ASM_P2;
    rom[0x42] = 0x21u;
    registers[0] = 0x42u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    EXPECT_EQ(registers[2], 0x21u);
}

TEST_F(EmulatorInstructionsTests, JINTest) {
    rom[0] = ASM_JIN | ASM_P1;
    registers[1] = 0x42u;
    emulator.step();

    EXPECT_EQ(*pc, 0x042u);
}

TEST_F(EmulatorInstructionsTests, ISZTest) {

}

// TODO: Make tests for instructions that operates on page boundary

// TODO: Make tests for these:
// constexpr uint8_t INS_FIN_MASK = 0x3E; - Skip until more rom
// constexpr uint8_t INS_JIN_MASK = 0x3F; - Skip until more rom
// constexpr uint8_t INS_ISZ_MASK = 0x7F; - Skip until more rom
