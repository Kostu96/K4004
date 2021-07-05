// TODO: Move to shared location
#pragma once
#include <cinttypes>

bool hexStrToUint8(const char* hexStr, uint8_t& value);
bool binStrToUint8(const char* binStr, uint8_t& value);
bool octStrToUint8(const char* octStr, uint8_t& value);
