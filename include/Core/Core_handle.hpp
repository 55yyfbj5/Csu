#pragma once

#include <cstdint>

namespace Core {
    template<typename Tag>
    class Handle {
    public:
        Handle(uint32_t uniq_id, uint16_t idx) : unique_id(uniq_id), slot_index(idx) {};
        Handle() : unique_id(0), slot_index(0) {};
        uint32_t unique_id;
        uint16_t slot_index;

    };
}
