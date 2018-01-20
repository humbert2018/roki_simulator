#pragma once
#include "AIManager.hpp"

namespace rokicar{
class simple_ai : public BaseAI {
/*
    一个沿着路一直走的AI(忽略周围的车辆)
*/

public:
    int tick(Environment* env, BaseRoboCar* self){
        
        return 0;
    };


CREATOR_AI(simple_ai);
};
}