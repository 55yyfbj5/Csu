#pragma once

#include "IRenderer.hpp"
#include <expected>
#include <stdexcept>
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"

namespace Renderer {

class Sdl_renderer : public IRenderer{
public:
    // std::expected<void, std::error_code> initialize(SDL_Window* window) override;
    Sdl_renderer(SDL_Window* window) : _renderer(window ? SDL_CreateRenderer(window, NULL) : throw std::invalid_argument("Invalid window\n") ) {if (!_renderer) { throw std::invalid_argument("Couldn't create renderer\n"); }}
    std::expected<void, std::error_code> begin_drawing() override;
    std::expected<void, std::error_code> draw_square(int x, int y, int, int) override;
    std::expected<void, std::error_code> draw_line(float x1, float y1, float x2, float y2) override;
    //std::expected<void, std::error_code> draw_circle(int x, int y) override;
    std::expected<void, std::error_code> present() override;
private:
    SDL_Renderer* _renderer;  
};

}

