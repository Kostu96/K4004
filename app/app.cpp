#include "app/app.hpp"

#include <sstream>

bool App::OnUserCreate()
{
    emulator.loadProgram("programs/4bit_addition.asm");
    return true;
}

bool App::OnUserUpdate(float fElapsedTime)
{
    Clear(olc::DARK_CYAN);
    printROM();
    printCPU();
    return true;
}

void App::printROM()
{
    auto rom = emulator.getROMData();
    constexpr uint8_t numberOfColumns = 8;
    std::stringstream ss;
    
    ss << "-ROM-\n";
    uint16_t addr = 0x000;
    ss << std::setfill('0') << std::hex;
    for (uint16_t i = 0; i < ROM::ROM_SIZE; ++i) {
        if (i % numberOfColumns == 0) {
            ss << '\n' << std::setw(3) << addr << ':';
            addr += static_cast<uint16_t>(numberOfColumns);
        }
        ss << ' ' << std::setw(2) << +rom[i];
    }

    DrawStringDecal({ 1, 1 }, ss.str(), olc::BLACK);
}

void App::printCPU()
{
    auto& cpu = emulator.getCPU();
    auto stack = cpu.getStack();
    auto registers = cpu.getRegisters();
    std::stringstream ss;

    ss << "-CPU-\n";
    ss << '\n';
    ss << "Stack:    Registers:\n";
    ss << std::setfill('0') << std::hex;
    ss << "SP " << std::setw(3) << cpu.getPC() << "    R0R1 " << std::setw(2) << +registers[0] << " R8R9 " << std::setw(2) << +registers[4] << '\n';
    ss << "L1 " << std::setw(3) << stack[0]    << "    R2R3 " << std::setw(2) << +registers[1] << " RARB " << std::setw(2) << +registers[5] << '\n';
    ss << "L2 " << std::setw(3) << stack[1]    << "    R4R5 " << std::setw(2) << +registers[2] << " RCRD " << std::setw(2) << +registers[6] << '\n';
    ss << "L3 " << std::setw(3) << stack[2]    << "    R6R7 " << std::setw(2) << +registers[3] << " RERF " << std::setw(2) << +registers[7] << '\n';
    ss << '\n';
    ss << "Acc: " << std::setw(2) << +cpu.getAcc() << '\n';
    ss << "CY: " << +cpu.getCY();

    DrawStringDecal({ 250, 1 }, ss.str(), olc::BLACK);
}
