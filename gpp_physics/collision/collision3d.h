
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
static bool sphereSphere(sphere3d* s1, sphere3d* s2, CollisionInfo* info);
static bool sphereBox(sphere3d* s, box3d* b, CollisionInfo* info);
static bool boxBox(box3d* b1, box3d* b2, CollisionInfo* info);
static bool rayPoint(const vector3d& origin, const vector3d& dir, const vector3d& point);
static bool raySphere(const vector3d& origin, const vector3d& dir, sphere3d* s, vector3d& q);
static bool rayBox(const vector3d& origin, const vector3d& dir, box3d* b, vector3d& q, bool calculateEntry);
};
}
#endif
