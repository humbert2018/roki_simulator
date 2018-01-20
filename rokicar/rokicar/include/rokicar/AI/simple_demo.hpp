#pragma once
#include "AIManager.hpp"

namespace rokicar{
class simple_demo : public BaseAI {

public:
    int tick(Environment* env, BaseRoboCar* self){
        BaseRoboCar* main_car = env->get_main_car();
        if(main_car) {
            self->position = main_car->position + Vector3(1, 1, 0); 
        }
        return 0;
    };


CREATOR_AI(simple_demo);
};
}