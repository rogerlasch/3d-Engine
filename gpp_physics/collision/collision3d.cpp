
#include <limits>
#include"../common.h"
#include"../gpp_physics.h"
#include"collision3d.h"

using namespace std;

namespace gpp
{
bool collision3d::isColliding(GeometricShape* s1, GeometricShape* s2, CollisionInfo* info)
{
_GASSERT(s1!=NULL);
_GASSERT(s2!=NULL);
if((info->r1->mass<=0.0f)&&(info->r2->mass>0)){
swap(info->r1, info->r2);
swap(s1, s2);
}
switch((s1->getGeometricType()|s2->getGeometricType()))
{
case GTYPE_SPHERE:
return sphereSphere((sphere3d*)s1, (sphere3d*)s2, info);
case GTYPE_SPHERE|GTYPE_BOX:
{
box3d* b=((s1->geometricType==GTYPE_BOX) ? (box3d*)s1 : (box3d*)s2);
sphere3d* s=((s1->geometricType==GTYPE_SPHERE) ? (sphere3d*)s1 : (sphere3d*)s2);
return ((s1->geometricType==GTYPE_SPHERE) ? sphereBox(s, b, info) : boxSphere(b, s, info));
}
case GTYPE_SPHERE|GTYPE_CAPSULE:
{
capsule3d* c=((s1->geometricType==GTYPE_CAPSULE) ? (capsule3d*)s1 : (capsule3d*)s2);
sphere3d* s=((s1->geometricType==GTYPE_SPHERE) ? (sphere3d*)s1 : (sphere3d*)s2);
return ((s1->geometricType==GTYPE_SPHERE) ? sphereCapsule(s, c, info) : capsuleSphere(c, s, info));
}
case GTYPE_BOX:
return boxBox((box3d*)s1, (box3d*)s2, info);
case GTYPE_BOX|GTYPE_CAPSULE:{
capsule3d* c=((s1->geometricType==GTYPE_CAPSULE) ? (capsule3d*)s1 : (capsule3d*)s2);
box3d* b=((s1->geometricType==GTYPE_BOX) ? (box3d*)s1 : (box3d*)s2);
return ((s1->geometricType==GTYPE_BOX) ? boxCapsule(b, c, info) : capsuleBox(c, b, info));
}
}
return false;
}

bool collision3d::sphereSphere(sphere3d* s1, sphere3d* s2, CollisionInfo* info)
{
return computeSphereSphereCollision(s1->center, s1->radius, s2->center, s2->radius, info->point, info->normal, info->depth);
}

bool collision3d::sphereBox(sphere3d* s, box3d* b, CollisionInfo* info)
{
//Recuperar o ponto mais próximo da caixa em relação ao centro da esfera...
vector3d closestPoint=get_closest_point_from_aabb(s->center, b->min, b->max);
return computeSphereBoxCollision(s->getLastPosition(), closestPoint, s->center, s->radius, b->min, b->max, info->point, info->normal, info->depth);
}

bool collision3d::sphereCapsule(sphere3d* s, capsule3d* c, CollisionInfo* info)
{
vector3d closestPoint;
float t=0.0f;
ClosestPtPointSegment(s->center, c->min, c->max, t, closestPoint);
return computeSphereSphereCollision(s->center, s->radius, closestPoint, c->radius, info->point, info->normal, info->depth);
}

//Box...

bool collision3d::boxBox(box3d* b1, box3d* b2, CollisionInfo* info)
{
    // Verificar colisão entre caixas
    if (b1->max.x < b2->min.x || b1->min.x > b2->max.x ||
        b1->max.y < b2->min.y || b1->min.y > b2->max.y ||
        b1->max.z < b2->min.z || b1->min.z > b2->max.z) {
        return false; // Sem colisão
    }

    // Determinar a normal de colisão (usando o eixo da sobreposição mínima)
    vector3d collisionNormal;
    // Calcule a normal de colisão aqui
float overlapX = min(b1->max.x - b2->min.x, b2->max.x - b1->min.x);
float overlapY = min(b1->max.y - b2->min.y, b2->max.y - b1->min.y);
float overlapZ = min(b1->max.z - b2->min.z, b2->max.z - b1->min.z);

if (overlapX < overlapY && overlapX < overlapZ) {
    collisionNormal = vector3d(overlapX, 0.0f, 0.0f);
} else if (overlapY < overlapX && overlapY < overlapZ) {
    collisionNormal = vector3d(0.0f, overlapY, 0.0f);
} else {
    collisionNormal = vector3d(0.0f, 0.0f, overlapZ);
}

// Leva em consideração a direção da colisão
if (b2->min.x < b1->min.x) collisionNormal.x *= -1.0f;
if (b2->min.y < b1->min.y) collisionNormal.y *= -1.0f;
if (b2->min.z < b1->min.z) collisionNormal.z *= -1.0f;
collisionNormal.normalize();
    // Calcular o ponto de colisão (usando média das coordenadas)
    vector3d collisionPoint;
collisionPoint.x = (b2->min.x + b2->max.x) * 0.5f;
collisionPoint.y = (b2->min.y + b2->max.y) * 0.5f;
collisionPoint.z = (b2->min.z + b2->max.z) * 0.5f;

    // Determinar a profundidade de penetração (usando a sobreposição mínima)
float depthX = min(b1->max.x - b2->min.x, b2->max.x - b1->min.x);
float depthY = min(b1->max.y - b2->min.y, b2->max.y - b1->min.y);
float depthZ = min(b1->max.z - b2->min.z, b2->max.z - b1->min.z);
float depth = min(depthX, min(depthY, depthZ));

    info->normal = collisionNormal;
    info->point = collisionPoint;
    info->depth = depth;

    return true; // Houve colisão
}

bool collision3d::boxSphere(box3d* b, sphere3d* s, CollisionInfo* info){
vector3d closestPoint=get_closest_point_from_aabb(s->center, b->min, b->max);
return computeBoxSphereCollision(b->getLastPosition(), closestPoint, b->min, b->max, s->center, s->radius, info->point, info->normal, info->depth);
}

bool collision3d::boxCapsule(box3d* b, capsule3d* c, CollisionInfo* info){
vector3d pt;
float t;
ClosestPtPointSegment(b->getCenter(), c->min, c->max, t, pt);
vector3d closestPoint=get_closest_point_from_aabb(pt, b->min, b->max);
return computeBoxSphereCollision(b->getLastPosition(), closestPoint, b->min, b->max, pt, c->radius, info->point, info->normal, info->depth);
}

//Capsule

bool collision3d::capsuleCapsule(capsule3d* c1, capsule3d* c2, CollisionInfo* info)
{
float s, t=0.0f;
vector3d v1, v2;
float sqdist=ClosestPtSegmentSegment(c1->min, c1->max, c2->min, c2->max, s, t, v1, v2);
return computeSphereSphereCollision(v1, c1->radius, v2, c2->radius, info->point, info->normal, info->depth);
}

bool collision3d::capsuleSphere(capsule3d* c, sphere3d* s, CollisionInfo* info)
{
vector3d closestPoint;
float t=0.0f;
ClosestPtPointSegment(s->center, c->min, c->max, t, closestPoint);
return computeSphereSphereCollision(closestPoint, c->radius, s->center, s->radius, info->point, info->normal, info->depth);
}

bool collision3d::capsuleBox(capsule3d* c, box3d* b, CollisionInfo* info)
{
vector3d pt;
float t;
ClosestPtPointSegment(b->getCenter(), c->min, c->max, t, pt);
vector3d closestPoint=get_closest_point_from_aabb(pt, b->min, b->max);
return computeSphereBoxCollision(c->getLastPosition(), closestPoint, pt, c->radius, b->min, b->max, info->point, info->normal, info->depth);
}
}
