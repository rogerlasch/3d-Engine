


#ifndef GPP_MATH_H
#define GPP_MATH_H

#include <type_traits>
#include<initializer_list>
#include <array>
#include<vector>
#include"../types.h"
#include"vector3d.h"
#include"matrix3x3.h"
#include"matrix4x4.h"
#include"quaternion.h"
#include"RayInfo.h"
#include"ray.h"
#include<format>
#include"format_math_overloads.h"


namespace gpp{

#define GPP_PI 3.1415926f
#define GPP_EPSILON 0.00001f
#define GPP_DISTANCE_TOLERANCE 0.01f

decimal degrees_to_radians(decimal f);
decimal radians_to_degrees(decimal r);
decimal gpp_clamp(decimal n, decimal min, decimal max);
decimal ClosestPtSegmentSegment(const vector3d& p1, const vector3d& q1, const vector3d& p2, const vector3d& q2,                                              decimal &s, decimal &t, vector3d& c1, vector3d& c2);
decimal ClosestPtPointSegment(const vector3d& c, const vector3d& a, const vector3d& b, decimal &t, vector3d &d);
                 decimal SqDistPointSegment(const vector3d& a, const vector3d& b, const vector3d& c);
void closestPointFromTriangle(const vector3d& p, const vector3d& a, const vector3d& b, const vector3d& c, vector3d& pt);
void calculateBoundingBox(const std::vector<vector3d>& points, vector3d& min, vector3d& max);
bool point_in_box(const vector3d& p, const vector3d& min, const vector3d& max);
vector3d get_closest_point_from_aabb(const vector3d& pt, const vector3d& min, const vector3d& max);
decimal get_distance_from_aabb(const vector3d& pt, const vector3d& min, const vector3d& max);
bool point_in_aabb(const vector3d& pt, const vector3d& min, const vector3d& max);
bool aabbOverlap(const vector3d& v1, decimal r1, const vector3d& v2, decimal r2);
bool aabbOverlap(const vector3d& center, decimal r, const vector3d& min, const vector3d& max);
bool aabbOverlap(const vector3d& v1, const vector3d& v2, const vector3d& v3, const vector3d& v4);
bool aabbInsideAll(const vector3d& v1, decimal r1, const vector3d& v2, decimal r2);
bool aabbInsideAll(const vector3d& center, decimal r, const vector3d& min, const vector3d& max);
bool aabbInsideAll(const vector3d& v1, const vector3d& v2, const vector3d& v3, const vector3d& v4);
}
#endif
