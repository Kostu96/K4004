// TODO: Move to shared location
#pragma once
#include <cinttypes>

bool hexStrToUint8(const char* hexStr, uint16_t& value);
bool binStrToUint8(const char* binStr, uint16_t& value);
bool octStrToUint8(const char* octStr, uint16_t& value);
