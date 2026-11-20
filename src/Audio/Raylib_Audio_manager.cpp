#include "Audio/Raylib_Audio_manager.hpp"
#include "Audio/IAudio_manager.hpp"
#include "Build_preprocessor_macros.hpp"
#include "Core/Core_error.hpp"
#include "Resource_management/Resource_error.hpp"
#include "SDL3/SDL_audio.h"
#include <cstdint>
#include <expected>
#include <filesystem>
#include <print>
#include <system_error>

std::expected<Audio_file_handle, std::error_code> Raylib_audio_manager::load_audio_file(const std::filesystem::path& path) {
    std::error_code error{};
    auto audio_file_size_in_bytes = std::filesystem::file_size(path, error);
    if (static_cast<bool>(error) != 0) {
        std::println("couldn't open file {}", path.string());
        return std::unexpected(make_error_code(Resource::Error::UNKNOWN)); //PLACEHOLDER
    }
    auto audio_file_size_in_megabytes = audio_file_size_in_bytes/1048576;
    auto result = _audio_resource_pool.allocate_id();
    if(!result) {
        if (result.error() == Resource::Error::COULDNT_ALLOCATE_ID) {
            return std::unexpected(make_error_code(Resource::Error::COULDNT_ALLOCATE_ID));
        }
        return std::unexpected(make_error_code(Resource::Error::UNKNOWN));
    }
    auto handle = result.value();
    auto result2 = _audio_resource_pool.get_resource_pointer(handle); //PLACEHOLDER
    if(!result2) {
        return std::unexpected(make_error_code(Resource::Error::UNKNOWN));
    }
    auto resource = result2.value();
    //POSSIBLE CODE SMELL: TRASHY ILLOGICAL SIZE CHECK
    if(true) { //PLACEHOLDER: 0 is a placeholder value
        SDL_AudioSpec audio_file_spec{};
        if(!SDL_LoadWAV(path.string().c_str(), &audio_file_spec, &resource->audio_file.wav_data, &resource->audio_file.wav_data_len)) { //PROBABLY WRONG
            return std::unexpected(make_error_code(Resource::Error::UNKNOWN));
        }
        if(resource->audio_file.stream = SDL_CreateAudioStream(&audio_file_spec, NULL); !resource->audio_file.stream) {
            return std::unexpected(Resource::Error::UNKNOWN);
        }//PROBABLY WRONG
        if(!SDL_BindAudioStream(_audio_device, resource->audio_file.stream)) {
            return std::unexpected(Resource::Error::UNKNOWN);
        }
        
    } // else if (audio_file_size_in_megabytes >= 10) {
    //     resource->audio_file = raylib::Sound(path);
    // }
    return handle;
}

// void Application::run() { //code example
// .....
// auto result = function_that_returns_result_or_error_code(); //returns std::expected<> 
// if(!result) {
//     if(result.error() == Core::Error::CRITICAL_ERROR) {
//         return; //early return
//     }
//     if(result.error() == Core::Error::NON_CRITICAL_ERROR) {
//     //try to solve error or keep going
//     }
// }
// .....
// } end of Applicatoin::run()

// if(auto result = )


std::expected<void, std::error_code> Raylib_audio_manager::play_audio(Audio_file_handle audio_file) {
    Audio_file_resource* audio_resource_pointer{};
    if (auto rs = _audio_resource_pool.get_resource_pointer(audio_file); rs) {
        auto audio_resource_pointer = rs.value();
    } else {
        return std::unexpected(make_error_code(Resource::Error::UNKNOWN));
    }
    if(!SDL_ResumeAudioDevice(_audio_device)) {
        return std::unexpected(make_error_code(Resource::Error::UNKNOWN));
    }
    return {};
}

std::expected<void, std::error_code> Raylib_audio_manager::unload_audio_file(Audio_file_handle audio_file) {
    return {};
};
std::expected<void, std::error_code> Raylib_audio_manager::pause_audio(Audio_file_handle audio_file) {
    return {};
};

std::expected<void, std::error_code> Raylib_audio_manager::set_volume(Audio_file_handle audio_file, int volume) {
    return {};
};
std::expected<void, std::error_code> Raylib_audio_manager::set_speed(Audio_file_handle audio_file, int speed) {
    return {};
};
std::expected<void, std::error_code> Raylib_audio_manager::seek(Audio_file_handle audio_file, int64_t timestamp) {
    return {};
};
int Raylib_audio_manager::get_current_time(Audio_file_handle audio_file) {
    
    return {};
};

std::expected<void, std::error_code> Raylib_audio_manager::update_stream(Audio_file_handle audio_file) {
    Audio_file_resource* resource_pointer{};
    if(auto result = _audio_resource_pool.get_resource_pointer(audio_file); !result) {
        return std::unexpected(Resource::make_error_code(Resource::Error::UNKNOWN)); 
    } else {
        resource_pointer = result.value();
    }
   
    int bytes = SDL_GetAudioStreamQueued(resource_pointer->audio_file.stream);
    if(resource_pointer->audio_file.total_queued_samples_last_update == 0) {
        resource_pointer->audio_file.total_queued_samples_last_update = bytes;
    }
    if constexpr (IS_DEBUG) {
        if(bytes > resource_pointer->audio_file.total_queued_samples_last_update) {
            std::println("BEWARE, INTEGER OVERFLOW HAPPENING.");
        }
    }
    resource_pointer->audio_file.played_samples += resource_pointer->audio_file.total_queued_samples_last_update - bytes;
    std::println("resource_pointer->audio_file.total_queued_samples_last_update - bytes == ({} - {}) == {}", resource_pointer->audio_file.total_queued_samples_last_update, bytes, resource_pointer->audio_file.total_queued_samples_last_update - bytes);
    resource_pointer->audio_file.total_queued_samples_last_update = bytes;
    // std::println("bytes queued: {}\nresource_pointer->audio_file.played_samples = {} \nresource_pointer->audio_file.total_queued_samples_last_update = {}\n", bytes, resource_pointer->audio_file.total_queued_samples_last_update - bytes, bytes);
    std::println("resource_pointer->audio_file.played_samples == {}", resource_pointer->audio_file.played_samples);
    if ((bytes < 100000)) { //MAGIC CODE:             
        SDL_PutAudioStreamData(resource_pointer->audio_file.stream, resource_pointer->audio_file.wav_data, 1000000); //FIXME: I FED /*IT*/ TO LITTLE DATA(4000 BYTES) AND IT WAS BORDERLINE IMPOSIBBLE TO DEBUG
        resource_pointer->audio_file.total_queued_samples_last_update = bytes + 1000000;

    }
    return {};
}

// std::expected<void, std::error_code> Raylib_audio_manager::_initialize_audio() {
//      = 
//     if (_audio_device == 0) {
//         //SDL_Log("Couldn't open audio device: %s", SDL_GetError());
//         //return SDL_APP_FAILURE;
//     }
// };



// Error Raylib_audio_manager::translate_to_error(const Resource_error& resource_error) {
//     if(resource_error == Resource_error::COULDNT_ALLOCATE_ID) {
//         return Error::INTERNAL_ERROR;
//     } else if (resource_error == Resource_error::NO_FREE_SLOTS_LEFT) {
//         return Error::RESOURCE_EXHAUSTED;
//     }
//     return {};
// };
