


#ifndef GPP_MATH_H
#define GPP_MATH_H

#include<vector>
#include"vector3d.h"
#include"matrix3x3.h"
#include"quaternion.h"
#include"ray.h"
#include"MathCollision.h"


namespace gpp{

#define GPP_PI 3.1415926
#define GPP_EPSILON 0.0000001f
#define GPP_DISTANCE_TOLERANCE 0.01f

float degrees_to_radians(float f);
float radians_to_degrees(float r);
float gpp_clamp(float n, float min, float max);
float ClosestPtSegmentSegment(const vector3d& p1, const vector3d& q1, const vector3d& p2, const vector3d& q2,                                              float &s, float &t, vector3d& c1, vector3d& c2);
float ClosestPtPointSegment(const vector3d& c, const vector3d& a, const vector3d& b, float &t, vector3d &d);
                 float SqDistPointSegment(const vector3d& a, const vector3d& b, const vector3d& c);
void calculateBoundingBox(const std::vector<vector3d>& points, vector3d& min, vector3d& max);
bool point_in_box(const vector3d& p, const vector3d& min, const vector3d& max);
}
#endif
