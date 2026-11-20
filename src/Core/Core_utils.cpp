#include "Core/Core_utils.hpp"
#include <chrono>
#include <format>
#include <fstream>
#include <sstream>

std::string Core::Utils::load_text_file(const std::string& file_path) {
    std::ifstream file{file_path};
    std::stringstream buffer{};
    buffer << file.rdbuf();
    file.close();
    return buffer.str();

};

std::string Core::Utils::get_current_system_time() {
    auto now = std::chrono::system_clock::now();

    auto tp = std::chrono::sys_seconds{ std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch())};
    return std::format("{:%Y-%m-%d %H:%M:%S}", tp);
};

bool Core::Utils::is_string_a_number(const std::string &string) {
    for(char c : string) {
        if(!isdigit(c)) {
            return false;
        }
    }
    return true;
}