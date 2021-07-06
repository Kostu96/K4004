#include "assembler/source/conversion_functions.hpp"

#include <string.h>

bool hexStrToUint8(const char* hexStr, uint16_t& value)
{
    size_t length = strnlen_s(hexStr, 5);
    if (length == 5 || length == 0)
        return false; // String representation of a to big number or empty

    value = 0;
    for (size_t i = 0; i < length; ++i) {
        if (hexStr[i] >= '0' && hexStr[i] <= '9')
            value += (1 << 4 * (length - i - 1)) * (hexStr[i] - '0');
        else if (hexStr[i] >= 'a' && hexStr[i] <= 'f')
            value += (1 << 4 * (length - i - 1)) * (hexStr[i] - 'a' + 10);
        else if (hexStr[i] >= 'A' && hexStr[i] <= 'F')
            value += (1 << 4 * (length - i - 1)) * (hexStr[i] - 'A' + 10);
        else
            return false; // Illformed bin number
    }

    return true;
}

bool binStrToUint8(const char* binStr, uint16_t& value)
{
    size_t length = strnlen_s(binStr, 17);
    if (length == 17 || length == 0)
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

bool octStrToUint8(const char* octStr, uint16_t& value)
{
    size_t length = strnlen_s(octStr, 7);
    if (length == 7 || length == 0)
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
