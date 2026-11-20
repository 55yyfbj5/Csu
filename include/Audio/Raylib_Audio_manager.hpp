#pragma once

#include "IAudio_manager.hpp"
#include "Resource_management/Resource_base.hpp"
#include "Resource_management/Resource_pool.hpp"
#include <algorithm>
#include <cstdint>
#include <expected>
#include <stdexcept>
#include <system_error>
#include "SDL3/SDL_audio.h"



struct Sound {
    Uint8 *wav_data;
    Uint32 wav_data_len;
    SDL_AudioStream *stream;
    uint32_t total_queued_samples_last_update;
    uint32_t played_samples;
};
//POSSIBLE CODE SMELL: Can't define Audio_file_resource before Sound
struct Audio_file_resource : public Resource::Resource_base {
    Sound audio_file;
};



class Raylib_audio_manager : public IAudio_manager{
public:
    //POSSIBLE CODE SMELL: RETARDED CONSTRUCTOR. INCONSISTENT USAGE OF OUTLINE FUNCTION DEFINITIONS
    Raylib_audio_manager(int pool_size) : _audio_resource_pool(std::max(5, pool_size)), _audio_device(SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL)) {
        if(!SDL_ResumeAudioDevice(_audio_device)) {throw std::runtime_error("couldnt resume audio device");}
    };
    std::expected<Audio_file_handle, std::error_code> load_audio_file(const std::filesystem::path& path) override;
    std::expected<void, std::error_code> unload_audio_file(Audio_file_handle audio_file) override;
    std::expected<void, std::error_code> pause_audio(Audio_file_handle audio_file) override;
    std::expected<void, std::error_code> play_audio(Audio_file_handle audio_file) override;
    std::expected<void, std::error_code> set_volume(Audio_file_handle audio_file, int volume) override;
    std::expected<void, std::error_code> set_speed(Audio_file_handle audio_file, int speed) override;
    std::expected<void, std::error_code> seek(Audio_file_handle audio_file, int64_t timestamp) override;
    int get_current_time(Audio_file_handle audio_file) override;
    //PLACEHOLDER: update_stream should be update_streams and update all streams without taking a handle
    std::expected<void, std::error_code> update_stream(Audio_file_handle audio_file) override;
private:
//    std::expected<void, std::error_code> _initialize_audio();//...
    
    SDL_AudioDeviceID _audio_device{};
    //POSSIBLE CODE SMELL: Passing Audio_file as a template argument to Resource::Pool is confusing and possibly error prone
    Resource::Pool<Audio_file_resource, struct Audio_file> _audio_resource_pool;
};
