
#include <limits>
#include<gpp/debug_system.h>
#include"types.h"
#include"math/math.h"
#include"RigidBody.h"
#include"Box3d.h"
#include"Capsule3d.h"
#include"Sphere3d.h"
#include"CollisionInfo.h"
#include"collision3d.h"

using namespace std;

namespace gpp
{
bool collision3d::isColliding(RigidBody* s1, RigidBody* s2, CollisionInfo* info)
{
    _GASSERT(s1 != nullptr);
    _GASSERT(s2 != nullptr);
    info->r1 = s1;
    info->r2 = s2;

    if ((info->r1->getMass() <= 0.0f) && (info->r2->getMass() > 0)) {
        std::swap(info->r1, info->r2);
        std::swap(s1, s2);
    }

    switch ((s1->getGeometricType() | s2->getGeometricType()))
    {
    case GTYPE_SPHERE:
        return sphereSphere(dynamic_cast<Sphere3d*>(s1), dynamic_cast<Sphere3d*>(s2), info);
    case GTYPE_SPHERE | GTYPE_BOX:
    {
        Box3d* b = ((s1->getGeometricType() == GTYPE_BOX) ? dynamic_cast<Box3d*>(s1) : dynamic_cast<Box3d*>(s2));
        Sphere3d* s = ((s1->getGeometricType() == GTYPE_SPHERE) ? dynamic_cast<Sphere3d*>(s1) : dynamic_cast<Sphere3d*>(s2));
        return ((s1->getGeometricType() == GTYPE_SPHERE) ? sphereBox(s, b, info) : boxSphere(b, s, info));
    }
    case GTYPE_SPHERE | GTYPE_CAPSULE:
    {
        Capsule3d* c = ((s1->getGeometricType() == GTYPE_CAPSULE) ? dynamic_cast<Capsule3d*>(s1) : dynamic_cast<Capsule3d*>(s2));
        Sphere3d* s = ((s1->getGeometricType() == GTYPE_SPHERE) ? dynamic_cast<Sphere3d*>(s1) : dynamic_cast<Sphere3d*>(s2));
        return ((s1->getGeometricType() == GTYPE_SPHERE) ? sphereCapsule(s, c, info) : capsuleSphere(c, s, info));
    }
    case GTYPE_BOX:
        return boxBox(dynamic_cast<Box3d*>(s1), dynamic_cast<Box3d*>(s2), info);
    case GTYPE_BOX | GTYPE_CAPSULE:
    {
        Capsule3d* c = ((s1->getGeometricType() == GTYPE_CAPSULE) ? dynamic_cast<Capsule3d*>(s1) : dynamic_cast<Capsule3d*>(s2));
        Box3d* b = ((s1->getGeometricType() == GTYPE_BOX) ? dynamic_cast<Box3d*>(s1) : dynamic_cast<Box3d*>(s2));
        return ((s1->getGeometricType() == GTYPE_BOX) ? boxCapsule(b, c, info) : capsuleBox(c, b, info));
    }
    case GTYPE_CAPSULE:
        return capsuleCapsule(dynamic_cast<Capsule3d*>(s1), dynamic_cast<Capsule3d*>(s2), info);
    }
    return false;
}

bool collision3d::sphereSphere(Sphere3d* s1, Sphere3d* s2, CollisionInfo* info)
{
return computeSphereSphereCollision(s1->position, s1->radius, s2->position, s2->radius, info->point, info->normal, info->depth);
}

bool collision3d::sphereBox(Sphere3d* s, Box3d* b, CollisionInfo* info)
{
//Recuperar o ponto mais próximo da caixa em relação ao centro da esfera...
vector3d min=b->position-b->alf;
vector3d max=b->position+b->alf;
vector3d closestPoint=get_closest_point_from_aabb(s->position, min, max);
return computeSphereBoxCollision(s->getLastPosition(), closestPoint, s->position, s->radius, min, max, info->point, info->normal, info->depth);
}

bool collision3d::sphereCapsule(Sphere3d* s, Capsule3d* c, CollisionInfo* info)
{
vector3d closestPoint;
float t=0.0f;
vector3d min, max;
c->getLine(min, max);
ClosestPtPointSegment(s->position, min, max, t, closestPoint);
return computeSphereSphereCollision(s->position, s->radius, closestPoint, c->radius, info->point, info->normal, info->depth);
}

//Box...

bool collision3d::boxBox(Box3d* b1, Box3d* b2, CollisionInfo* info)
{
return false;
}

bool collision3d::boxSphere(Box3d* b, Sphere3d* s, CollisionInfo* info){
        vector3d min = b->position - b->alf;
        vector3d max = b->position + b->alf;
vector3d closestPoint=get_closest_point_from_aabb(s->position, min, max);
return computeBoxSphereCollision(b->getLastPosition(), closestPoint, min, max, s->position, s->radius, info->point, info->normal, info->depth);
}

bool collision3d::boxCapsule(Box3d* b, Capsule3d* c, CollisionInfo* info){
vector3d pt;
float t;
vector3d cmin, cmax;
vector3d bmin, bmax;
c->getLine(cmin, cmax);
b->getMinMax(bmin, bmax);
ClosestPtPointSegment(b->position, cmin, cmax, t, pt);
vector3d closestPoint=get_closest_point_from_aabb(pt, bmin, bmax);
return computeBoxSphereCollision(b->getLastPosition(), closestPoint, bmin, bmax, pt, c->radius, info->point, info->normal, info->depth);
}

//Capsule

bool collision3d::capsuleCapsule(Capsule3d* c1, Capsule3d* c2, CollisionInfo* info)
{
float s, t=0.0f;
vector3d v1, v2;
vector3d m1, m2, m3, m4;
c1->getLine(m1, m2);
c2->getLine(m3, m4);
float sqdist=ClosestPtSegmentSegment(m1, m2, m3, m4, s, t, v1, v2);
return computeSphereSphereCollision(v1, c1->radius, v2, c2->radius, info->point, info->normal, info->depth);
}

bool collision3d::capsuleSphere(Capsule3d* c, Sphere3d* s, CollisionInfo* info)
{
vector3d closestPoint;
float t=0.0f;
vector3d min, max;
c->getLine(min, max);
ClosestPtPointSegment(s->position, min, max, t, closestPoint);
return computeSphereSphereCollision(closestPoint, c->radius, s->position, s->radius, info->point, info->normal, info->depth);
}

bool collision3d::capsuleBox(Capsule3d* c, Box3d* b, CollisionInfo* info)
{
vector3d pt;
float t;
vector3d cmin, cmax, bmin, bmax;
c->getLine(cmin, cmax);
b->getMinMax(bmin, bmax);
ClosestPtPointSegment(b->position, cmin, cmax, t, pt);
vector3d closestPoint=get_closest_point_from_aabb(pt, bmin, bmax);
return computeSphereBoxCollision(c->getLastPosition(), closestPoint, pt, c->radius, bmin, bmax, info->point, info->normal, info->depth);
}
}
