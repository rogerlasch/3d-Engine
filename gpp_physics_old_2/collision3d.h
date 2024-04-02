
/**
*Classe que implementa diversos métodos de colisão entre geometrias.
*No momento, apenas colisão entre esferas, caixas e linhas.
**/
#ifndef COLLISION3D_H
#define COLLISION3D_H

namespace gpp
{

class collision3d
{
public:
static bool isColliding(RigidBody* s1, RigidBody* s2, CollisionInfo* info);
//Sphere...
static bool sphereSphere(Sphere3d* s1, Sphere3d* s2, CollisionInfo* info);
static bool sphereBox(Sphere3d* s, Box3d* b, CollisionInfo* info);
static bool sphereCapsule(Sphere3d* s, Capsule3d* c, CollisionInfo* info);

//Box...
static bool boxBox(Box3d* b1, Box3d* b2, CollisionInfo* info);
static bool boxSphere(Box3d* b, Sphere3d* s, CollisionInfo* info);
static bool boxCapsule(Box3d* b, Capsule3d* c, CollisionInfo* info);

//Capsule
static bool capsuleCapsule(Capsule3d* c1, Capsule3d* c2, CollisionInfo* info);
static bool capsuleSphere(Capsule3d* c, Sphere3d* s, CollisionInfo* info);
static bool capsuleBox(Capsule3d* c, Box3d* b, CollisionInfo* info);

};
}
#endif
