#include "Csu/Parser/Csu_parser.hpp"
#include "Core/Core_getlines.hpp"
#include "Core/Core_getvalue.hpp"
#include "Core/Core_utils.hpp"
#include "Csu/Csu_map.hpp"
#include "Csu/Parser/Csu_parser_error.hpp"
#include <algorithm>
#include <expected>
#include <print>
#include <system_error>
#include <vector>
#include "Core/outcome.hpp"
#include "Build_preprocessor_macros.hpp"

std::expected<std::vector<Csu::Hit_object>, std::error_code> Csu::Parser::parse_hit_objects(const std::string& raw_map) {
    std::vector<std::string> total_lines{};
    std::vector<Hit_object> result{};
    if(auto result = Core::get_lines(raw_map); result) {
        total_lines = result.value();
    } else {
        return std::unexpected(Csu::Parser::Error::INTERNAL_ERROR);
    }

    for(const std::string& line : total_lines) {
        std::vector<std::string> total_current_line_data{};
        std::vector<int> current_line_hit_object_position_data{};
        std::vector<int> current_int_line_hitsample_data{};
       
        if(auto result = Core::get_lines(line, ','); result) {
            total_current_line_data = result.value();
        } else {
            return std::unexpected(Csu::Parser::Error::UNKNOWN);
        }
        
        //the last element is the hitsample data and not needed here
        total_current_line_data.pop_back();  // POSIBBLE CODE SMELL: THIS IS VERY CONFUSING
        
        if constexpr (IS_DEBUG) {
            for(auto i : total_current_line_data) {
                std::println("total_current_line_data[x] = {}", i);
            }
        }
        for (auto data : total_current_line_data) {
            if(!Core::Utils::is_string_a_number(data)) {
                return std::unexpected(Csu::Parser::Error::UNKNOWN); //PLACEHOLDER: RETURN INVALID INPUT ERROR
            } 
            current_line_hit_object_position_data.push_back(std::stoi(data));
        }
        int first_colon_pos = line.find(":");
        if(first_colon_pos == std::string::npos) {
            return std::unexpected(Csu::Parser::Error::BAD_INPUT); 
        }
        int last_colon_pos = line.rfind(":");
        if(last_colon_pos == std::string::npos) {
            return std::unexpected(Csu::Parser::Error::BAD_INPUT); 
        }
        std::string current_line_hitsample_path = line.substr(last_colon_pos + 1, line.length() - last_colon_pos);
        std::string current_hitsample_substring = line.substr( first_colon_pos - 1, line.length() - first_colon_pos + 1);
        std::vector<std::string> current_string_hitsample_data{};

        if(auto result = Core::get_lines(current_hitsample_substring, ':'); result) {
            current_string_hitsample_data = result.value();
        } else {
            //TODO: HANDLE ERROR
        }
        if(current_string_hitsample_data.size() == 5 ) { //PLACEHOLDER
            //last element is the hitsample path and is not needed here
            current_string_hitsample_data.pop_back();  //POSIBBLE CODE SMELL: THIS IS REALLY CONFUSING
        }
        if(current_string_hitsample_data.size() == 6) { //PLACEHOLDER
            current_string_hitsample_data.erase(current_string_hitsample_data.begin());
            current_string_hitsample_data.pop_back();
        }
        if constexpr (IS_DEBUG) {
            for(auto i : current_string_hitsample_data) {
                std::println("current_string_hitsample_data[x] = {}", i);
            }
        }
        for (auto hitsample_data : current_string_hitsample_data) {
            if(!Core::Utils::is_string_a_number(hitsample_data)) {
                return std::unexpected(Csu::Parser::Error::UNKNOWN); //PLACEHOLDER: RETURN BAD INPUT ERROR
            }
            current_int_line_hitsample_data.push_back(std::stoi(hitsample_data));
        }
        if(current_line_hit_object_position_data.size() != 5) {
            return std::unexpected(Csu::Parser::Error::UNKNOWN);
        }
        if(current_int_line_hitsample_data.size() != 4) {
            return std::unexpected(Csu::Parser::Error::UNKNOWN);
        }
        result.emplace_back(Hit_object{.x = current_line_hit_object_position_data[0],
            .y = current_line_hit_object_position_data[1],
            .timing = current_line_hit_object_position_data[2],
            .type = current_line_hit_object_position_data[3],
            .hit_sound = current_line_hit_object_position_data[4],
            .hit_sample = { .normal_set = current_int_line_hitsample_data[0],
            .additional_set = current_int_line_hitsample_data[1],
            .custom_index = current_int_line_hitsample_data[2],
            .volume = current_int_line_hitsample_data[3],
            .file_name = current_line_hitsample_path
            }
        });
        current_line_hit_object_position_data.clear();
        current_int_line_hitsample_data.clear();
    }
    std::ranges::sort(result, [] (const Hit_object& a, const Hit_object& b) {return a.timing > b.timing;});
    return result;
}

std::expected<Csu::Metadata, std::error_code> Csu::Parser::parse_metadata(const std::string& input) {
    Csu::Metadata result{};
    std::vector<std::string> total_lines{};
    if(auto rs = Core::get_lines(input); rs) {
        total_lines = rs.value();
    } else {
        //TODO: HANDLE ERROR
    }
    if (total_lines.size() != 10) { //TODO: COULD CREATE AN ENUM OR VARIABLE FOR THIS VALUE
        //TODO: RETURN INVALID INPUT ERROR
    }

    if (auto rs = Core::extract_key(total_lines[0])) {
        result.Title = rs.value(); 
    } else {
        //TODO: HANDLE ERROR
    }
    if (auto rs = Core::extract_key(total_lines[1])) {
        result.TitleUnicode = rs.value();
    } else {
        //TODO: HANDLE ERROR
    }
    //TODO: COMPLETE
    return result;
};

std::expected<std::vector<std::string>, std::error_code> Csu::Parser::get_map_sections(std::string raw_map) {
    std::vector<std::string> total_lines{};  //TODO: REPLACE FOR TRY MACRO
    if(auto rs = Core::get_lines(raw_map); rs) {
        total_lines = rs.value();
    } else {
        return std::unexpected(Csu::Parser::Error::UNKNOWN);
    }
    std::vector<std::string> sections_buffer{};
    std::string section_buffer{};
    bool recording{false};
    for (auto line : total_lines) {
        if (line.starts_with('[')) { //TODO: WRITE SECTION SPECIFIC CODE TO CHECK FILE VALIDITY
            recording = true;
        }
        if(recording) {
            section_buffer += line;
        }
        if(line.empty() && recording) { //POSSIBLE CODE SMELL: DEPENDENT ON HOW THE FILE WAS READ
            recording = false;
            sections_buffer.push_back(section_buffer);
            section_buffer = "";
        }
    }     
    if (sections_buffer.empty()) {
        return std::unexpected(Csu::Parser::Error::UNKNOWN);
    }
    return sections_buffer;   
};
