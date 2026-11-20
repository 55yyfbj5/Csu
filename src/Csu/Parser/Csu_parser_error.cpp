#pragma once

#include "Csu/Parser/Csu_parser_error.hpp"
#include <system_error>

namespace {
    struct Csu_parser_error_category : public std::error_category {
        const char* name() const noexcept override {
            return "Csu Parser.";
        };
        std::string message(int value) const override {
            switch(static_cast<Csu::Parser::Error>(value)) {
                case Csu::Parser::Error::BAD_INPUT:
                    return "Invalid input.";
                default:
                    return "Unknow error/ invalid enum member.";
            }
        };
    };

    const Csu_parser_error_category the_resource_error_category {};
}


std::error_code Csu::Parser::make_error_code(Csu::Parser::Error error) {
    return {static_cast<int>(error), the_resource_error_category};
};