#pragma once
#include <string_view>

unsigned int textToHex(const std::string_view& str);
unsigned int textToBin(const std::string_view& str);
unsigned int textToOct(const std::string_view& str);
unsigned int textToDec(const std::string_view& str);
