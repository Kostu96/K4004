#include "app/app.hpp"

#include "assembler/source/assembler.hpp"

#include <sstream>
#include <bitset>

bool App::OnUserCreate()
{
    Assembler assembler;
    std::vector<uint8_t> bytecode;
    assembler.assemble("programs/mcs4_evaluation.asm", bytecode);
    emulator.loadProgram(bytecode.data(), bytecode.size());
    assembler.disassemble(bytecode, disassembly);
    return true;
}

bool App::OnUserUpdate(float fElapsedTime)
{
    if (GetKey(olc::SPACE).bPressed)
        emulator.step();

    if (GetKey(olc::R).bPressed)
        emulator.reset();

    Clear(olc::DARK_CYAN);
    printROM();
    printCPU();
    printRAM();
    return true;
}

void App::printROM()
{
    auto PC = emulator.getCPU().getPC();
    auto rom = emulator.getROM().getRomContents();
    constexpr uint8_t numberOfColumns = 8;
    std::stringstream ss;
    ss << std::hex << std::uppercase;
    
    uint16_t page = PC / ROM::PAGE_SIZE;
    ss << "-ROM- PAGE: " << page << '\n';
    page <<= 8;
    uint16_t addr = 0x000;
    ss << std::setfill('0');
    for (uint16_t i = 0; i < ROM::PAGE_SIZE; ++i) {
        if (i % numberOfColumns == 0) {
            ss << '\n' << std::setw(2) << addr << ':';
            addr += static_cast<uint16_t>(numberOfColumns);
        }
        ss << ' ' << std::setw(2) << +rom[page | i];
    }

    size_t PCnext = PC + 1ull;
    int pageIdx = PC % ROM::PAGE_SIZE;
    float xpos = 33 + (pageIdx % 8 * 24);
    float ypos = 17 + (pageIdx / 8 * 8);

    FillRectDecal({ xpos, ypos }, { 17, 9 });

    bool isTwobyte = PCnext < disassembly.size() ? disassembly[PCnext].empty() : false;
    if (isTwobyte) {
        bool isNewLine = (pageIdx + 1) % numberOfColumns == 0;
        float width = isNewLine ? 17 : 24; // 24
        xpos = isNewLine ? 33 : xpos + 17;
        ypos = isNewLine ? ypos + 8 : ypos;

        FillRectDecal({ xpos, ypos }, { width, 9 });
    }

    ss << "\n\n";
    ss << "SRC Address: " << +emulator.getROM().getSrcAddress();

    DrawStringDecal({ 2, 2 }, ss.str(), olc::BLACK);
}

void App::printCPU()
{
    auto& cpu = emulator.getCPU();
    auto rom = emulator.getROM().getRomContents();
    auto PC = cpu.getPC();
    auto stack = cpu.getStack();
    auto registers = cpu.getRegisters();
    std::stringstream ss;

    ss << "-CPU-\n";
    ss << '\n';
    ss << std::setfill('0') << std::hex << std::uppercase;
    ss << "PC " << std::setw(3) << PC       << "  Acc: " << std::setw(2) << +cpu.getAcc() << '\n';
    ss << "L1 " << std::setw(3) << stack[0] << "  CY:   " << +cpu.getCY() << '\n';
    ss << "L2 " << std::setw(3) << stack[1] << "  Test: " << +cpu.getTest() << '\n';
    ss << "L3 " << std::setw(3) << stack[2] << '\n';
    ss << '\n';

    ss << "R0R1 " << std::setw(2) << +registers[0] << " R8R9 " << std::setw(2) << +registers[4] << '\n';
    ss << "R2R3 " << std::setw(2) << +registers[1] << " RARB " << std::setw(2) << +registers[5] << '\n';
    ss << "R4R5 " << std::setw(2) << +registers[2] << " RCRD " << std::setw(2) << +registers[6] << '\n';
    ss << "R6R7 " << std::setw(2) << +registers[3] << " RERF " << std::setw(2) << +registers[7] << '\n';
    ss << '\n';
    
    size_t PCnext = PC + 1ull;
    ss << "Curr.Ins.:\n";
    ss << (PCnext < disassembly.size() ? disassembly[PC] : "ERROR!") << "\n\n";

    DrawStringDecal({ 232, 2 }, ss.str(), olc::BLACK);
}

void App::printRAM()
{
    auto& ramObj = emulator.getRAM();
    auto ram = ramObj.getRamContents();
    auto status = ramObj.getStatusContents();
    auto output = ramObj.getOutputContents();
    std::stringstream ss;

    uint16_t bank = 0u;
    ss << "-RAM- BANK: 0\n";
    uint16_t addr = 0x000;
    ss << std::hex;
    for (uint16_t i = 0; i < RAM::NUM_RAM_CHIPS; ++i) {
        addr |= i << 6;
        ss << '\n';
        ss << "CHIP" << i << ": Main:            Status: Output:\n";
        for (uint16_t j = 0; j < RAM::NUM_RAM_REGS; ++j) {
            addr |= j << 4;
            ss << " Reg" << j << ": ";
            for (uint16_t k = 0; k < RAM::NUM_REG_CHARS; ++k) {
                addr |= k;
                ss << +ram[addr];
                addr &= 0x3F0u;
            }
            ss << "  ";
            addr >>= 2;
            for (uint16_t k = 0; k < RAM::NUM_STAUS_CHARS; ++k) {
                addr |= k;
                ss << +status[addr];
                addr &= 0x0FCu;
            }
            addr <<= 2;
            if (j == 0) ss << "    " << std::bitset<4>(output[i]);
            ss << '\n';
            addr &= 0x3C0u;
        }
        addr &= 0x0300u;
    }

    DrawStringDecal({ 370, 2 }, ss.str(), olc::BLACK);
}
