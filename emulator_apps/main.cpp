#include <emulator_core/source/rom.hpp>
#include <emulator_core/source/ram.hpp>
#include <emulator_core/source/K4004.hpp>

class MCS4 {
public:
    MCS4() : m_cpu{ m_ROM, m_RAM } {}

    void clock() {
        m_cpu.clock();
    }
private:
    ROM m_ROM;
    RAM m_RAM;
    K4004 m_cpu;
};

int main()
{
    MCS4 app;

    while (true)
        app.clock();

    return 0;
}


// ideas for demo system:
// character rom to save on ram
// 8x8 sprites
// display? char mode | graphics mode?

// ROM memory map:

// RAM memory map:
