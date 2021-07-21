#include "shared/source/assembly.hpp"

uint8_t getOpcodeFromByte(uint8_t byte)
{
    uint8_t byteHP = byte >> 4;

    if (byteHP == 0u || byteHP > 0xDu)
        return byte;

    if (byteHP == 2u || byteHP == 3u)
        return byte & 0xF1u;

    return byte & 0xF0u;
}
