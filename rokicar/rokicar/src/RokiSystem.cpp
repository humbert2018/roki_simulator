#include "RokiSystem.hpp"
#include "AIManager.hpp"
#include "SimuManager.hpp"
namespace rokicar
{

RokiSystem::RokiSystem()
{
    env = new Environment();
    AIManager::InitAll();
    SimuManager::InitAll();
    nxt_show_rokicars_time = 0;
}

RokiSystem::~RokiSystem()
{
    if(env != nullptr) delete env;
    env = nullptr;
}

visualization_msgs::Marker RokiSystem::create_roki_marker_msg(int roki_id) {
    visualization_msgs::Marker marker;
    BaseRoboCar* car = env->get_car(roki_id);
    if(car == nullptr) return marker;

    Vector3 pos = car->position;
    Quaternion quat = car->quat;
    Box size = car->size;
    
	marker.header.frame_id = "map";
	marker.header.stamp = ros::Time();
	marker.ns = "rokicar";
	marker.id = roki_id;
	marker.type = visualization_msgs::Marker::CUBE;
	marker.action = visualization_msgs::Marker::ADD;
	Vector3 base_point_ = env->get_road_info()->get_base_point();
	marker.pose.position.x = pos.x() - base_point_.x(); // 坐标调整
	marker.pose.position.y = pos.y() - base_point_.y();
	marker.pose.position.z = pos.z();
	marker.pose.orientation.x = quat.x();
	marker.pose.orientation.y = quat.y();
	marker.pose.orientation.z = quat.z();
	marker.pose.orientation.w = quat.w();
	marker.scale.x = size.x();
	marker.scale.y = size.y();
	marker.scale.z = size.z();
	marker.color.a = 1.0; // Don't forget to set the alpha!
	marker.color.r = 1.0;
	marker.color.g = 0.0;
	marker.color.b = 0.0;
    // set rokicar mesh
	// marker.mesh_resource = "package://pr2_description/meshes/base_v0/base.dae";
    return marker;
}

perception_msgs::Objects RokiSystem::create_roki_info_msg()
{
	int now_sec = ros::Time().now().sec;
    perception_msgs::Objects objs;

    objs.header.frame_id = "rokicar";
	objs.header.stamp = ros::Time();
	objs.second = ros::Time().sec;
	objs.fra_second = ros::Time().nsec / 1000;

	vector<BaseRoboCar*> cars = env->get_all_rokicars();
	for(BaseRoboCar* car : cars)
	{
		tf::Transform car_transform;
        car_transform.setRotation(car->quat);
        Vector3 ahead = car_transform(Vector3(car->speed, 0, 0));

		perception_msgs::Object obj;
		obj.id = car->id;  // id
		obj.age = now_sec - car->bornTime;  // 存活时间
		obj.prediction_age = 100000; // 永远活着？

		obj.reference_point;// 参考点?
		obj.bounding_box_center;
        obj.bounding_box_size;
        obj.object_box_center;

		obj.object_box_size.x = car->size.x();
		obj.object_box_size.y = car->size.y();
		obj.object_box_size.z = car->size.z();

		obj.object_box_orientation;
		obj.object_box_orientation_absolute;

        obj.absolute_velocity.x = ahead.x();
		obj.absolute_velocity.y = ahead.y();
		obj.absolute_velocity.z = ahead.z();

		obj.absolute_velocity_sigma; //
		obj.relative_velocity;
		
		obj.absolute_acceleration;
		obj.relative_acceleration;
		
		obj.absolute_yaw_rate;
		obj.relative_yaw_rate;

		obj.classification;
		obj.classification_age;
		obj.classification_certainty;
		obj.contour_points;
		obj.contour_points_absolute;


		//
		obj.bounding_box_center.x = car->position.getX();
		obj.bounding_box_center.y = car->position.getY();
		obj.bounding_box_center.z = car->position.getZ();


		obj.pos_x = car->position.getX();
		obj.pos_y = car->position.getY();
		obj.pos_z = car->position.getZ();
		//

		objs.objects.push_back(obj);
	}

    return objs;
}

}
