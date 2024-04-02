


#ifndef GPP_FFUNCTIONS_H
#define GPP_FFUNCTIONS_H

namespace gpp{

vector3d fCalculateAirDrag(const vector3d& velocity, float projectedArea, float airDencity, float airDragCoefficient);
vector3d fCalculateTangent(const vector3d& collisionNormal);
vector3d fCalculateTangentialVelocity(const vector3d& velocity, const vector3d& collisionTangent);
vector3d fCalculateSpringForce(const vector3d& p1, const vector3d& v1, const vector3d& p2, const vector3d& v2, float restLength, float stiffness, float damping);
vector3d fCalculateStaticFriction(const vector3d& force, const vector3d& velocity, float staticFriction);
vector3d fCalculateKineticFriction(const vector3d& force, const vector3d& velocity, float kineticFriction);
}
#endif
