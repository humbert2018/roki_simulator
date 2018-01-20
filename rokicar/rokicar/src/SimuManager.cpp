#include "SimuManager.hpp"
#include "Simu/InitAllSimus.hpp"

namespace rokicar{


SimuManager* SimuManager::get_instance() {
    static SimuManager* mgr = nullptr;
    if(mgr == nullptr) {
        mgr = new SimuManager();
    }
    return mgr;
}

void SimuManager::register_simu(string simu_name, PtrNewBaseSimuCreator simu_creator){
    simu_set.insert(pair<string, PtrNewBaseSimuCreator>(simu_name, simu_creator));
}

BaseSimu* SimuManager::new_simu(string simu_name){
    return simu_set[simu_name]();
}

void SimuManager::InitAll() {
    SimuManager::get_instance();  // create instance
    init_all_simus();
}

};
