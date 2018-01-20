#pragma once
#include "RokiHeader.h"
#include "Environment.hpp"

namespace rokicar
{

class RokiSystem
{
public:
	RokiSystem();
	virtual ~RokiSystem();
	inline Environment* get_environment() {return env;}
	inline bool check_nxt_show_time() {return ros::Time::now().toSec() > nxt_show_rokicars_time;};
	inline void update_nxt_show_time() {nxt_show_rokicars_time = ros::Time::now().toSec() + ROKI_SHOW_INTERVAL;};

	visualization_msgs::Marker create_roki_marker_msg(int roki_id);
	perception_msgs::Objects create_roki_info_msg();

private:
	Environment*		env;
	double				nxt_show_rokicars_time;
	
};

}