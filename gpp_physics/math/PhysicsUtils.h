

#ifndef PHYSICSUTILS_H
#define PHYSICSUTILS_H

namespace gpp{

vector3d calculateGravitationalForce(const vector3d& position1, const vector3d& position2, decimal mass1, decimal mass2, decimal gravitationalConstant = 9.81f);
decimal calculateFrictionForce(decimal normalForce, decimal coefficientOfFriction);
vector3d calculateNetForce(const vector3d& force1, const vector3d& force2); 
vector3d calculateImpulse(const vector3d& velocity, decimal mass); 
bool checkCollision(const vector3d& position1, decimal radius1, const vector3d& position2, decimal radius2, vector3d& collisionNormal); 
void resolveCollision(vector3d& velocity1, vector3d& velocity2, decimal mass1, decimal mass2, const vector3d& normal); 
decimal calculateKineticEnergy(decimal mass, const vector3d& velocity); 
decimal calculateMomentOfInertiaSphere(decimal mass, decimal radius); 
bool checkStability(const vector3d& position, const quaternion& orientation, decimal stabilityThreshold); 
decimal calculateAngleBetweenVectors(const vector3d& vector1, const vector3d& vector2);
vector3d calculateLinearAcceleration(const vector3d& force, decimal mass); 
vector3d updateLinearVelocity(const vector3d& velocity, const vector3d& acceleration, decimal deltaTime); 
vector3d integratePosition(const vector3d& position, const vector3d& velocity, decimal deltaTime); 
vector3d calculateAngularAcceleration(const vector3d& torque, decimal inertia); 
vector3d updateAngularVelocity(const vector3d& angularVelocity, const vector3d& angularAcceleration, decimal deltaTime); 
quaternion integrateRotation(const quaternion& rotation, const vector3d& angularVelocity, decimal deltaTime); 
matrix3x3 updateInertiaTensor(const matrix3x3& inertiaTensor, const quaternion& rotation); 
quaternion applyTorqueAndUpdateRotation(vector3d& angularVelocity, const vector3d& torque, decimal inertia, quaternion& rotation, decimal deltaTime); 
}
#endif
