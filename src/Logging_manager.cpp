#include "Logging_manager.hpp"
#include "Core/Core_utils.hpp"
#include <chrono>
#include <format>
#include <fstream>
//#include <iostream>
#include <string>

bool Logging_manager::log(std::string_view message, std::source_location location) {
    if(_can_create_log() == false) {
        return false;
    }

    std::string log_entry = std::format("Function: {}\n Information: {}", location.function_name(), message);
    //std::cout << "log_entry: " << log_entry << "\n";
    _log_entries.push_back(log_entry);
    _time_since_last_created_log = 0;

    return true;
};

bool Logging_manager::_can_create_log() {
    _previous_timestamp = _internal_clock;
    _update_internal_clock();
    _time_since_last_created_log += _internal_clock - _previous_timestamp;
    // std::cout << "internal_clock: " << _internal_clock << "\n" <<
    //     "internal clock - previous timestamp: " << _internal_clock - _previous_timestamp << "\n" <<
    //     "time since last created log: " << _time_since_last_created_log << "\n";
    if (_time_since_last_created_log > _min_interval) {

        return true;
    }
    return false;
};

bool Logging_manager::_update_internal_clock() {
    auto start = _start_timepoint;
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    _internal_clock = duration.count();
    return true;
};

bool Logging_manager::generate_log_file() {
    std::ofstream log_file(std::format("./logs/{}_log.txt", Core::Utils::get_current_system_time()));
    for(std::string log_entry : _log_entries) {
        //  std::cout << "log_entry" << log_entry << "\n";
        log_file << log_entry;
    }
    return true;
};

// Logging_manager::~Logging_manager() {
//     generate_log_file();
// };
