#include "assembler/source/conversions.hpp"

#include <cassert>

unsigned int textToHex(const std::string_view& str)
{
    size_t length = str.size();
    assert(length > 0 && length <= sizeof(unsigned int) * 2);

    unsigned int value = 0;
    for (size_t i = 0; i < length; ++i) {
        if (str[i] >= '0' && str[i] <= '9')
            value += (1 << 4 * (length - i - 1)) * (str[i] - '0');
        else if (str[i] >= 'a' && str[i] <= 'f')
            value += (1 << 4 * (length - i - 1)) * (str[i] - 'a' + 10);
        else if (str[i] >= 'A' && str[i] <= 'F')
            value += (1 << 4 * (length - i - 1)) * (str[i] - 'A' + 10);
        else
            assert(false && "Error");
    }

    return value;
}

unsigned int textToBin(const std::string_view& str)
{
    size_t length = str.size();
    assert(length > 0 && length <= sizeof(unsigned int) * 8);

    unsigned int value = 0;
    for (size_t i = 1; i < length; ++i) {
        if (str[i] == '1')
            value += 1 << (length - i - 1);
        else if (str[i] != '0')
            assert(false);
    }

    return value;
}

unsigned int textToOct(const std::string_view& str)
{
    size_t length = str.size();
    assert(length > 0 && length <= sizeof(unsigned int) * 3);

    unsigned int value = 0;
    for (size_t i = 0; i < length; ++i) {
        if (str[i] >= '0' && str[i] <= '7')
            value += (1 << 3 * (length - i - 1)) * (str[i] - '0');
        else
            assert(false);
    }

    return value;
}

unsigned int textToDec(const std::string_view& str)
{
    size_t length = str.size();
    assert(length > 0 && length <= 10);

    unsigned int value = 0;
    uint16_t base = 1;
    for (size_t i = length; i > 0; --i) {
        if (str[i - 1] >= '0' && str[i - 1] <= '9') {
            value += base * (str[i - 1] - '0');
            base *= 10;
        }
        else
            assert(false);
    }

    return value;
}
