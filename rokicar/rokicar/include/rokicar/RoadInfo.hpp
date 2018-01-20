#pragma once
#include "RokiHeader.h"
#include "hadmap_navigation/NavMap.h"

namespace rokicar {

class RoadInfo
{
public:
	RoadInfo();
	virtual ~RoadInfo();

	void update(const hadmap_navigation::NavMap::ConstPtr &new_map);
	Vector3 get_base_point();
	inline vector< vector< vector<Vector3> > >* get_data() {return &sections_lanes_centers;}

private:
	vector< vector< vector<Vector3> > > sections_lanes_centers; // 当前道路的各个车道中心点集

};

}