#pragma once

#include <cstdint>
#include <filesystem>
#include <expected>
#include <system_error>
#include "Core/Core_handle.hpp"



using Audio_file_handle = Core::Handle<struct Audio_file>;

class IAudio_manager {
public:
    virtual ~IAudio_manager() = default;
    virtual std::expected<Audio_file_handle, std::error_code> load_audio_file(const std::filesystem::path& path) = 0;
    virtual std::expected<void, std::error_code> unload_audio_file(Audio_file_handle audio_file) = 0;
    virtual std::expected<void, std::error_code> pause_audio(Audio_file_handle audio_file) = 0;
    virtual std::expected<void, std::error_code> play_audio(Audio_file_handle audio_file) = 0;
    virtual std::expected<void, std::error_code> set_volume(Audio_file_handle audio_file, int volume) = 0;
    virtual std::expected<void, std::error_code> set_speed(Audio_file_handle audio_file, int speed) = 0;
    virtual std::expected<void, std::error_code> seek(Audio_file_handle audio_file, int64_t timestamp) = 0;
    virtual int get_current_time(Audio_file_handle audio_file) = 0;
    virtual std::expected<void, std::error_code> update_stream(Audio_file_handle audio_file) = 0;
    // virtual Result unload_unused_audio_files() = 0;
    // virtual Result shutdown() = 0;
    // virtual int get_current_song_id() = 0;
    // virtual std::expected<int, Result> get_sample_rate(Audio_file_handle audio_file) = 0;
};
