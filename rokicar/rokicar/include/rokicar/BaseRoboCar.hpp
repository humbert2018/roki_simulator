#pragma once
#include "geometry_msgs/Point.h"
#include "RokiHeader.h"
#include "Environment.hpp"
#include "AIManager.hpp"
#include "SimuManager.hpp"


namespace rokicar{

class BaseRoboCar
{
public:
	BaseRoboCar(Box _size, Vector3 _pos, Quaternion _quat, string _ai_name, string _simu_name, int _id=NULL_CAR_ID, RoboType _type=ROKI_CAR);
	~BaseRoboCar() {};

	int tick(Environment* envInfo);  		 // ai_tick
	int simulate(Environment* envInfo, double delta_t);		 // 物理模拟，这个真的需要么？

public:  // for more easyly getting data
	int 		id;
	RoboType	type;
	Box			size;
	Vector3		position;
	Quaternion	quat;
	double		speed;
	double		rota_radius;  // 模拟的转弯半径，即roki会在以speed速度在rota_radius的圆上跑
	BaseAI*		ai;
	BaseSimu*	simu;

	int			bornTime;
	
};

/*** 主线机器人:便于管理  ***/
class MainCar : public BaseRoboCar {
public:
	MainCar(Box _size, Vector3 _pos, Quaternion _quat):
	BaseRoboCar(_size, _pos, _quat, "", "", MAIN_CAR_ID, MAIN_CAR) {};
};

/*** roki 机器人 ***/
class RokiRoboCar : public BaseRoboCar {
public:
	RokiRoboCar(Box _size, Vector3 _pos, Quaternion _quat, string ai_name, string simu_name, double _speed=0):
	BaseRoboCar(_size, _pos, _quat, ai_name, simu_name, NULL_CAR_ID, ROKI_CAR) {
		speed = _speed;
	};
};
};