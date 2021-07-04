#include "app.hpp"

int main()
{
    App app;
    if (app.Construct(640, 360, 2, 2))
        app.Start();
    else
        return -1;

    return 0;
}

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
