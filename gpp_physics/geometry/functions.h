

#ifndef GPP_FUNCTIONS_H
#define GPP_FUNCTIONS_H

#include<vector>
namespace gpp
{

class AABB;
std::string floatToString(float n);
float degrees_to_radians(float f);
float radians_to_degrees(float r);
float ClosestPtSegmentSegment(const vector3d& p1, const vector3d& q1, const vector3d& p2, const vector3d& q2,  float& s, float& t, vector3d& c1, vector3d& c2);
void ClosestPtPointSegment(const vector3d& c, const vector3d& a, const vector3d& b, float &t, vector3d &d);
                 float SqDistPointSegment(const vector3d& a, const vector3d& b, const vector3d& c);
void calculateBoundingBox(const std::vector<vector3d>& points, AABB& b);
}
#endif
