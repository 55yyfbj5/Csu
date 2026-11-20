#pragma once

#include <SDL3/SDL_video.h>
#include <expected>
#include <system_error>


class IRenderer {
public:
    virtual ~IRenderer() = default;
    // virtual std::expected<void, std::error_code> initialize(SDL_Window* window) = 0;
    virtual std::expected<void, std::error_code> begin_drawing() = 0;
    virtual std::expected<void, std::error_code> draw_square(int x, int y, int w, int h) = 0;
    virtual std::expected<void, std::error_code> draw_line(float x1, float y1, float x2, float y2) = 0;
    //virtual std::expected<void, std::error_code> draw_circle(int x, int y) = 0;
    virtual std::expected<void, std::error_code> present() = 0;
    // virtual std::expected<void, std::error_code> placeholder_draw(/*TODO: shoudl probably take vertices*/) = 0;
    // virtual std::expected<void, std::error_code> placeholder_update() = 0;
};
