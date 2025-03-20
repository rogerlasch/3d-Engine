

#ifndef COLLISIONDISPATCHER_H
#define COLLISIONDISPATCHER_H

namespace gpp{

class CollisionDispatcher{
public:
static bool dispatch(GeometricShape* s1, GeometricShape* s2, CollisionInfo* info);
static bool rayCast(const vector3d& origin, const vector3d& dir, GeometricShape* sh, RayInfo* info);
};
}
#endif
