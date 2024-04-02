


#ifndef MATHCOLLISION_H
#define MATHCOLLISION_H

namespace gpp{

bool computeSphereSphereCollision(const vector3d& c1, decimal r1, const vector3d& c2, decimal r2, vector3d& point, vector3d& normal, decimal& depth);
bool computeSphereBoxCollision(const vector3d& lp, const vector3d& closestPoint, const vector3d& center, decimal radius, const vector3d& min, const vector3d& max, vector3d& point, vector3d& normal, decimal& depth);
bool computeBoxSphereCollision(const vector3d& lp, const vector3d& closestPoint, const vector3d& min, const vector3d& max, const vector3d& center, decimal radius, vector3d& point, vector3d& normal, decimal& depth);
}
#endif
