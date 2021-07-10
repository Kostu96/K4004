#pragma once
#include "emulator/source/cycle_types.hpp"

CycleType& operator++(CycleType& other)
{
    switch (other)
    {
    case CycleType::A1: return other = CycleType::A2;
    case CycleType::A2: return other = CycleType::A3;
    case CycleType::A3: return other = CycleType::M1;
    case CycleType::M1: return other = CycleType::M2;
    case CycleType::M2: return other = CycleType::X1;
    case CycleType::X1: return other = CycleType::X2;
    case CycleType::X2: return other = CycleType::X3;
    case CycleType::X3: return other = CycleType::A1;
    }
    return other = CycleType::Invalid;
}
