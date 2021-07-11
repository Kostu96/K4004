#include "app/app.hpp"

bool App::OnUserCreate()
{
    emulator.loadProgram("programs/4bit_addition.asm");
    return true;
}

bool App::OnUserUpdate(float fElapsedTime)
{
    Clear(olc::DARK_CYAN);
    printROM();
    //printStack();*/
    return true;
}

void App::printROM()
{
    auto rom = emulator.getROMData();
    constexpr auto romSize = ROM::ROM_SIZE;
    constexpr Byte numberOfColumns = 8;
    constexpr Byte numberOfRows = romSize / numberOfColumns;
    static char buf[1 + 3 + numberOfColumns * 3 + numberOfRows * 5];
    char* ptr = buf;

    ptr += std::sprintf(ptr, "ROM");
    Word addr = 0x000;
    for (Word i = 0; i < romSize; ++i) {
        if (i % numberOfColumns == 0) {
            ptr += std::sprintf(ptr, "\n%03x:", addr);
            addr += static_cast<Word>(numberOfColumns);
        }
        ptr += std::sprintf(ptr, " %02x", rom[i]);
    }

    DrawStringDecal({ 1, 1 }, buf, olc::BLACK);
}

void App::printStack()
{

}
