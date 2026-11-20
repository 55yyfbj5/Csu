#include "Renderer/Raylib_Renderer.hpp"
#include "Resource_management/Resource_error.hpp"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include <SDL3/SDL.h>
#include <system_error>

std::expected<void, std::error_code> Renderer::Sdl_renderer::begin_drawing() {
    SDL_SetRenderDrawColor(_renderer, 33, 33, 33, SDL_ALPHA_OPAQUE);  /* dark gray, full alpha */
    SDL_RenderClear(_renderer); 
    return {};
};

std::expected<void, std::error_code> Renderer::Sdl_renderer::draw_square(int x, int y, int w, int h) {
    SDL_FRect rect{};

    /* draw a filled rectangle in the middle of the canvas. */
    SDL_SetRenderDrawColor(_renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);  /* blue, full alpha */
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    SDL_RenderFillRect(_renderer, &rect);

    /* draw some points across the canvas. */
    SDL_SetRenderDrawColor(_renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  /* red, full alpha */
    //SDL_RenderPoints(_renderer, points, SDL_arraysize(points));

    /* draw a unfilled rectangle in-set a little bit. */
    // SDL_SetRenderDrawColor(_renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);  /* green, full alpha */
    // rect.x += 30;
    // rect.y += 30;
    // rect.w -= 60;
    // rect.h -= 60;
    // SDL_RenderRect(_renderer, &rect);

    return {};
}

std::expected<void, std::error_code> Renderer::Sdl_renderer::draw_line(float x1, float y1, float x2, float y2) {
    SDL_SetRenderDrawColor(_renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);  /* yellow, full alpha */
    if(!SDL_RenderLine(_renderer, x1, y1, x2, y2)){
        return std::unexpected(Resource::Error::UNKNOWN); //PLACEHOLDER
    }
    return {};
}

//std::expected<void, std::error_code> Renderer::Sdl_renderer::draw_circle(int x, int y) {};
std::expected<void, std::error_code> Renderer::Sdl_renderer::present() {
    SDL_RenderPresent(_renderer);
    return {};
}



