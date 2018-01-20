#include "AIManager.hpp"
#include "AI/InitAllAIs.hpp"

namespace rokicar{


AIManager* AIManager::get_instance() {
    static AIManager* mgr = nullptr;
    if(mgr == nullptr) {
        mgr = new AIManager();
    }
    return mgr;
}

void AIManager::register_ai(string ai_name, PtrNewBaseAICreator ai_creator){
    ai_set.insert(pair<string, PtrNewBaseAICreator>(ai_name, ai_creator));
}

BaseAI* AIManager::new_ai(string ai_name){
    return ai_set[ai_name]();
}

void AIManager::InitAll() {
    AIManager::get_instance();  // create instance
    init_all_ais();
}

};
