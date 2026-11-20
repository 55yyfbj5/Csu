#pragma once

#include "Resource_management/Resource_error.hpp"
#include <system_error>

namespace {
    struct Resource_error_category : public std::error_category {
        const char* name() const noexcept override {
            return "Resource Management.";
        };
        std::string message(int value) const override {
            switch(static_cast<Resource::Error>(value)) {
                case Resource::Error::INVALID_ID:
                    return "No valid id was provided.";
                default:
                    return "Unknow error/ invalid enum member.";
            }
        };
    };

    const Resource_error_category the_resource_error_category {};
}


std::error_code Resource::make_error_code(Resource::Error error) {
    return {static_cast<int>(error), the_resource_error_category};
};
