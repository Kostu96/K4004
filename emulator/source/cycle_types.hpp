#pragma once
#include <cstdint>

enum class CycleType : uint8_t
{
    Invalid, A1, A2, A3, M1, M2, X1, X2, X3
};

CycleType& operator++(CycleType& other);
