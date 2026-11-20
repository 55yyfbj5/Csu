#include "Csu/Csu_map.hpp"
#include "Core/Core_math.hpp"
#include "Core/Core_utils.hpp"
//#include "Logging_manager.hpp"
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <expected>
#include <optional>
#include <string>
#include <algorithm>
#include <system_error>
#include <vector>
#include "Build_preprocessor_macros.hpp"
#include "Csu/Parser/Csu_parser.hpp"

std::expected<Csu::Map, std::error_code> Csu::Map::create_map_object(const std::string& file_path) {
    auto map = Map();
    if(auto result = map.load_map(file_path); !result) {
        return std::unexpected(result.error());
    }
    return map;
}


std::expected<void, std::error_code> Csu::Map::load_map(std::string file_path) {
    std::string raw_map{Core::Utils::load_text_file(file_path)};
    

    auto result = Csu::Parser::parse_hit_objects(raw_map);
    if (!result) {
        return std::unexpected(result.error());
    }
    _notes = result.value();
    return {};
}
//time_scale_factor is the number by which you can multiply target_time to convert it into miliseconds
// this is done mainly because target_time may be a pcm frame which can vary depending on the song

Core::Math::Vec3<int> Csu::Map::get_note(int target_time, double time_scale_factor) {
    if(_notes.empty()) {
        return Core::Math::Vec3<int> {0,0};
    }
    int current_time = static_cast<int>(static_cast<double>(target_time) * time_scale_factor);
    auto it = std::lower_bound(_notes.begin(), _notes.end(), current_time, [](const Hit_object& a, int time) {return a.timing > time;});
    return Core::Math::Vec3<int> {.x = it->x, .y = it->y, .z = it->timing};
};
