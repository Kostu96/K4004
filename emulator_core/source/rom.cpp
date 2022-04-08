#include "emulator/source/rom.hpp"

#include <cstring>

ROM::ROM()
{
    reset();
}

bool ROM::load(const uint8_t* objectCode, size_t objectCodeLength)
{   
    if (objectCode == nullptr || objectCodeLength == 0)
        return false;

     // Start of the I/O metal mask section
    if (objectCode[0] != 0xFEu)
        return false;

    size_t i = 1;
    uint8_t chipNumber;
    while (objectCode[i] != 0xFFu) {
        chipNumber = objectCode[i++];

        if (i >= objectCodeLength)
            return false; // Somethin went terribly wrong

        // Do not support more than NUM_ROM_CHIPS (32 in future) rom chips for now
        if (chipNumber >= NUM_ROM_CHIPS)
            return false;

        m_ioPortsMasks[chipNumber] = objectCode[i++];

        if (i >= objectCodeLength)
            return false; // Somethin went terribly wrong
    }
    ++i;

    if (objectCodeLength - i > ROM_SIZE)
        return false;

    for (size_t j = 0; i < objectCodeLength; ++i, ++j)
        m_rom[j] = objectCode[i];

    return true;
}

void ROM::reset()
{
    m_srcAddress = 0u;
    std::memset(m_rom, 0, ROM_SIZE);
    std::memset(m_ioPorts, 0, NUM_ROM_CHIPS);
    std::memset(m_ioPortsMasks, 0, NUM_ROM_CHIPS);
}

void ROM::writeIOPort(uint8_t value)
{
    uint8_t oldValue = m_ioPorts[m_srcAddress];
    uint8_t mask = m_ioPortsMasks[m_srcAddress];
    uint8_t newValue = 0u;
    newValue |= ((mask & 1 << 0) ? oldValue : value) & 1 << 0;
    newValue |= ((mask & 1 << 2) ? oldValue : value) & 1 << 1;
    newValue |= ((mask & 1 << 4) ? oldValue : value) & 1 << 2;
    newValue |= ((mask & 1 << 6) ? oldValue : value) & 1 << 3;
    
    m_ioPorts[m_srcAddress] = newValue;
}

uint8_t ROM::readIOPort() const
{
    uint8_t portValue = m_ioPorts[m_srcAddress];
    uint8_t mask = m_ioPortsMasks[m_srcAddress];
    uint8_t returnValue = 0u;
    returnValue |= ((mask & 1 << 0) ? portValue : (mask >> 1 & 1)) & 1 << 0;
    returnValue |= ((mask & 1 << 2) ? portValue : (mask >> 2 & 1)) & 1 << 1;
    returnValue |= ((mask & 1 << 4) ? portValue : (mask >> 3 & 1)) & 1 << 2;
    returnValue |= ((mask & 1 << 6) ? portValue : (mask >> 4 & 1)) & 1 << 3;
    
    return returnValue;
}
