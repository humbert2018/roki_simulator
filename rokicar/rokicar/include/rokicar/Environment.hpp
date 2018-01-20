#pragma once
#include "RokiHeader.h"
#include "BaseRoboCar.hpp"
#include "RoadInfo.hpp"

namespace rokicar{

class Environment
{
public:
	Environment();
	virtual ~Environment();

	bool add_robot(BaseRoboCar* robo);  // 添加一个机器人
	void update_cars();					// 模拟器rokicar的物理模拟与ai脚本
	void update_main_car(Vector3 pos, Quaternion quat);  // 主车的位置信息，来自hadmap相关msg驱动
	// 道路信息更新，来自hadmap相关msg驱动
	inline void update_road(const hadmap_navigation::NavMap::ConstPtr &global_map){
		road_info->update(global_map);
	}
	inline RoadInfo* get_road_info() {return road_info;};
	inline BaseRoboCar* get_main_car(){
		return all_cars.find(MAIN_CAR_ID) == all_cars.end()? nullptr : all_cars[MAIN_CAR_ID];
	}
	inline BaseRoboCar* get_car(int id){
		return all_cars.find(id) == all_cars.end()? nullptr : all_cars[id];
	}
	vector<BaseRoboCar*> get_all_rokicars();

private:
	void ai_tick(double now_time);						// 模拟器ai_tick(update_cars决定合适调用)
	void simu_tick(double now_time);					// 物理模拟
	void gc_tick(double now_time);						// 清除无效肉鸡的tick，目前无效就是离主车太远的肉鸡

private:
	map<int, BaseRoboCar*>	all_cars;   // 主机器人会被封装成一个BaseRoboCar
	RoadInfo*				road_info;
	double					last_ai_tick_time;
	double					last_simu_tick_time;
	double					last_gc_tick_time;
	
};

}