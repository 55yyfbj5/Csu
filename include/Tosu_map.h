#include "Tosu_math.h"

using int Tosu_result;

class Tosu_map {

public:
    bool load_map{std::string file_path};

private:
    std::vector<Tosu_math::Vec2<float>> _level_data{};
};
