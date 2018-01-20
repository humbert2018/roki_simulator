#pragma once
#include "Simu/simple_simu.hpp"

namespace rokicar{

void init_all_simus() {
    REGISTER_SIMU(simple_simu);
}

}