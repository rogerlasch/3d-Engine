
#include <limits>
#include"../common.h"
#include"../math/math.h"
#include"../geometry/geometry.h"
#include"CollisionInfo.h"
#include"collision3d.h"

using namespace std;

namespace gpp
{
bool collision3d::isColliding(GeometricShape* s1, GeometricShape* s2, CollisionInfo* info)
{
_GASSERT(s1!=NULL);
_GASSERT(s2!=NULL);
switch((s1->getGeometricType()|s2->getGeometricType()))
{
case GTYPE_SPHERE:
return sphereSphere((sphere3d*)s1, (sphere3d*)s2, info);
case GTYPE_SPHERE|GTYPE_BOX:
{
return sphereBox(((s1->geometricType==GTYPE_SPHERE) ? (sphere3d*)s1 : (sphere3d*)s2), ((s1->geometricType==GTYPE_BOX) ? (box3d*)s1 : (box3d*)s2), info);
}
case GTYPE_BOX:
return boxBox((box3d*)s1, (box3d*)s2, info);
}
return false;
}

bool collision3d::sphereSphere(sphere3d* s1, sphere3d* s2, CollisionInfo* info)
{
vector3d dt=s2->center-s1->center;
float sqDist=(dt*dt);
float rdSun=s1->radius+s2->radius;
if(sqDist>(rdSun*rdSun)) return false;
info->normal=vector3d::normalize(dt);
info->depth=fabs(sqrt(sqDist)-rdSun);
vector3d maxPenetration=s2->center+(vector3d::inverse(info->normal)*s2->radius);
maxPenetration+=info->normal*info->depth;
info->point=maxPenetration;
    return true;
}

bool collision3d::sphereBox(sphere3d* s, box3d* b, CollisionInfo* info)
{
//Recuperar o ponto mais próximo da caixa em relação ao centro da esfera...
vector3d closestPoint=get_closest_point_from_aabb(s->center, b->min, b->max);
//Calcular a distância ao quadrado do ponto e o centro da esfera
//Depois, verificar se a distância é menor ou igual ao raio ao quadrado da esfera...
vector3d vdir=s->center-closestPoint;
float sqDist=vdir*vdir;
float sqRadius=s->radius*s->radius;
//Se a distância for maior, não à colisão...
if(sqDist>sqRadius) return false;
//Caso o centro da esfera esteja dentro da caixa...
//O vetor normal da colisão será nulo, e precisamos tratar isso...
//Aqui, calcularemos a direção da tragetória e depois lançaremos um raio na direção oposta para encontrar o ponto de saída da caixa.
//Por fim, definiremos o vetor normal apontando na direção em que o raio foi disparado.
vector3d normal;
float depth=0.0f;
vector3d contactPoint;
//Verifica se o centro da esfera está dentro dos limites da caixa...
if(point_in_aabb(s->center, b->min, b->max))
{
//Primeiro, calcular o vetor de direção desde a última posição conhecida...
vector3d lastPosition=s->getLastPosition();
vector3d from=s->center-lastPosition;
from.normalize();
vector3d fromInverse=vector3d::inverse(from);
vector3d pt;
//Vamos lançar um raio na direção oposta da tragetória para determinarmos o ponto de saída.
rayBox(s->center, fromInverse, b, pt, false);
normal=fromInverse;
contactPoint=pt;
//Para a profundidade de penetração primeiro devemos calcular o ponto máximo de penetração...
//E depois calcular a distância entre ele e o ponto de contato...
vector3d maxPenetration=s->center+(s->radius*from);
depth=fabs((from*(maxPenetration-contactPoint)));
}
else//O centro da esfera está fora da caixa, por tanto, apenas calcule as informações...
{
contactPoint=closestPoint;
normal=(s->center-closestPoint).normalize();
vector3d maxPenetration=(s->center+(vector3d::inverse(normal)*s->radius));
depth=normal*(maxPenetration-contactPoint);
}
info->normal=normal;
info->point=contactPoint;
info->depth=fabs(depth);
return true;
}

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

//Raycasting

bool collision3d::rayPoint(const vector3d& origin, const vector3d& dir, const vector3d& point) {
    // Calcula o vetor que aponta do jogador até o ponto
    vector3d targetDirection = point - origin;

    // Normaliza o vetor de direção do jogador
    vector3d normalizedDir = vector3d::normalize(dir);

    // Normaliza o vetor que aponta do jogador até o ponto
    vector3d normalizedTarget = vector3d::normalize(targetDirection);

    // Calcula o produto escalar entre os vetores normalizados
    float dotProductValue = vector3d::dotProduct(normalizedDir, normalizedTarget);

    // Verifica se o produto escalar é maior ou igual a 0
    // Isso indica que o vetor dir aponta na mesma direção que o vetor jogador -> ponto
    return dotProductValue > 0.0f;
}

//Código retirado do realtime collision detection...
bool collision3d::raySphere(const vector3d& origin, const vector3d& dir, sphere3d* s, vector3d& q)
{
          vector3d m = origin - s->center;
          float b = vector3d::dotProduct(m, dir);
          float c = vector3d::dotProduct(m, m) - s->radius * s->radius;
          // Exit if r's origin outside s (c > 0) and r pointing away from s (b > 0)
          if (c > 0.0f && b > 0.0f) return false;
          float discr = b*b - c;
          // A negative discriminant corresponds to ray missing sphere
          if (discr < 0.0f) return false;
          // Ray now found to intersect sphere, compute smallest t value of intersection
float           t = -b - sqrt(discr);
          // If t is negative, ray started inside sphere so clamp t to zero
          if (t < 0.0f) t = 0.0f;
          q = origin + t * dir;
          return true;
}

bool collision3d::rayBox(const vector3d& origin, const vector3d& dir, box3d* b, vector3d& q, bool calculateEntry) {
    float tmin = 0.0f;
    float tmax = FLT_MAX;

    for (int i = 0; i < 3; i++) {
        if (fabs(dir[i]) < numeric_limits<float>::epsilon()) {
            if (origin[i] < b->min[i] || origin[i] > b->max[i]) return false;
        } else {
            float ood = 1.0f / dir[i];
            float t1 = (b->min[i] - origin[i]) * ood;
            float t2 = (b->max[i] - origin[i]) * ood;
            if (t1 > t2) swap(t1, t2);

            if (calculateEntry) {
                if (t1 > tmin) tmin = t1;
            } else {
                if (t2 < tmax) tmax = t2;
            }

            if (tmin > tmax) return false;
        }
    }

    q = origin + dir * (calculateEntry ? tmin : tmax);
    return true;
}
}
