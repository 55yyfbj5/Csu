#pragma once

#include <algorithm>
#include <chrono>
#include <source_location>
#include <string>
#include <string_view>
#include <vector>


class Logging_manager {
public:
    Logging_manager(int min_interval): _min_interval(std::max(200, min_interval)) , _internal_clock(0), _previous_timestamp(0), _start_timepoint(std::chrono::steady_clock::now()), _time_since_last_created_log(std::max(200, min_interval) + 1){
    };
    Logging_manager() = default;
    bool log(std::string_view message, std::source_location location = std::source_location::current());

    bool generate_log_file();
    // ~Logging_manager();
private:
    std::vector<std::string> _log_entries;
    int _internal_clock;
    int _previous_timestamp;
    int _time_since_last_created_log;
    std::chrono::time_point<std::chrono::steady_clock> _start_timepoint;
    bool _update_internal_clock();
    int _min_interval;
    [[nodiscard]]bool _can_create_log();
};
