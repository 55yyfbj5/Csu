#include <cstdint>

namespace Resource {
    //template<class Tag>
    struct Id {
        Id(uint16_t slot_idx, uint32_t uniq_stamp ) : slot_index(slot_idx), unique_stamp(uniq_stamp), is_valid(true) {};
        Id();
        uint16_t slot_index;
        uint32_t unique_stamp;
        bool is_valid{false};
    };
};
