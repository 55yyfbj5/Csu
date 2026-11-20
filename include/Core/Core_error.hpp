#pragma once

#include <system_error>

//TODO: get rid of values that are too specific

namespace Core {
    enum class Error : int {
        UNKNOWN_ERROR,
        INVALID_OPERATION,
        ALLOCATION_ERROR,
        MISSING_FILE,
        RESOURCE_EXHAUSTED,
        INTERNAL_ERROR
    };

    std::error_condition make_error_condition(Core::Error e);
}

namespace std
{
  template <>
    struct is_error_condition_enum<Core::Error>
    : true_type {}; 
}
  
