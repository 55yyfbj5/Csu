#include "Core/Core_getlines.hpp"
#include <optional>
#include <sstream>
#include <string>
#include <vector>

std::optional<std::vector<std::string>> Core::get_lines(const std::string& input, char delimeter) {
    std::vector<std::string> result{};
    std::string buffer{};
    std::stringstream input_stream{input};
    while(std::getline(input_stream, buffer, delimeter)) {
        // if(input_stream.bad()) {
        //     return std::nullopt;
        // }
        // if(input_stream.fail()) {
        //     return std::nullopt;
        // }
        result.push_back(buffer);
    }
    return result;
};