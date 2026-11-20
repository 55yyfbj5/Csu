#pragma once

#include <string>

namespace Core::Utils {
    std::string load_text_file(const std::string& file_path);
    [[nodiscard]] bool is_string_a_number(const std::string& string);
    [[nodiscard]] std::string get_current_system_time();
}
