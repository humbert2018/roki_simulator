#include "RoadInfo.hpp"
#include "geometry_msgs/Point.h"

namespace rokicar {
	


RoadInfo::RoadInfo()
{
    sections_lanes_centers.clear();
}

RoadInfo::~RoadInfo()
{
}

void RoadInfo::update(const hadmap_navigation::NavMap::ConstPtr &new_map) {
    // road info 只记录每个section中各条路的中心线;
    // 用了一个3层vector的结构暴力储存
    ROS_ERROR("road info update, section size: %d", new_map->sections.size());
    sections_lanes_centers.resize(new_map->sections.size());
    for(int sec = 0; sec < sections_lanes_centers.size(); sec ++){
        sections_lanes_centers[sec].resize(new_map->sections[sec].lanes.size());
        for(int lan = 0; lan < sections_lanes_centers[sec].size(); lan ++){
            sections_lanes_centers[sec][lan].resize(new_map->sections[sec].lanes[lan].pts_center.size());
            for(int pt = 0; pt < sections_lanes_centers[sec][lan].size(); pt ++){
                geometry_msgs::Point point = new_map->sections[sec].lanes[lan].pts_center[pt].point;
                sections_lanes_centers[sec][lan][pt] = Vector3(point.x, point.y, point.z);
            }
        }
    }
}

Vector3 RoadInfo::get_base_point() {
    // 基础参考点： rviz显示时给market修正坐标用的。
    if(sections_lanes_centers.size() <= 0) return Vector3(0, 0, 0);
    return sections_lanes_centers[0][0][0];
}

}