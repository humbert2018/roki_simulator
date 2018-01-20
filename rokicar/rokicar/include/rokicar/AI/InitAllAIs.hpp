#pragma once
#include "AI/simple_demo.hpp"
#include "AI/simple_ai.hpp"

namespace rokicar{

void init_all_ais() {
    REGISTER_AI(simple_demo);
    REGISTER_AI(simple_ai);
}

}