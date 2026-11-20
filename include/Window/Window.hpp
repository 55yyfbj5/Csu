#pragma once

#include "Core/Core_math.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include <expected>
#include <stdexcept>
#include <system_error>

class Window {
public:
    Window(SDL_Window* window, Core::Math::Vec2<int> window_dimensions) : _window(window), _window_dimensions(window_dimensions), _should_close(false) {if(!window) { throw std::invalid_argument("INVALID WINDOW\n");}}

    inline SDL_Window* get_window_pointer() {
        return _window;
    }

    inline Core::Math::Vec2<int> get_window_dimensions() {
        return _window_dimensions;
    }

    inline std::expected<void, std::error_code> handle_window_event(SDL_Event* event) {
        switch (event->type) {
            case SDL_EVENT_QUIT:
                _should_close = true;
                break;
                //TODO: HOW DOES THE RENDERER KNOW ABOUT THIS??
            case SDL_EVENT_WINDOW_RESIZED:
                _window_dimensions.x = event->window.data1;
                _window_dimensions.y = event->window.data2;
                break;

        }
        return {};
    }

    //POSSIBLE CODE SMELL: return type of should_close is not consistent with my usage of std::expected
    inline bool should_close() {
        return _should_close;
    }
private:
    
    SDL_Window* _window;
    Core::Math::Vec2<int> _window_dimensions;
    bool _should_close;
};