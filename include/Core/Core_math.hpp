#pragma once

namespace Core::Math {
    template<typename T>
    struct Vec2 {
        T x{};
        T y{};
    };

    template<typename T>
    struct Vec3 {
        T x{};
        T y{};
        T z{};
    };

    [[nodiscard]] inline int int_by_float(int a, float b) { //POSSIBLE CODE SMELL: THE NAME OF THE FUNCTION SOUNDS OFF
        return static_cast<int>(static_cast<float>(a) * b);
    }
}