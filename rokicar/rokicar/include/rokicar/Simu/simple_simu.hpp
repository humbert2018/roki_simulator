#pragma once
#include "SimuManager.hpp"

namespace rokicar{
class simple_simu : public BaseSimu {
/*
    用speed和rota_radius做简单仿真的simu
*/

public:
    int simulate(Environment* env, BaseRoboCar* self, double delta_t){
        tf::Transform transform;
        transform.setRotation(self->quat);
        Vector3 ahead = transform(Vector3(1, 0, 0));
        self->position = self->position + ahead * self->speed * delta_t;
        // ROS_ERROR("roki pos: %.2lf %.2lf", self->position.x(), self->position.y());
        return 0;
    };


CREATOR_SIMU(simple_simu);
};
}