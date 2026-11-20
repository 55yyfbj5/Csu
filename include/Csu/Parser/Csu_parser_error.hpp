#pragma once

#include <system_error>

namespace Csu::Parser {
    enum class Error {
        INTERNAL_ERROR,
        BAD_INPUT,
        UNKNOWN      
    };
    std::error_code make_error_code(Csu::Parser::Error);
}

namespace std {

    template <>
    struct is_error_code_enum<Csu::Parser::Error> : true_type {};
}