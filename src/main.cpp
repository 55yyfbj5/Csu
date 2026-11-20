#include "Application.hpp"
#include "Audio/Raylib_Audio_manager.hpp"
#include "Core/Core_math.hpp"
#include "Renderer/Raylib_Renderer.hpp"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_video.h"
#include "Window/Window.hpp"
#include <cstddef>
#include <cstdlib>
#include <memory>

int main(void) {
    if(!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        return EXIT_FAILURE;   
    }
    //POSSIBLE CODE SMELL: WHO OWNS sdl_window!?!?!?!?
    SDL_Window* sdl_window{}; //PLACEHOLDER
    Core::Math::Vec2<int> starting_window_dimesions{800, 600};
    if(sdl_window = SDL_CreateWindow("Csu", starting_window_dimesions.x, starting_window_dimesions.y, 0); !sdl_window) { // PLACEHOLDER: CHANGE WINDOW SIZE IF NEEDED
        return EXIT_FAILURE;
    }

    if(!SDL_SetWindowPosition(sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED)) {
        
    }
    std::unique_ptr<Window> window {std::make_unique<Window>(sdl_window, starting_window_dimesions)};
    //POSSIBLE CODE SMELL: USED /*POSSIBLY SHARED*/ INFO BELONGING TO A UNIQUE POINTER BEFORE TRANSFERING OWNERSHIP OF THAT UNIQUE POINTER
    std::unique_ptr<Renderer::Sdl_renderer> renderer {std::make_unique<Renderer::Sdl_renderer>((window->get_window_pointer()))};
    
    std::unique_ptr<Raylib_audio_manager> audio_manager {std::make_unique<Raylib_audio_manager>(200)};


    Application main_app(std::move(window), std::move(audio_manager), std::move(renderer));
   
    if(auto result = main_app.initialize(); result) { //TODO: check if main_app is valid. Application follows a work from instantiation model.
        if(!main_app.run()) {
            //TODO: log error or do something with it
            main_app.shutdown();
            return EXIT_FAILURE;
        }
    }
    main_app.shutdown();

    return EXIT_SUCCESS;
}