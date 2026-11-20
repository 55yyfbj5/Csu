#include "Csu/Csu_map.hpp"
#include "Core/Core_math.hpp"

namespace Csu::Utils {
    [[nodiscard]] Core::Math::Vec2<int> map_osu_editor_to_screen_pixels(const Core::Math::Vec2<int>& surface_extent, const Csu::Hit_object& note_osu_position);
    [[nodiscard]] float compute_playfield_scale_factor();
};
