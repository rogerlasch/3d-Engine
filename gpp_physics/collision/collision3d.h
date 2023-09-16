
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
static bool isColliding(GeometricShape* s1, GeometricShape* s2, CollisionInfo* info);
//sphere...
static bool sphereSphere(sphere3d* s1, sphere3d* s2, CollisionInfo* info);
static bool sphereBox(sphere3d* s, box3d* b, CollisionInfo* info);
static bool sphereCapsule(sphere3d* s, capsule3d* c, CollisionInfo* info);

//box...
static bool boxBox(box3d* b1, box3d* b2, CollisionInfo* info);
static bool boxSphere(box3d* b, sphere3d* s, CollisionInfo* info);
static bool boxCapsule(box3d* b, capsule3d* c, CollisionInfo* info);

//capsule...
static bool capsuleCapsule(capsule3d* c1, capsule3d* c2, CollisionInfo* info);
static bool capsuleSphere(capsule3d* c, sphere3d* s, CollisionInfo* info);
static bool capsuleBox(capsule3d* c, box3d* b, CollisionInfo* info);
};
}
#endif
