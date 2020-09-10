#pragma once
#include "types.hpp"

namespace mcs4 {

    enum class CycleType : Byte
    {
        Invalid, A1, A2, A3, M1, M2, X1, X2, X3
    };

    CycleType& operator++(CycleType& other);

} // namespace mcs4
