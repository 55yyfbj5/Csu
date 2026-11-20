#pragma once

#include "Csu/Csu_map.hpp"
#include <expected>
#include <string>
#include <system_error>

namespace Csu::Parser {
    std::expected<Csu::General, std::error_code> parse_general(const std::string& input);
    std::expected<Csu::Editor, std::error_code> parse_editor(const std::string& input);
    std::expected<Csu::Metadata, std::error_code> parse_metadata(const std::string& input);
    std::expected<Csu::Difficulty, std::error_code> parse_difficulty(const std::string& input);
    //    std::expected<Csu::Editor, std::error_code> parse_events(const std::string& input);
    //    std::expected<Csu::Editor, std::error_code> parse_timingpoints(const std::string& input);
    std::expected<std::vector<Hit_object>, std::error_code> parse_hit_objects(const std::string& input);
    std::expected<std::vector<std::string>, std::error_code> get_map_sections(std::string raw_map);
}