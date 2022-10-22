

#ifndef GPP_FUNCTIONS_H
#define GPP_FUNCTIONS_H

#include<vector>
namespace gpp
{

bool gpp_find_nearest_point(const std::vector<vector3d>& points, const vector3d& point, vector3d* outv);
void gpp_rotate_points(std::vector<vector3d>& points, float angle, const vector3d& origin);
void gpp_rotate_single_point(vector3d& point, float angle, const vector3d& origin);
float ClosestPtSegmentSegment(const vector3d& p1, const vector3d& q1, const vector3d& p2, const vector3d& q2,  float& s, float& t, vector3d& c1, vector3d& c2);
}
#endif
