#include "Core/Core_getvalue.hpp"
#include <optional>

std::optional<std::string> Core::extract_key(std::string input, char delimeter) {
    int data_position{};
    data_position = input.find(delimeter) + 1;
    if (data_position == std::string::npos) {
        return std::nullopt;
    }
    if (input[data_position] == ' ') {
        data_position += 1;
    }
    if(data_position > input.size()) {
        return std::nullopt;
    }
    return input.substr(data_position);
}
