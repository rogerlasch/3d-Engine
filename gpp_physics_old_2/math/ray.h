


#ifndef GPP_RAY_H
#define GPP_RAY_H

namespace gpp{

bool raySphere(const vector3d& origin, const vector3d& dir, const vector3d& center, float radius, vector3d& in, vector3d& out);
bool rayBox(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, vector3d& in, vector3d& out);
bool rayCylinder(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, float radius, vector3d& in, vector3d& out);
bool rayCapsule(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, float radius, vector3d& in, vector3d& out);
}
#endif
