#include "emulator/source/emulator.hpp"

#include <gtest/gtest.h>

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
    static K4004& getCPU(Emulator& emulator) {
        return emulator.m_cpu;
    }
};

// TODO: Make these tests unaffected by rom loading
struct EmulatorInstructionsTests : public testing::Test {
    void SetUp() override {
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
    uint8_t* acc = nullptr;
    uint8_t* CY = nullptr;
    uint16_t* pc = nullptr;
    uint16_t* stack = nullptr;
    uint8_t* registers = nullptr;
};

TEST_F(EmulatorInstructionsTests, NOPTest) {
    const uint8_t prog[] = { 0x00 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

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
    const uint8_t prog[] = { 0xE0 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, WMPTest) {
    const uint8_t prog[] = { 0xE1 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, WRRTest) {
    const uint8_t prog[] = { 0xE2 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, WR0Test) {
    const uint8_t prog[] = { 0xE4 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, WR1Test) {
    const uint8_t prog[] = { 0xE5 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, WR2Test) {
    const uint8_t prog[] = { 0xE6 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, WR3Test) {
    const uint8_t prog[] = { 0xE7 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, SBMTest) {
    const uint8_t prog[] = { 0xE8 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, RDMTest) {
    const uint8_t prog[] = { 0xE9 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, RDRTest) {
    const uint8_t prog[] = { 0xEA };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, ADMTest) {
    const uint8_t prog[] = { 0xEB };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, RD0Test) {
    const uint8_t prog[] = { 0xEC };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, RD1Test) {
    const uint8_t prog[] = { 0xED };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, RD2Test) {
    const uint8_t prog[] = { 0xEE };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, RD3Test) {
    const uint8_t prog[] = { 0xEF };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, CLBTest) {
    const uint8_t prog[] = { 0xF0 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, CLCTest) {
    const uint8_t prog[] = { 0xF1 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, IACTest) {
    const uint8_t prog[] = { 0xF2 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, CMCTest) {
    const uint8_t prog[] = { 0xF3 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, CMATest) {
    const uint8_t prog[] = { 0xF4 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, RALTest) {
    const uint8_t prog[] = { 0xF5 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, RARTest) {
    const uint8_t prog[] = { 0xF6 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, TCCTest) {
    const uint8_t prog[] = { 0xF7 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, DACTest) {
    const uint8_t prog[] = { 0xF8 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, TCSTest) {
    const uint8_t prog[] = { 0xF9 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, STCTest) {
    const uint8_t prog[] = { 0xFA };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, DAATest) {
    const uint8_t prog[] = { 0xFB };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, KBPTest) {
    const uint8_t prog[] = { 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*CY, 0u);

    EXPECT_EQ(*pc, 0x001u);
    EXPECT_EQ(*acc, 0b0000u);

    *acc = 0b0001u;
    emulator.step();
    EXPECT_EQ(*pc, 0x002u);
    EXPECT_EQ(*acc, 0b0001u);

    *acc = 0b0010u;
    emulator.step();
    EXPECT_EQ(*pc, 0x003u);
    EXPECT_EQ(*acc, 0b0010u);

    *acc = 0b0100u;
    emulator.step();
    EXPECT_EQ(*pc, 0x004u);
    EXPECT_EQ(*acc, 0b0011u);

    *acc = 0b1000u;
    emulator.step();
    EXPECT_EQ(*pc, 0x005u);
    EXPECT_EQ(*acc, 0b0100u);

    *acc = 0b0011u;
    emulator.step();
    EXPECT_EQ(*pc, 0x006u);
    EXPECT_EQ(*acc, 0b1111u);

    *acc = 0b1111u;
    emulator.step();
    EXPECT_EQ(*pc, 0x007u);
    EXPECT_EQ(*acc, 0b1111u);
}

TEST_F(EmulatorInstructionsTests, DCLTest) {
    const uint8_t prog[] = { 0xFD };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    ASSERT_TRUE(false);
    /*EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    EXPECT_EQ(*acc, 0x00u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);*/
}

TEST_F(EmulatorInstructionsTests, LDMTest) {
    const uint8_t prog[] = { 0xDF, 0xD2 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    emulator.step();

    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], 0x00u);
    EXPECT_EQ(*CY, 0u);
    
    EXPECT_EQ(*pc, 0x001u);
    EXPECT_EQ(*acc, 0x0Fu);

    emulator.step();

    EXPECT_EQ(*pc, 0x002u);
    EXPECT_EQ(*acc, 0x02u);
}

TEST_F(EmulatorInstructionsTests, LDTest) {
    const uint8_t prog[] = { 0xA2 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    registers[1] = 0x20;
    emulator.step();

    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0x20 : 0x00u);
    EXPECT_EQ(*CY, 0u);

    EXPECT_EQ(*pc, 0x001u);
    EXPECT_EQ(*acc, 0x02u);
}

TEST_F(EmulatorInstructionsTests, XCHTest) {
    const uint8_t prog[] = { 0xB2 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    *acc = 0x07;
    registers[1] = 0x24;
    emulator.step();

    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0x74 : 0x00u);
    EXPECT_EQ(*CY, 0u);

    EXPECT_EQ(*pc, 0x001u);
    EXPECT_EQ(*acc, 0x02u);
}

TEST_F(EmulatorInstructionsTests, ADDTest) {
    const uint8_t prog[] = { 0x82, 0x82, 0x82 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    // Add
    *acc = 0x07u;
    registers[1] = 0x20u;
    emulator.step();

    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0x20 : 0x00u);

    EXPECT_EQ(*pc, 0x001u);
    EXPECT_EQ(*acc, 0x02u + 0x07u);
    EXPECT_EQ(*CY, 0u);

    // Add with overflow
    registers[1] = 0xF0u;
    emulator.step();
    EXPECT_EQ(*pc, 0x002u);
    EXPECT_EQ(*acc, (0x09u + 0x0Fu) & 0x0F);
    EXPECT_EQ(*CY, 1u);

    // Add with carry
    registers[1] = 0x20u;
    emulator.step();
    EXPECT_EQ(*pc, 0x003u);
    EXPECT_EQ(*acc, 0x08u + 0x02u + 1u);
    EXPECT_EQ(*CY, 0u);
}

TEST_F(EmulatorInstructionsTests, SUBTest) {
    const uint8_t prog[] = { 0x92, 0x92, 0x92 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    // Sub
    *acc = 0x07u;
    registers[1] = 0x20u;
    emulator.step();

    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0x20 : 0x00u);

    EXPECT_EQ(*pc, 0x001u);
    EXPECT_EQ(*acc, 0x07u - 0x02u);
    EXPECT_EQ(*CY, 1u);

    // Sub with overflow
    *CY = 0u;
    registers[1] = 0xF0u;
    emulator.step();
    EXPECT_EQ(*pc, 0x002u);
    EXPECT_EQ(*acc, (0x05u - 0x0Fu) & 0x0F);
    EXPECT_EQ(*CY, 0u);

    // Sub with borrow
    *CY = 1u;
    registers[1] = 0x30u;
    emulator.step();
    EXPECT_EQ(*pc, 0x003u);
    EXPECT_EQ(*acc, 0x06u - 0x03u - 1u);
    EXPECT_EQ(*CY, 1u);
}

TEST_F(EmulatorInstructionsTests, INCTest) {
    const uint8_t prog[] = { 0x62, 0x62 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

    // Inc
    registers[1] = 0xE0u;
    emulator.step();

    EXPECT_EQ(*pc, 0x001u);
    for (uint8_t i = 0; i < 3u; ++i)
        EXPECT_EQ(stack[i], 0x000u);
    for (uint8_t i = 0; i < 8u; ++i)
        EXPECT_EQ(registers[i], i == 1 ? 0xF0 : 0x00u);
    EXPECT_EQ(*acc, 0u);
    EXPECT_EQ(*CY, 0u);

    // Inc with overflow
    emulator.step();
    EXPECT_EQ(*pc, 0x002u);
    EXPECT_EQ(registers[1], 0x00u);
}

TEST_F(EmulatorInstructionsTests, BBLTest) {
    const uint8_t prog[] = { 0xC2 };
    const size_t size = sizeof(prog) / sizeof(uint8_t);
    emulator.loadProgram(prog, size);

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

/* TODO: Make tests for these:
constexpr uint8_t INS_JCN_MASK = 0x1F;
constexpr uint8_t INS_FIM_MASK = 0x2E;
constexpr uint8_t INS_SRC_MASK = 0x2F;
constexpr uint8_t INS_FIN_MASK = 0x3E;
constexpr uint8_t INS_JIN_MASK = 0x3F;
constexpr uint8_t INS_JUN_MASK = 0x4F;
constexpr uint8_t INS_JMS_MASK = 0x5F;
constexpr uint8_t INS_ISZ_MASK = 0x7F;
constexpr uint8_t INS_LD_MASK = 0xAF;
constexpr uint8_t INS_BBL_MASK = 0xCF;*/
