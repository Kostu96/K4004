#include "emulator/source/ram.hpp"

#include <cstring>

RAM::RAM()
{
    reset();
}

void RAM::reset()
{
    m_srcAddress = 0u;
    std::memset(m_ram, 0, RAM_SIZE);
}
