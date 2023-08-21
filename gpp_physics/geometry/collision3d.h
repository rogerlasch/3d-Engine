
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
static bool isColliding(GeometricShape* s1, GeometricShape* s2, CollisionInfo* data);
static bool sphereSphere(sphere3d* s1, sphere3d* s2, CollisionInfo* data);
static bool sphereBox(sphere3d* s, box3d* b, CollisionInfo* data);
static bool boxBox(box3d* b1, box3d* b2, CollisionInfo* data);
};
}
#endif
