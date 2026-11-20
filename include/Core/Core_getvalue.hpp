#pragma once

#include <optional>
#include <string>

namespace Core {
    //extracts a key from a key value pair in a string
    std::optional<std::string> extract_key(std::string input, char delimeter = ':');
}