

#ifndef GPP_COLLISION3D_H
#define GPP_COLLISION3D_H

#include"../common.h"
#include"../math/math.h"
#include"../geometry/geometry.h"
#include"CollisionInfo.h"
#include"CollisionTracker.h"

namespace gpp{
bool checkSphereSphereCollision(Sphere3d* sphereA, Sphere3d* sphereB, CollisionInfo* info);
bool checkCapsuleCapsuleCollision(Capsule3d* capsuleA, Capsule3d* capsuleB, CollisionInfo* info);
bool checkSphereCapsuleCollision(Sphere3d* sphere, Capsule3d* capsule, CollisionInfo* info);
bool checkBoxSphereCollision(Box3d* box, Sphere3d* sphere, CollisionInfo* info);
bool checkBoxCapsuleCollision(Box3d* box, Capsule3d* capsule, CollisionInfo* info);
bool checkBoxBoxCollision(Box3d* boxA, Box3d* boxB, CollisionInfo* info);
bool detectCollisionWith(RigidBody* r1, RigidBody* r2, CollisionInfo* info);
}
#endif
