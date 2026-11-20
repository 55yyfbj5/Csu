#include "Csu/Csu_utils.hpp"
#include "Csu/Csu_map.hpp"
#include "Core/Core_math.hpp"



Core::Math::Vec2<int> Csu::Utils::map_osu_editor_to_screen_pixels(const Core::Math::Vec2<int>& terminal_dimensions, const Hit_object& note) {
    Core::Math::Vec2<int> osu_editor_dimensions{512, 384};
    float x_scale_factor = static_cast<float>(terminal_dimensions.x)/osu_editor_dimensions.x;
    float y_scale_factor = static_cast<float>(terminal_dimensions.y)/osu_editor_dimensions.y;
    int x = static_cast<int>(static_cast<float>(note.x) * x_scale_factor);
    int y = static_cast<int>(static_cast<float>(note.y) * y_scale_factor);
    return Core::Math::Vec2<int> {x, y};
};


