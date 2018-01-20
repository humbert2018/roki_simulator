#include "BaseRoboCar.hpp"

namespace rokicar{

BaseRoboCar::BaseRoboCar(Box _size, Vector3 _pos, Quaternion _quat, string _ai_name, string _simu_name, int _id, RoboType _type):
id(_id), type(_type), size(_size), position(_pos), quat(_quat) {
	speed = 0;
	rota_radius = 0;
	if(_ai_name!="") ai = AIManager::get_instance()->new_ai(_ai_name);
	else ai = nullptr;
	if(_simu_name!="") simu = SimuManager::get_instance()->new_simu(_simu_name);
	else simu = nullptr;
	bornTime = ros::Time::now().sec;
}

int BaseRoboCar::tick(Environment* envInfo){
	if(ai == nullptr) return 0;
	return ai->tick(envInfo, this);
}

int BaseRoboCar::simulate(Environment* envInfo, double delta_t){
	if(simu == nullptr) return 0;
	return simu->simulate(envInfo, this, delta_t);
}

};