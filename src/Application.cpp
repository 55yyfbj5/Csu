#include "Application.hpp"
#include "Core/Core_math.hpp"
#include "Resource_management/Resource_error.hpp"
#include "SDL3/SDL_events.h"
#include "Csu/Csu_map.hpp"
// #include "Logging_manager.hpp"
#include "Renderer/IRenderer.hpp"
#include "Audio/IAudio_manager.hpp"
#include "Build_preprocessor_macros.hpp"
#include <algorithm>
#include <expected>
#include <print>
#include <system_error>

struct placeholder_square {
    placeholder_square(Core::Math::Vec2<int> pos, Core::Math::Vec2<int> sp, Core::Math::Vec2<int> dimen) : position(pos), speed(sp), dimensions(dimen) {}
    Core::Math::Vec2<int> position;
    Core::Math::Vec2<int> speed;
    const Core::Math::Vec2<int> dimensions;
    void update(Core::Math::Vec2<int> window_dimensions, float delta_time) {
        if (position.x < window_dimensions.x) {
            position.x += speed.x * delta_time;
        }
        if (position.y < window_dimensions.y) {
            position.y += speed.y * delta_time;
        }
        if (position.x < window_dimensions.x) {
            position.x += speed.x * delta_time;
        }
        if (position.y < window_dimensions.y) {
            position.y += speed.y * delta_time;
        }
    }
};

namespace Csu {
    struct Context {
        Core::Math::Vec2<int> nothing;
    };
}


std::expected<void, std::error_code> Application::initialize() {
   // _logging_manager = Logging_manager(200);

    return {};
};



std::expected<void, std::error_code> Application::run() {

    Csu::Map current_map{};
    if(auto rs = Csu::Map::create_map_object("./assets/test_map.osu")) {
        current_map = rs.value();    
    } else {
        return std::unexpected(Resource::Error::UNKNOWN); //PLACEHOLDER
    }
    Audio_file_handle current_audio_handle{};
    if(auto result = _audio_manager->load_audio_file("./assets/osumap/audio.wav"); !result) {
        // if (result.error() == Core::Error::ALLOCATION_ERROR) {
            
        // }
        return std::unexpected(Resource::Error::UNKNOWN);
    } else {
        current_audio_handle = result.value();
    }
    if (!_audio_manager->play_audio(current_audio_handle)) {
        return std::unexpected(Resource::Error::UNKNOWN);
    }  

    // if(result.error() != Core::Error::ALLOCATION_ERROR) {
    //     auto current_audio_handle = *result;     

    //     if(!_audio_manager->play_audio(current_audio_handle)) {
    //         //handle error
    //     }
    // }
    Core::Math::Vec2<int> osu_playfield_dimensions{640, 480};
    int max_osu_playfield_screen_percentage{90};
    int playfield_x_offset{20};
    int playfield_y_offset{0};
   // float playfield_scale_factor{std::min(static_cast<float>((_window->get_window_dimensions().x * max_osu_playfield_screen_percentage)/100)/static_cast<float>(osu_playfield_dimensions.x), static_cast<float>((_window->get_window_dimensions().y * max_osu_playfield_screen_percentage)/100))/static_cast<float>(osu_playfield_dimensions.y)};
    float playfield_scale_factor{1.0f};
    Core::Math::Vec2<int> max_playfield_coordinates{playfield_x_offset + (static_cast<int>(static_cast<float>(osu_playfield_dimensions.x) * playfield_scale_factor)), playfield_y_offset + (static_cast<int>(static_cast<float>(osu_playfield_dimensions.y) * playfield_scale_factor))};
    Core::Math::Vec2<int> min_playfield_coordinates{playfield_x_offset, playfield_y_offset};
    std::println("max_playfield_coordinates --> ({}, {})", max_playfield_coordinates.x, max_playfield_coordinates.y);
    std::println("playfield_scale_factor --> {}", playfield_scale_factor);
    while(!_window->should_close()) {
        SDL_Event event{};
        while(SDL_PollEvent(&event)) 
            if(!_window->handle_window_event(&event)) {
                //HANDLE ERROR
            }
        

        if(_audio_manager->update_stream(current_audio_handle)) {

        }
        
        if (!_renderer->begin_drawing()) {
        
        }

        if(!_renderer->draw_line(min_playfield_coordinates.x, min_playfield_coordinates.y, min_playfield_coordinates.x, max_playfield_coordinates.x)) {

        }
        
        if(!_renderer->draw_square(300, 100, 30, 30)) {

        }
        if(!_renderer->present()) {

        }
        if constexpr (IS_DEBUG) {

         //   std::cout << "current_note_position: x: " << current_note_position.x << " y: " << current_note_position.y << "\n" << "current pcm frame: " << current_pcm_frame << "\n";
            //_logging_manager.log(std::format("current position: x: {}, y: {} \ncurrent pcm frame: {}\n", current_note_position.x, current_note_position.y, current_pcm_frame));
        }
    }
    return {};    
}
    

void Application::shutdown() {
    std::println("shutting down...");
    // _logging_manager.generate_log_file();
};
