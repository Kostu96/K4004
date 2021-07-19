#include "shared/source/assembly.hpp"

uint8_t getOpcodeFromByte(uint8_t byte)
{
    uint8_t byteHP = (byte >> 4) & BITMASK_4BITS;

    if (byteHP == 0u || byteHP > 0xDu)
        return byteHP;

    if (byteHP == 2u || byteHP == 3u)
        return byte | 0x0Eu;

    return byte | BITMASK_4BITS;
}
