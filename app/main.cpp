#include "olcPixelGameEngine.h"
#include "MCS4.hpp"

class App :
    public olc::PixelGameEngine
{
public:
    App() {
        sAppName = "Intel4004 Emulator";
    }
private:
    bool OnUserCreate() override
    {
        emulator.loadProgram("programs/4bit_addition.asm");
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::DARK_CYAN);
        printROM();
        return true;
    }

    void printROM() {
        constexpr Byte numberOfColumns = 16;
        static char buf[1 + 3 + numberOfColumns * (2 + 1 + 1)];
        DrawString({ 1, 1 }, "ROM", olc::BLACK);
        auto rom = emulator.getROMData();
        auto romSize = emulator.getROMSize();
        Word addr = 0x000;
        for (Word i = 0; i < romSize; ++i) {
            if (i % numberOfColumns == 0) {
                std::sprintf(buf, "%03x:", addr);
                DrawString({ 1, 10 * (i / numberOfColumns + 1) }, buf, olc::BLACK);
                addr += static_cast<Word>(numberOfColumns);
            }
            std::sprintf(buf, "%02x", rom[i]);
            DrawString({ 20 * (i % numberOfColumns + 1) + 15, 10 * (i / numberOfColumns + 1) }, buf, olc::BLACK);
        }
    }

    mcs4::MCS4 emulator;
};

//void printStuff(const Byte& bus, const mcs4::ROM& rom, const mcs4::K4004 cpu)
//{
//    const Byte* romData = rom.getRomContents();
//    static const Byte numberOfColumns = 32;
//    Word addr = 0x000;
//    std::printf("ROM:                                                                                                 | Regs:\n");
//    for (Word i = 0; i < rom.ROM_SIZE; ++i) {
//        if (i % numberOfColumns == 0) {
//            std::printf("%03x: ", addr);
//            addr += static_cast<Word>(numberOfColumns);
//        }
//        
//        printf("%02x ", romData[i]);
//
//        if (i % numberOfColumns == numberOfColumns - 1)
//            std::printf("| P%d: %02x\n", i / numberOfColumns, cpu.getRegisters()[i / numberOfColumns]);
//    }
//
//    std::printf("\nStack | SP | PC  | IR | Acc CY");
//    std::printf("\n%03x   | %01x  | %03x | %02x | %01x   %01x",
//        cpu.getStack().getStackContents()[0],
//        cpu.getStack().getSP(),
//        cpu.getPC(),
//        cpu.getIR(),
//        cpu.getAcc(),
//        cpu.getCY());
//    std::printf("\n%03x   |", cpu.getStack().getStackContents()[1]);
//    std::printf("\n%03x   |", cpu.getStack().getStackContents()[2]);
//
//    std::printf("\n");
//}

int main()
{
    App app;
    if (app.Construct(640, 360, 2, 2))
        app.Start();

    return 0;

    /*Byte sumOfTwo4BitNumbers[] = { 0x20, 0xA2, 0xA0, 0x81, 0xB1, 0x40, 0x05, };
    Byte intelAndRoutine[] = { 0x28, 0x00, 0x29, 0xEA, 0xB0, 0x68, 0x29, 0xEA, 0xB1, 0x50,
                               0x10, 0xB2, 0xE1, 0x40, 0x00, 0x00, 0xF0, 0xB2, 0xD4, 0xB0,
                               0xF6, 0xB0, 0x1A, 0x22, 0xB1, 0xF6, 0xB1, 0xB2, 0xF6, 0xB2,
                               0xF8, 0x1C, 0x13, 0xC0, 0xB1, 0xF6, 0xB1, 0xF1, 0x40, 0x1B, };
    
    std::uint8_t* bytecode;
    std::size_t codeSize;
    Assembler::assemble("programs/4bit_addition.asm", bytecode, codeSize);

    for (size_t i = 0u; i < 7u; ++i)
        if (bytecode[i] != sumOfTwo4BitNumbers[i])
            std::cout << "Code doesn't match! Byte nr." << i << " compiled: " << bytecode[i] << " ref: " << sumOfTwo4BitNumbers[i] << '\n';

    Assembler::freeOutput(bytecode);*/

    using namespace mcs4;

    /*std::string command = "";
    unsigned int value;
    while (command != "quit")
    {
        std::system("cls");
        printStuff(bus, rom, cpu);
        
        std::cout << "\n$> ";
        std::cin >> command;
        std::cin.get();

        if (command == "n") {
            value = 8;
            while (value--) {
                switch (currentCycle)
                {
                case CycleType::A1:
                case CycleType::A2:
                case CycleType::A3:
                    cpu.cycle(currentCycle);
                    rom.cycle(currentCycle);
                    break;
                case CycleType::M1:
                case CycleType::M2:
                    rom.cycle(currentCycle);
                    cpu.cycle(currentCycle);
                    break;
                case CycleType::X1:
                case CycleType::X2:
                case CycleType::X3:
                    cpu.cycle(currentCycle);
                    break;
                }
                ++currentCycle;
            }
        }
    }*/
}
