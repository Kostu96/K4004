#include "app.hpp"

int main()
{
    App app;
    if (app.Construct(640, 360, 2, 2, false, true))
        app.Start();
    else
        return -1;

    return 0;
}

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
