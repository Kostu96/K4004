#include "assembler/source/conversion_functions.hpp"

#include <string.h>

bool hexStrToUint8(const char* hexStr, uint8_t& value)
{
    size_t length = strnlen_s(hexStr, 3);
    if (length == 3 || length == 0)
        return false; // String representation of a to big number or empty

    if (hexStr[0] >= '0' && hexStr[0] <= '9')
        value = 16 * (hexStr[0] - '0');
    else if (hexStr[0] >= 'A' && hexStr[0] <= 'F')
        value = 16 * (hexStr[0] - 'A' + 10);
    else if (hexStr[0] >= 'a' && hexStr[0] <= 'f')
        value = 16 * (hexStr[0] - 'a' + 10);
    else {
        // Error: Illformed hex number
        return false;
    }

    if (hexStr[1] >= '0' && hexStr[1] <= '9')
        value += hexStr[1] - '0';
    else if (hexStr[1] >= 'A' && hexStr[1] <= 'F')
        value += hexStr[1] - 'A' + 10;
    else if (hexStr[1] >= 'a' && hexStr[1] <= 'f')
        value += hexStr[1] - 'a' + 10;
    else {
        // Error: Illformed hex number
        return false;
    }

    return true;
}

bool binStrToUint8(const char* binStr, uint8_t& value)
{
    size_t length = strnlen_s(binStr, 9);
    if (length == 9 || length == 0)
        return false; // String representation of a to big number or empty

    value = 0;
    for (size_t i = 0; i < length; ++i) {
        if (binStr[i] == '1')
            value += 1 << (length - i - 1);
        else if (binStr[i] != '0')
            return false; // Illformed bin number
    }

    return true;
}

bool octStrToUint8(const char* octStr, uint8_t& value)
{
    size_t length = strnlen_s(octStr, 4);
    if (length == 4 || length == 0)
        return false; // String representation of a to big number or empty

    value = 0;
    for (size_t i = 0; i < length; ++i) {
        if (octStr[i] >= '0' && octStr[i] <= '7')
            value += (1 << 3 * (length - i - 1)) * (octStr[i] - '0');
        else
            return false; // Illformed bin number
    }

    return true;
}
