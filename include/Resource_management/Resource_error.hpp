#pragma once

#include <system_error>

namespace Resource {
    enum class Error : int {
        INVALID_ID,
        NO_FREE_SLOTS_LEFT,
        COULDNT_ALLOCATE_ID,
        UNKNOWN
    };
    std::error_code make_error_code(Resource::Error);
}

namespace std {

    template <>
    struct is_error_code_enum<Resource::Error> : true_type {};
}
