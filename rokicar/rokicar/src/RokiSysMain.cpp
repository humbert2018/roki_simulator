#include "ros/ros.h"
#include "std_msgs/String.h"
#include "pnc_msgs/VehicleState.h"
#include "RokiHeader.h"
#include "RokiSystem.hpp"

using namespace rokicar;

RokiSystem* sys = nullptr;


void easy_vehicle_callback(const pnc_msgs::VehicleState::ConstPtr &vehicle_state){
	double x = vehicle_state->pose.translation.x;
    double y = vehicle_state->pose.translation.y;
    double z = vehicle_state->pose.translation.z;
    geometry_msgs::Quaternion q = vehicle_state->pose.rotation;

	sys->get_environment()->update_main_car(Vector3(x, y, z), Quaternion(q.x, q.y, q.z, q.w));  // 更新主车信息
}

void easy_nav_map_callback(const hadmap_navigation::NavMap::ConstPtr &global_map){
	sys->get_environment()->update_road(global_map);
}

void create_new_rokicar_callback(const rokicar::RokiCreatorMsg::ConstPtr &create_info){
	// ROS_ERROR("CREATE:  %s %s %lf %lf %lf", 
	// create_info->ai_name.data(), create_info->simu_name.data(), create_info->pos.x, create_info->pos.y, create_info->pos.z);
	// ROS_ERROR("Base %lf %lf", base_point_.x, base_point_.y);
	// ROS_ERROR("MainCar %lf %lf", 
	// sys->get_environment()->get_main_car()->position.x(), sys->get_environment()->get_main_car()->position.y());
	Vector3 base_point_ = sys->get_environment()->get_road_info()->get_base_point();  // 修正位置偏移
	Box box(create_info->box.x, create_info->box.y, create_info->box.z);
	Vector3 pos(create_info->pos.x + base_point_.x(), create_info->pos.y + base_point_.y(), create_info->pos.z + base_point_.z());
	Quaternion quat(create_info->quat.x, create_info->quat.y, create_info->quat.z, create_info->quat.w);
	BaseRoboCar* car = new RokiRoboCar(box, pos, quat, create_info->ai_name.data(), create_info->simu_name.data(), create_info->speed);
	sys->get_environment()->add_robot(car);
}

// rokicar msg publisher: Marker msg
int main(int argc, char *argv[])
{
	ros::init(argc, argv, "RokiSys");
	sys = new RokiSystem();
	Environment* env = sys->get_environment();

	ros::NodeHandle node;
	ros::Subscriber vehicle = node.subscribe("/pnc_msgs/vehicle_state", 1, easy_vehicle_callback);
	ros::Subscriber hadmap = node.subscribe("hadmap_navigation/global_map", 1, easy_nav_map_callback);
	ros::Subscriber creator = node.subscribe("/rokicar/create", 1, create_new_rokicar_callback);

	ros::Publisher marker_pub = node.advertise<visualization_msgs::Marker>( "visualization_marker", 0);
	ros::Publisher roki_info_pub = node.advertise<perception_msgs::Objects>( "roki_info", 0);

	while(ros::ok()){
		env->update_cars();
		if(sys->check_nxt_show_time()){
			vector<BaseRoboCar*> cars = env->get_all_rokicars();
			// 发送marker数据
			for(BaseRoboCar* car : cars){
				marker_pub.publish(sys->create_roki_marker_msg(car->id));
			}
			// 肉鸡车伪造的obj信息
	        roki_info_pub.publish(sys->create_roki_info_msg());
			sys->update_nxt_show_time();
		}
		ros::spinOnce();  
	}
	return 0;
}