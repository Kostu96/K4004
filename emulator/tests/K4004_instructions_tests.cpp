#include "emulator/source/emulator.hpp"

#include <gtest/gtest.h>

template<>
struct WhiteBox<K4004> {
    static uint8_t* getAcc(K4004& cpu) {
        return &cpu.m_Acc;
    }

    static uint16_t* getPC(K4004& cpu) {
        return &cpu.m_PC;
    }

    static uint8_t* gerRegisters(K4004& cpu) {
        return cpu.m_registers;
    }

    static constexpr uint8_t getRegistersSize() { return K4004::REGISTERS_SIZE; }
};

template<>
struct WhiteBox<Emulator> {
    static K4004& getCPU(Emulator& emulator) {
        return emulator.m_cpu;
    }
};

struct EmulatorInstructionsTests : public testing::Test {
    void SetUp() override {
        auto& cpu = WhiteBox<Emulator>::getCPU(emulator);
        acc = WhiteBox<K4004>::getAcc(cpu);
        pc = WhiteBox<K4004>::getPC(cpu);
        registers = WhiteBox<K4004>::gerRegisters(cpu);

        ASSERT_NE(acc, nullptr);
    }

    Emulator emulator;
    uint8_t* acc = nullptr;
    uint16_t* pc = nullptr;
    uint8_t* registers = nullptr;
};

TEST_F(EmulatorInstructionsTests, NOPTest) {
    const uint8_t nopIns[] = { 0x00 };
    const size_t size = sizeof(nopIns) / sizeof(uint8_t);
    emulator.loadProgram(nopIns, size);

    emulator.step(size);

    EXPECT_EQ(*pc, 0x001u);
    EXPECT_EQ(*acc, 0x00u);
    constexpr uint8_t regSize = WhiteBox<K4004>::getRegistersSize();
    for (uint8_t i = 0; i < regSize; ++i)
        EXPECT_EQ(registers[i], 0x00u);
}
