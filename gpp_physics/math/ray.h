


#ifndef GPP_RAY_H
#define GPP_RAY_H

namespace gpp{

bool raySphereAUX(const vector3d& origin, const vector3d& dir, const vector3d& center, decimal radius, decimal& tMin, decimal& tMax);
bool raySphere(const vector3d& origin, const vector3d& dir, const vector3d& center, decimal radius, RayInfo* info);
bool rayBox(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, RayInfo* info);
bool rayCylinder(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, decimal radius, RayInfo* info);
bool rayCapsule(const vector3d& origin, const vector3d& dir, const vector3d& cPos, const vector3d& axis, decimal length, decimal radius, RayInfo* info);
    bool rayOBB(const vector3d& origin, const vector3d& dir, const vector3d& center, const vector3d& alf, const std::array<vector3d, 3>& axis, RayInfo* info);
}
#endif
