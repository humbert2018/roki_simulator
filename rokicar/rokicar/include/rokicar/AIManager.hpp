#pragma once
#include <map>
#include "RokiHeader.h"
#include "Environment.hpp"
#include "BaseRoboCar.hpp"


// AI基类：做反射用
namespace rokicar{

#define REGISTER_AI(class_name) \
class_name::register_ai_##class_name()

#define CREATOR_AI(class_name) \
public:\
inline static BaseAI* creator() { return new class_name(); } \
inline static void register_ai_##class_name() { AIManager::get_instance()->register_ai(#class_name, class_name::creator); }

class BaseAI {
public:
    virtual int tick(Environment* env, BaseRoboCar* self) = 0;
};

 
typedef BaseAI* (*PtrNewBaseAICreator)(void);  

class AIManager {
public:
    static AIManager* get_instance();
    static void InitAll();
    void register_ai(string ai_name, PtrNewBaseAICreator ai_creator);
    BaseAI* new_ai(string ai_name);

private:
    map<string, PtrNewBaseAICreator> ai_set;

};



}