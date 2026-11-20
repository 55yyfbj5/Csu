#pragma once
#include <cstdint>

namespace Resource {
    enum class State : uint8_t {
        USED,
        FREE,
        INVALID
    };
};
