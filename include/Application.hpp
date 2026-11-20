#pragma once

#include "Audio/IAudio_manager.hpp"
#include "Renderer/IRenderer.hpp"
#include "Input/IAction_input_manager.hpp"
//  #include "Logging_manager.hpp"
#include "SDL3/SDL_init.h"
// #include <X11/X.h>
#include <cassert>
#include "Window/Window.hpp"
#include <expected>
#include <memory>
#include <system_error>

class Application {
public:
    Application(std::unique_ptr<Window> window, std::unique_ptr<IAudio_manager> audio_manager, std::unique_ptr<IRenderer> renderer/*, std::unique_ptr<IAction_input_manager> action_input_manager*/) :
    _window(std::move(window)),
    _audio_manager(std::move(audio_manager)),
    _renderer(std::move(renderer)) /*, 
    _action_input_manager(std::move(action_input_manager))*/ { assert(_audio_manager && "Error: Invalid audio manager.\n"); assert(_renderer && "Error: Invalid renderer.\n"); /*if(!_action_input_manager) {std::cerr << "Error: Invalid action input manager\n"; }*/ if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){     throw std::runtime_error(std::string("couldn't initialize SDL: ") + SDL_GetError()); }};
    std::expected<void, std::error_code> initialize();
    void shutdown();
    std::expected<void, std::error_code> run();
private:
    std::unique_ptr<Window> _window;
//    Logging_manager _logging_manager;
    std::unique_ptr<IAudio_manager> _audio_manager;
    std::unique_ptr<IRenderer> _renderer;
    std::unique_ptr<IAction_input_manager> _action_input_manager;
    
    
};
