#include "Core/Core_error.hpp"
#include "Resource_management/Resource_error.hpp"
#include <system_error>

namespace {
    struct Core_error_category : public std::error_category {
    const char* name() const noexcept override {
        return "core error";
    };
    std::string message(int ev) const override {
        switch (static_cast<Core::Error>(ev)) {
            case Core::Error::INTERNAL_ERROR:
                return "There was an internal error.";
            default:
                return "Unknown/Invalid error.";
        }
    };
    bool equivalent(const std::error_code& code, int condition) const noexcept override {
        switch (static_cast<Core::Error>(condition)) {
            case Core::Error::ALLOCATION_ERROR:
                if (code == Resource::Error::COULDNT_ALLOCATE_ID) {
                    return true;
                };
            case Core::Error::INTERNAL_ERROR:
                // if (code ==)
                ;
            case Core::Error::INVALID_OPERATION:
                // if (code ==)
                ;
            case Core::Error::MISSING_FILE:
                // if (code ==)
                ;
            case Core::Error::RESOURCE_EXHAUSTED:
                // if (code ==)
                ;
            case Core::Error::UNKNOWN_ERROR:
                // if (code ==)
                ;
            default: 
                return false;
        }
    };
        
    };

    Core_error_category core_error_category_instance {};
}

std::error_condition Core::make_error_condition(Core::Error error) {
    return {static_cast<int>(error), core_error_category_instance};
}