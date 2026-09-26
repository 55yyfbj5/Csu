#include "Tosu_utils.h"
#include <fstream>
#include <sstream>

std::string Tosu_utils::load_text_file(std::string file_path) {
    std::ifstream file{file_path};
    std::stringstream buffer{};
    buffer << file.rdbuf();
    file.close();
    return buffer.str();

};
