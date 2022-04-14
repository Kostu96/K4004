#include "emulator_core/source/ram.hpp"

#include <cstring>

RAM::RAM()
{
    reset();
}

void RAM::reset()
{
    m_srcAddress = 0u;
    std::memset(m_ram, 0, RAM_SIZE);
    std::memset(m_status, 0, STATUS_SIZE);
    std::memset(m_oPorts, 0, OUTPUT_SIZE);
}

void RAM::writeRAM(uint8_t character)
{
    m_ram[m_srcAddress] = character & 0x0Fu;
}

void RAM::writeStatus(uint8_t character, uint8_t index)
{
    uint8_t addr = ((m_srcAddress >> 2) & 0x3Cu) | (index & 0x03u);
    m_status[addr] = character & 0x0Fu;
}

uint8_t RAM::readStatus(uint8_t index) const
{
    uint8_t addr = ((m_srcAddress >> 2) & 0x3Cu) | (index & 0x03u);
    return m_status[addr] & 0x0F;
}

void RAM::writeOutputPort(uint8_t character)
{
    uint8_t addr = m_srcAddress >> 6;
    m_oPorts[addr] = character & 0x0Fu;
}

uint8_t RAM::readOutputPort() const
{
    uint8_t addr = m_srcAddress >> 6;
    return m_oPorts[addr] & 0x0F;
}

void RAM::setRAMBank(uint8_t index)
{
    m_srcAddress &= 0x00FFu;
    m_srcAddress |= static_cast<uint16_t>(index) << 8;
}

void RAM::writeSrcAddress(uint8_t address)
{
    m_srcAddress &= 0x0300u;
    m_srcAddress |= address;
}
