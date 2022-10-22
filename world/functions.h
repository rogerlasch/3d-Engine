

#ifndef GPP_FUNCTIONS_H
#define GPP_FUNCTIONS_H

#include<vector>
namespace gpp
{

bool gpp_find_nearest_point(const std::vector<vector3d>& points, const vector3d& point, vector3d* outv);
void gpp_rotate_points(std::vector<vector3d>& points, float angle, const vector3d& origin);
void gpp_rotate_single_point(vector3d& point, float angle, const vector3d& origin);
}
#endif
