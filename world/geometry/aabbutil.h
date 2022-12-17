

#ifndef AABBUTIL_H
#define AABBUTIL_H

namespace gpp
{

bool aabbOverlap(const vector3d& v1, float r1, const vector3d& v2, float r2);
bool aabbOverlap(const vector3d& center, float radius, AABB* b);
bool aabbOverlap(AABB* a, AABB* b);
bool aabbInsideAll(const vector3d& v1, float r1, const vector3d& v2, float r2);
bool aabbInsideAll(const vector3d& center, float radius, AABB* b);
bool aabbInsideAll(AABB* a, AABB* b);
bool aabbInsidePoint(AABB* a, const vector3d& v);
}
#endif
