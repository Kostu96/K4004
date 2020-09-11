#include "K4001.hpp"
#include "K4004.hpp"
#include <iostream>
#include <string>
#include <stdio.h>

#include "assembler.hpp"

void printStuff(const Byte& bus, const mcs4::K4001& rom, const mcs4::K4004 cpu)
{
    const Byte* romData = rom.getRomContents();
    static const Byte numberOfColumns = 32;
    Word addr = 0x000;
    std::printf("ROM:                                                                                                 | Regs:\n");
    for (Word i = 0; i < rom.ROM_SIZE; ++i) {
        if (i % numberOfColumns == 0) {
            std::printf("%03x: ", addr);
            addr += static_cast<Word>(numberOfColumns);
        }
        
        printf("%02x ", romData[i]);

        if (i % numberOfColumns == numberOfColumns - 1)
            std::printf("| P%d: %02x\n", i / numberOfColumns, cpu.getRegisters()[i / numberOfColumns]);
    }

    std::printf("\nStack | SP | PC  | IR | Acc CY");
    std::printf("\n%03x   | %01x  | %03x | %02x | %01x   %01x",
        cpu.getStack().getStackContents()[0],
        cpu.getStack().getSP(),
        cpu.getPC(),
        cpu.getIR(),
        cpu.getAcc(),
        cpu.getCY());
    std::printf("\n%03x   |", cpu.getStack().getStackContents()[1]);
    std::printf("\n%03x   |", cpu.getStack().getStackContents()[2]);

    std::printf("\n");
}

int main()
{
    Byte sumOfTwo4BitNumbers[] = { 0x20, 0xA2, 0xA0, 0x81, 0xB1, 0x40, 0x05, };
    
    std::uint8_t* bytecode;
    std::size_t codeSize;
    Assembler::assemble("programs/4bit_addition.asm", bytecode, codeSize);

    for (size_t i = 0u; i < 7u; ++i)
        if (bytecode[i] != sumOfTwo4BitNumbers[i])
            std::cout << "Code doesn't match! Byte nr." << i << " compiled: " << bytecode[i] << " ref: " << sumOfTwo4BitNumbers[i] << '\n';

    Assembler::freeOutput(bytecode);

    using namespace mcs4;

    Byte intelAndRoutine[] = { 0x28, 0x00, 0x29, 0xEA, 0xB0, 0x68, 0x29, 0xEA, 0xB1, 0x50,
                               0x10, 0xB2, 0xE1, 0x40, 0x00, 0x00, 0xF0, 0xB2, 0xD4, 0xB0,
                               0xF6, 0xB0, 0x1A, 0x22, 0xB1, 0xF6, 0xB1, 0xB2, 0xF6, 0xB2,
                               0xF8, 0x1C, 0x13, 0xC0, 0xB1, 0xF6, 0xB1, 0xF1, 0x40, 0x1B, };

    Byte bus = 0x0;
    CycleType currentCycle = CycleType::A1;
    K4001 rom(0);
    rom.connect(&bus);
    rom.load(0x00, intelAndRoutine, sizeof(intelAndRoutine) / sizeof(Byte));
    K4004 cpu;
    cpu.connect(&bus);

    std::string command = "";
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
    }

    return 0;
}
