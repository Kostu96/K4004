#include "emulator/source/emulator.hpp"

#include <gtest/gtest.h>

template<>
struct WhiteBox<ROM> {
    static uint8_t* getROMData(ROM& rom) {
        return rom.m_rom;
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

    static uint16_t* getPC(K4004& cpu) {
        return &cpu.m_PC;
    }

    static uint16_t* getStack(K4004& cpu) {
        return cpu.m_stack;
    }

    static uint8_t* gerRegisters(K4004& cpu) {
        return cpu.m_registers;
    }
};

template<>
struct WhiteBox<Emulator> {
    static ROM& getROM(Emulator& emulator) {
        return emulator.m_rom;
    }

    static K4004& getCPU(Emulator& emulator) {
        return emulator.m_cpu;
    }
};

struct EmulatorInstructionsTests : public testing::Test {
    void SetUp() override {
        rom = WhiteBox<ROM>::getROMData(WhiteBox<Emulator>::getROM(emulator));
        ASSERT_NE(rom, nullptr);

        auto& cpu = WhiteBox<Emulator>::getCPU(emulator);
        acc = WhiteBox<K4004>::getAcc(cpu);
        CY = WhiteBox<K4004>::getCY(cpu);
        pc = WhiteBox<K4004>::getPC(cpu);
        stack = WhiteBox<K4004>::getStack(cpu);
        registers = WhiteBox<K4004>::gerRegisters(cpu);
        ASSERT_NE(acc, nullptr);
        ASSERT_NE(pc, nullptr);
        ASSERT_NE(stack, nullptr);
        ASSERT_NE(registers, nullptr);
    }

    void TearDown() override {
        emulator.reset();
    }

    Emulator emulator;
    uint8_t* rom = nullptr;
    uint8_t* acc = nullptr;
    uint8_t* CY = nullptr;
    uint16_t* pc = nullptr;
    uint16_t* stack = nullptr;
    uint8_t* registers = nullptr;
};

TEST_F(EmulatorInstructionsTests, NOPTest) {
    rom[0] = 0x00;
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
    rom[0] = 0xE0;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, WMPTest) {
    rom[0] = 0xE1;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, WRRTest) {
    rom[0] = 0xE2;
    emulator.step();

    // TODO: Add ROM I/Os and more ROM first
}

TEST_F(EmulatorInstructionsTests, WR0Test) {
    rom[0] = 0xE4;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, WR1Test) {
    rom[0] = 0xE5;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, WR2Test) {
    rom[0] = 0xE6;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, WR3Test) {
    rom[0] = 0xE7;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, SBMTest) {
    rom[0] = 0xE8;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, RDMTest) {
    rom[0] = 0xE9;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, RDRTest) {
    rom[0] = 0xEA;
    emulator.step();

    // TODO: Add ROM I/Os and more ROM first
}

TEST_F(EmulatorInstructionsTests, ADMTest) {
    rom[0] = 0xEB;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, RD0Test) {
    rom[0] = 0xEC;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, RD1Test) {
    rom[0] = 0xED;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, RD2Test) {
    rom[0] = 0xEE;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, RD3Test) {
    rom[0] = 0xEF;
    emulator.step();

    // TODO: Add RAM first
}

TEST_F(EmulatorInstructionsTests, CLBTest) {
    rom[0] = 0xF0;
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
    rom[0] = 0xF1;
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
    rom[0] = 0xF2;
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
    rom[0] = 0xF3;
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
    rom[0] = 0xF4;
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
    rom[0] = 0xF5;
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
    rom[0] = 0xF6;
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
    rom[0] = 0xF7;
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
    rom[0] = 0xF8;
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
    rom[0] = 0xF9;
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
    rom[0] = 0xFA;
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
    rom[0] = 0xFB;
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
    rom[0] = 0xFC;
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
    rom[0] = 0xFD;
    emulator.step();

    // TODO: Add ram first
}

TEST_F(EmulatorInstructionsTests, LDMTest) {
    rom[0] = 0xDF;
    rom[1] = 0xD2;
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
    rom[0] = 0xA2;
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
    rom[0] = 0xB2;
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
    rom[0] = 0x82;
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
    rom[0] = 0x92;
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
    rom[0] = 0x62;
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
    rom[0] = 0xC2;
    registers[1] = 0x10u;
    stack[0] = 0x010u;
    emulator.step();

    EXPECT_EQ(*pc, 0x010u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0x10u : 0x00u);
    EXPECT_EQ(*acc, 1u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, JUNTest) {
    rom[0] = 0x41;
    rom[1] = 0x42;
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
    rom[0] = 0x51;
    rom[1] = 0x42;
    emulator.step();

    EXPECT_EQ(*pc, 0x142u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], i == 0 ? 0x002u : 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*acc, 0u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, FIMTest) {
    rom[0] = 0x24;
    rom[1] = 0x42;
    emulator.step();

    EXPECT_EQ(*pc, 0x002u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 2u ? 0x42u : 0x00u);
    EXPECT_EQ(*acc, 0u);
    EXPECT_EQ(*CY, 0u);
}

// TODO: Make tests for these:
// constexpr uint8_t INS_JCN_MASK = 0x1F; - Skip until test pin and more rom
// constexpr uint8_t INS_SRC_MASK = 0x2F; - Skip until ram and more rom
// constexpr uint8_t INS_FIN_MASK = 0x3E; - Skip until more rom
// constexpr uint8_t INS_JIN_MASK = 0x3F; - Skip until more rom
// constexpr uint8_t INS_ISZ_MASK = 0x7F; - Skip until more rom
