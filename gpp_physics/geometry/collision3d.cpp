
#include <limits>
#include<gpp/debug_system.h>
#include"geometry.h"
#include"collision3d.h"

using namespace std;

namespace gpp
{
bool collision3d::isColliding(GeometricShape* s1, GeometricShape* s2, CollisionData* data)
{
if((s1==NULL)||(s2==NULL))
{
return false;
}
switch((s1->GetGeometricType()|s2->GetGeometricType()))
{
case GTYPE_SPHERE:
return sphereSphere((sphere3d*)s1, (sphere3d*)s2, data);
case GTYPE_SPHERE|GTYPE_BOX:
{
return sphereBox(((s1->geometricType==GTYPE_SPHERE) ? (sphere3d*)s1 : (sphere3d*)s2), ((s1->geometricType==GTYPE_BOX) ? (box3d*)s1 :
(box3d*)s2), data);
}
case GTYPE_SPHERE|GTYPE_LINE:
return sphereLine(((s1->geometricType==GTYPE_SPHERE) ? (sphere3d*)s1 : (sphere3d*)s2), ((s1->geometricType==GTYPE_LINE) ? (line3d*)s1 :
(line3d*)s2), data);
case GTYPE_BOX:
return boxBox((box3d*)s1, (box3d*)s2, data);
case GTYPE_BOX|GTYPE_LINE:
return boxLine( ((s1->geometricType==GTYPE_BOX) ? (box3d*)s1 : (box3d*)s2), ((s1->geometricType==GTYPE_LINE) ? (line3d*)s1 : (line3d*)s2), data);
case GTYPE_LINE:
return lineLine((line3d*)s1, (line3d*)s2, data);
}
return false;
}

bool collision3d::sphereSphere(sphere3d* s1, sphere3d* s2, CollisionData* data)
{
    vector3d displacement = s1->center - s2->center;
    float distanceSquared = vector3d::dotProduct(displacement, displacement);
    float radiusSum = s1->radius + s2->radius;
    float radiusSumSquared = radiusSum * radiusSum;

    if (distanceSquared > radiusSumSquared)         return false;

    float distance = sqrt(distanceSquared);
    vector3d collisionNormal = displacement / distance;
if (vector3d::dotProduct(s1->center - s2->center, collisionNormal ) > 0) {
collisionNormal.inverse();
}
    vector3d collisionPoint = s1->center + s1->radius * collisionNormal;
    data->normal = collisionNormal;
    data->point = collisionPoint;
    data->depth= radiusSum - distance;
    return true;
}

bool collision3d::sphereLine(sphere3d* s, line3d* l, CollisionData* data)
{
  vector3d centerToStart = l->min - s->center;
  vector3d centerToEnd = l->max - s->center;
  vector3d startToEnd = l->max - l->min;
  float a =vector3d::dotProduct(startToEnd, startToEnd);
  float b = 2 * vector3d::dotProduct(centerToStart, startToEnd);
  float c = vector3d::dotProduct(centerToStart, centerToStart) - (s->radius * s->radius);
  float disc = b * b - 4 * a * c;
  if (disc < 0)
  {
    return false;
  }
  else
  {
    disc = sqrt(disc);
    float t1 = (-b - disc) / (2 * a);
    float t2 = (-b + disc) / (2 * a);
    vector3d collision1 = l->min + t1 * startToEnd;
    vector3d collision2 = l->min + t2 * startToEnd;
    if (t1 >= 0 && t1 <= 1)
    {
      data->point = collision1;
      data->normal = vector3d::normalize((collision1 - s->center));
      data->depth = s->radius - (collision1 - s->center).length();
      return true;
    }
    else if (t2 >= 0 && t2 <= 1)
    {
      data->point = collision2;
      data->normal = vector3d::normalize((collision2 - s->center));
      data->depth = s->radius - (collision2 - s->center).length();
      return true;
    }
    else
    {
      return false;
    }
  }
}

bool collision3d::sphereBox(sphere3d* s, box3d* b, CollisionData* data)
{
vector3d closestPoint;
closestPoint.x = std::max(b->min.x, std::min(s->center.x, b->min.x + b->measures.x));
closestPoint.y = std::max(b->min.y, std::min(s->center.y, b->min.y + b->measures.y));
closestPoint.z = std::max(b->min.z, std::min(s->center.z, b->min.z + b->measures.z));
float sqdist=vector3d::dotProduct(closestPoint-s->center, closestPoint-s->center);
float sqradius=s->radius*s->radius;
if(sqdist<=sqradius)
{
data->point=closestPoint;
data->normal=(closestPoint-s->center).normalize();
data->depth=sqrt(sqradius-sqdist);
return true;
}
return false;
}

bool collision3d::boxBox(box3d* b1, box3d* b2, CollisionData* data)
{
vector3d min1 = b1->min, max1 = b1->min + b1->measures;
vector3d min2 = b2->min, max2 = b2->min + b2->measures;
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
data->point = (closestPoint1 + closestPoint2) / 2;
// Calcular a intensidade da colisão
data->depth = data->normal.length();
return true;
}

bool collision3d::boxLine(box3d* b, line3d* l, CollisionData* data)
{
  // Cálculo da origem da linha e do vetor direção
  vector3d o = l->min;
  vector3d d = l->max - l->min;
  
  // Variáveis para guardar os valores de t
  float tMin = 0.0f;
  float tMax = 1.0f;

  // Verificação de colisão com cada face da caixa
  for (int i = 0; i < 3; i++) {
    if (abs(d[i]) < numeric_limits<float>::epsilon()) {
      // Linha paralela a uma face da caixa
      if (o[i] < b->min[i] || o[i] > b->min[i] + b->measures[i])
        return false;
    } else {
      // Calcula o valor de t para interceção com cada face da caixa
      float t1 = (b->min[i] - o[i]) / d[i];
      float t2 = (b->min[i] + b->measures[i] - o[i]) / d[i];

      // Verifica se t1 é maior do que t2 e troca os valores
      if (t1 > t2)
        swap(t1, t2);

      // Atualiza os valores de tMin e tMax
      tMin = max(tMin, t1);
      tMax = min(tMax, t2);

      // Se tMin é maior do que tMax, a linha não colide com a caixa
      if (tMin > tMax)
        return false;
    }
  }

  // Calcula o ponto de colisão
  data->point = o + d * tMin;

  // Calcula o vetor normal da colisão
  vector3d normal;
  for (int i = 0; i < 3; i++) {
    if (abs(data->point[i] - b->min[i]) < numeric_limits<float>::epsilon())
      normal[i] = -1;
    else if (abs(data->point[i] - b->min[i] - b->measures[i]) < numeric_limits<float>::epsilon())
      normal[i] = 1;
    else
      normal[i] = 0;
  }
  data->normal = normal;

  // Calcula a profundidade de penetração
  data->depth = (tMax - tMin) * d.length();

  // A linha colide com a caixa
  return true;
}

bool collision3d::lineLine(line3d* l1, line3d* l2, CollisionData* data)
{
/*
static float f1=0.0f, f2=0.0f;
static vector3d v1, v2;
return ClosestPtSegmentSegment(l1->min, l1->max, l2->min, l2->max, f1, f2, v1, v2)<=0.01f;
*/
    vector3d u = l1->max - l1->min;
    vector3d v = l2->max - l2->min;
    vector3d w = l1->min - l2->min;
    float a = vector3d::dotProduct(u, u);
    float b = vector3d::dotProduct(u, v);
    float c = vector3d::dotProduct(v, v);
    float d = vector3d::dotProduct(u, w);
    float e = vector3d::dotProduct(v, w);
    float D = a * c - b * b;
    float sc, sN;
    if (D < FLT_EPSILON)
    {
        sN = 0.0f;
        sc = e / c;
        sc = min(max(sc, 0.0f), 1.0f);
    }
    else
    {
        sN = (b * e - c * d) / D;
        sc = (a * e - b * d) / D;
        if (sN < 0.0f)
        {
            sN = 0.0f;
            sc = e / c;
            sc = std::min(std::max(sc, 0.0f), 1.0f);
        }
        else if (sN > 1.0f)
        {
            sN = 1.0f;
            sc = (b + e) / c;
            sc = std::min(std::max(sc, 0.0f), 1.0f);
        }
    }
    vector3d P1 = l1->min + u * sc;
    vector3d P2 = l2->min + v * sN;
    vector3d normal = P1 - P2;
    float intensity = normal.length();
    if (intensity < FLT_EPSILON)
    {
        data->point = P1;
        data->normal = vector3d::normalize(normal);
        data->depth = intensity;
        return true;
    }
    return false;
}
}
