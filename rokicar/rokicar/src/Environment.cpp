#include "Environment.hpp"

namespace rokicar{

Environment::Environment()
{
    all_cars.clear();
    road_info = new RoadInfo();
    last_ai_tick_time = -1;
    last_simu_tick_time = -1;
    last_gc_tick_time = -1;
}

Environment::~Environment()
{
    for(auto car : all_cars){
        if(car.second != nullptr) delete car.second;
    }
    all_cars.clear();
    if(road_info != nullptr) {
        delete road_info;
        road_info = nullptr;
    }
}

bool Environment::add_robot(BaseRoboCar* robo)
{
    if(robo->id != NULL_CAR_ID){ // 直接替换all_cars里的robo
        auto old = all_cars.find(robo->id);
        if(old != all_cars.end()){
            if(old->second == robo) return true;
            delete old->second;  // 销毁原来的
        }
       all_cars[robo->id] = robo;
       return true;
    }

    for(int _id = 1; _id <= MAX_ROBOTS_NUM; _id++) {  // 
        if(all_cars.find(_id) == all_cars.end()){
            all_cars.insert(pair<int, BaseRoboCar*>(_id, robo));
            robo->id = _id;
            return true;
        }
    }
    // more than MAX_ROBOTS_NUM rokicars
    return false;
}

void Environment::ai_tick(double now_time) 
{
    last_ai_tick_time = now_time;
    for(auto car_ptr : all_cars) {
        BaseRoboCar* car = car_ptr.second;
        // main_car不需要运行AI
        if(car->type == ROKI_CAR){
            car->tick(this);
        }
    }
}

void Environment::simu_tick(double now_time)
{
    double old_t = last_simu_tick_time;
    last_simu_tick_time = now_time;
    if(old_t < 0) return;
    double delta_t = now_time - old_t;
    for(auto car_ptr : all_cars) {
        BaseRoboCar* car = car_ptr.second;
        // main_car不需要运行AI
        if(car->type == ROKI_CAR){
            car->simulate(this, delta_t);
        }
    }
}

void Environment::gc_tick(double now_time)
{
    last_gc_tick_time = now_time;
    BaseRoboCar* main_car = get_main_car();
    if(main_car != nullptr){
        vector<int> need_delete;
        for(auto car_ptr : all_cars) {
            BaseRoboCar* car = car_ptr.second;
            if(car->type == ROKI_CAR){
                if(car->position.distance2(main_car->position) >= MAX_LIMIT_DISTANCE2) {
                    need_delete.push_back(car_ptr.first);
                }
            }
        }
        for(int id : need_delete){
            delete all_cars[id];
            all_cars.erase(id);
        }
    }
}

// 模拟器rokicar的物理模拟
void Environment::update_cars(){
    double secs =ros::Time::now().toSec();
    if(secs > last_ai_tick_time + AI_TICK_INTERVAL){
        ai_tick(secs);
        // ROS_ERROR("ai tick time at: %lf", secs);
    }
    if(secs > last_simu_tick_time + SIMU_TICK_INTERVAL){
        simu_tick(secs);
        // ROS_ERROR("simu tick time at: %lf", secs);
    }
    // try delete useless cars
    if(secs > last_gc_tick_time + GC_TICK_INTERVAL) {
        gc_tick(secs);
    }    
}							

// 主车的位置信息，来自hadmap相关msg驱动
void Environment::update_main_car(Vector3 pos, Quaternion quat){
    BaseRoboCar* main_car = get_main_car();
    if(main_car == nullptr) {
        // 还没创建
        BaseRoboCar* car = new MainCar(Box(5,5,5), pos, quat);
        add_robot(car);
    }
    else {
        main_car->position = pos;
        main_car->quat = quat;
    }
}

vector<BaseRoboCar*> Environment::get_all_rokicars() {
    vector<BaseRoboCar*> ret;
    for(auto& iter : all_cars){
        if(iter.second->type == MAIN_CAR) continue;
        ret.push_back(iter.second);
    }
    return ret;
}

}