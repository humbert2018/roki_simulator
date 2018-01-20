#pragma once
#include <map>
#include "RokiHeader.h"
#include "Environment.hpp"
#include "BaseRoboCar.hpp"


// Simu
namespace rokicar{

#define REGISTER_SIMU(class_name) \
class_name::register_simu_##class_name()

#define CREATOR_SIMU(class_name) \
public:\
inline static BaseSimu* creator() { return new class_name(); } \
inline static void register_simu_##class_name() { SimuManager::get_instance()->register_simu(#class_name, class_name::creator); }

class BaseSimu {
public:
    virtual int simulate(Environment* env, BaseRoboCar* self, double delta_t) = 0;
};

 
typedef BaseSimu* (*PtrNewBaseSimuCreator)(void);  

class SimuManager {
public:
    static SimuManager* get_instance();
    static void InitAll();
    void register_simu(string simu_name, PtrNewBaseSimuCreator simu_creator);
    BaseSimu* new_simu(string simu_name);

private:
    map<string, PtrNewBaseSimuCreator> simu_set;

};



}