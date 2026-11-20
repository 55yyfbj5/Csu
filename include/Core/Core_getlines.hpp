#pragma once

#include <optional>
#include <string>
#include <vector>

namespace Core {
    std::optional<std::vector<std::string>> get_lines(const std::string& input, char delimeter = '\n');
}