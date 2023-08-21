
#include <limits>
#include<gpp/debug_system.h>
#include"geometry.h"
#include"collision3d.h"

using namespace std;

namespace gpp
{
bool collision3d::isColliding(GeometricShape* s1, GeometricShape* s2, CollisionInfo* data)
{
if((s1==NULL)||(s2==NULL))
{
return false;
}
switch((s1->getGeometricType()|s2->getGeometricType()))
{
case GTYPE_SPHERE:
return sphereSphere((sphere3d*)s1, (sphere3d*)s2, data);
case GTYPE_SPHERE|GTYPE_BOX:
{
return sphereBox(((s1->geometricType==GTYPE_SPHERE) ? (sphere3d*)s1 : (sphere3d*)s2), ((s1->geometricType==GTYPE_BOX) ? (box3d*)s1 :
(box3d*)s2), data);
}
case GTYPE_BOX:
return boxBox((box3d*)s1, (box3d*)s2, data);
}
return false;
}

bool collision3d::sphereSphere(sphere3d* s1, sphere3d* s2, CollisionInfo* data)
{
    vector3d displacement = s1->center - s2->center;
    float distanceSquared = vector3d::dotProduct(displacement, displacement);
    float radiusSum = s1->radius + s2->radius;
    float radiusSumSquared = radiusSum * radiusSum;

    if (distanceSquared > radiusSumSquared)         return false;

    float distance = sqrt(distanceSquared);
    vector3d collisionNormal = displacement / distance;
    vector3d collisionPoint = s2->center + s2->radius * collisionNormal;
    data->normal = collisionNormal;
    data->point = collisionPoint;
    data->depth= radiusSum - distance;
    return true;
}

bool collision3d::sphereBox(sphere3d* s, box3d* b, CollisionInfo* data)
{
/*
vector3d closestPoint;
closestPoint.x = std::max(b->min.x, std::min(s->center.x, b->min.x + b->measures.x));
closestPoint.y = std::max(b->min.y, std::min(s->center.y, b->min.y + b->measures.y));
closestPoint.z = std::max(b->min.z, std::min(s->center.z, b->min.z + b->measures.z));
float sqdist=vector3d::dotProduct(closestPoint-s->center, closestPoint-s->center);
float sqradius=s->radius*s->radius;
if(sqdist<=sqradius)
{
data->point=closestPoint;
data->normal=(s->center-closestPoint).normalize();
data->depth=sqrt(sqradius-sqdist);
return true;
}
*/
return false;
}

bool collision3d::boxBox(box3d* b1, box3d* b2, CollisionInfo* data)
{
vector3d min1 = b1->min, max1 = b1->max;
vector3d min2 = b2->min, max2 = b2->max;
// Verifica se há interseção
if (min1.x > max2.x || min2.x > max1.x) return false;
if (min1.y > max2.y || min2.y > max1.y) return false;
if (min1.z > max2.z || min2.z > max1.z) return false;
// Calcular o ponto de colisão mais próximo
vector3d closestPoint1 = vector3d(
std::max(min1.x, min2.x),
std::max(min1.y, min2.y),
std::max(min1.z, min2.z)
);
vector3d closestPoint2 = vector3d(
std::min(max1.x, max2.x),
std::min(max1.y, max2.y),
std::min(max1.z, max2.z)
);
// Calcular a normal de colisão
data->normal = closestPoint2 - closestPoint1;
data->normal = vector3d::normalize(data->normal);
// Calcular o ponto de colisão
data->point = (closestPoint1 + closestPoint2) *0.5f;
// Calcular a intensidade da colisão
data->depth = data->normal.length();
return true;
}
}
