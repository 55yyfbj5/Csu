#pragma once

#include "Resource_states.hpp"

namespace Resource {
    class Resource_base {
    public:
        Resource_base() : state(State::FREE){};
        State state;

    };
};
