

#include"math.h"

using namespace std;

namespace gpp{

bool raySphere(const vector3d& origin, const vector3d& dir, const vector3d& center, float radius, vector3d& in, vector3d& out)
{
    vector3d m = origin - center;
    float b = vector3d::dotProduct(m, dir);
    float c = vector3d::dotProduct(m, m) - radius * radius;

    // Inicialize discriminante e variáveis de ponto de entrada e saída
    float discr = 0.0f;
    float tEntry = 0.0f;
    float tExit = 0.0f;

    // Exit if r's origin is outside s (c > 0) and r is pointing away from s (b > 0)
    if (c > 0.0f && b > 0.0f) return false;

    discr = b * b - c;

    // A negative discriminant corresponds to the ray missing the sphere
    if (discr < 0.0f) return false;

    // Calculate both entry and exit points
    tEntry = -b - sqrt(discr);
    tExit = -b + sqrt(discr);

    // If tEntry is negative, ray started inside sphere, so clamp it to zero
    if (tEntry < 0.0f) tEntry = 0.0f;

    // Calculate entry and exit points
    in = origin +( dir*tEntry );
    out = origin + (dir*tExit );

    return true;
}

/*
bool rayBox(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, vector3d& in, vector3d& out)
{
float    tmin = 0.0f;  // set to -FLT_MAX to get first hit on line
   float tmax = FLT_MAX; // set to max distance ray can travel (for segment)
      // For all three slabs
      for (int i = 0; i < 3; i++) {
            if (fabs(dir[i]) < GPP_EPSILON) {
                  // Ray is parallel to slab. No hit if origin not within slab
                  if (origin[i] < min[i] || origin[i] > max[i]) return false;
            } else {
                  // Compute intersection t value of ray with near and far plane of slab
                  float ood = 1.0f / dir[i];
                  float t1 = (min[i] - origin[i]) * ood;
                  float t2 = (max[i] - origin[i]) * ood;
                  // Make t1 be intersection with near plane, t2 with far plane
                  if (t1 > t2) swap(t1, t2);

            // Para o caso quando a origem está dentro da caixa, ajuste tmax.
            if (origin[i] < min[i]) {
                tmax = (max[i] - origin[i]) * ood;
            } else if (origin[i] > max[i]) {
                tmax = (min[i] - origin[i]) * ood;
            }

                  // Compute the intersection of slab intersection intervals
                  if (t1 > tmin) tmin = t1;
                  if (t2 > tmax) tmax = t2;
                  // Exit with no collision as soon as slab intersection becomes empty
                  if (tmin > tmax) return false;
            }
      }
      // Ray intersects all 3 slabs. Return point (q) and intersection t value (tmin)
in= origin + dir * tmin;
out= origin + dir * tmax;
      return true;
}
*/

bool rayBox(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, vector3d& in, vector3d& out)
{
    float tmin = 0.0f;
    float tmax = FLT_MAX;

    for (int i = 0; i < 3; i++) {
        if (fabs(dir[i]) < GPP_EPSILON) {
            if (origin[i] < min[i] || origin[i] > max[i]) return false;
        } else {
            float ood = 1.0f / dir[i];
            float t1 = (min[i] - origin[i]) * ood;
            float t2 = (max[i] - origin[i]) * ood;
            if (t1 > t2) swap(t1, t2);

            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;

            if (tmin > tmax) return false;
        }
    }

    in = origin + dir * tmin;
    out = origin + dir * tmax;
    return true;
}

bool rayCylinder(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, float radius, vector3d& in, vector3d& out)
{
    bool hit;
vector3d axis=(max-min).normalize();
    // Calcular os vetores normais das tampas
    vector3d bottomNormal = axis;
    vector3d topNormal = -axis;

    // Calcular os pontos centrais das tampas
    vector3d bottomCenter = min;
    vector3d topCenter = max;

    // Verificar a interseção com a tampa inferior
    float tBottom = vector3d::dotProduct((bottomCenter - origin),bottomNormal) / vector3d::dotProduct(dir, bottomNormal);

    // Verificar a interseção com a tampa superior
    float tTop = vector3d::dotProduct((topCenter - origin), topNormal) /
vector3d::dotProduct(dir, topNormal);

    // Verificar se o raio intersecta alguma tampa
    if (tBottom > 0 && tTop > 0) {
        // Calcular os pontos de interseção
        in = origin + dir * tBottom;
        out = origin + dir * tTop;
if(tBottom>tTop){
swap(in, out);
}
        return true;
    }

//Testar com o corpo do cilindro...
    vector3d RC = origin - min;
    vector3d n =vector3d::crossProduct( dir, axis);
    float ln = n.length();

    if (ln == 0.0) {  // Raio paralelo ao cilindro
        float d = vector3d::dotProduct(RC, axis);
        vector3d D = RC - d * axis;
        d = D.length();
        return (d <= radius);  // Verdadeiro se o raio estiver dentro do cilindro
    }

    n.normalize();
    float d = std::fabs(vector3d::dotProduct(RC, n));
    hit = (d <= radius);

    if (hit) {
        vector3d O =vector3d::crossProduct( RC, axis);
        float t = -vector3d::dotProduct(O, n) / ln;
        vector3d O_cross = vector3d::crossProduct(n, axis);
        O_cross.normalize();
        float s = std::fabs(std::sqrt(radius * radius - d * d) / vector3d::dotProduct(dir, O_cross));
float din = static_cast<float>(t - s);  // Distância de entrada
float dout = static_cast<float>(t + s);  // Distância de saída
in=origin+(dir*din);
out=origin+(dir*dout);
    }
    return hit;
}

bool rayCapsule(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, float radius, vector3d& in, vector3d& out)
{
    bool hit;
vector3d axis=(max-min).normalize();
    // Calcular os vetores normais das hemisférios nas extremidades
    vector3d minNormal = (origin - min).normalize();
    vector3d maxNormal = (origin - max).normalize();
    // Calcular os pontos centrais das hemisférios
    vector3d minCenter = min;
    vector3d maxCenter = max;
    // Calcular os raios das hemisférios
    float minRadius = radius;
    float maxRadius = radius;
    // Calcular a interseção com a hemisfério de início
    float tStart = vector3d::dotProduct((minCenter - origin), minNormal);
    // Calcular a interseção com a hemisfério de fim
    float tEnd = vector3d::dotProduct((maxCenter - origin), maxNormal);
    // Verificar se o raio intersecta alguma das hemisférios
    if (tStart >= 0 && tStart * tStart <= minRadius * minRadius) {
        // Raio intersecta a hemisfério de início, calcule o ponto de entrada
        float d = std::sqrt(minRadius * minRadius - tStart * tStart);
        in = origin + dir * (tStart - d);
    } else if (tEnd >= 0 && tEnd * tEnd <= maxRadius * maxRadius) {
        // Raio intersecta a hemisfério de fim, calcule o ponto de entrada
        float d = std::sqrt(maxRadius * maxRadius - tEnd * tEnd);
        in = origin + dir * (tEnd - d);
    }

//Testar com o corpo do cilindro...
    vector3d RC = origin - min;
    vector3d n =vector3d::crossProduct( dir, axis);
    float ln = n.length();

    if (ln == 0.0) {  // Raio paralelo ao cilindro
        float d = vector3d::dotProduct(RC, axis);
        vector3d D = RC - d * axis;
        d = D.length();
        return (d <= radius);  // Verdadeiro se o raio estiver dentro do cilindro
    }

    n.normalize();
    float d = std::fabs(vector3d::dotProduct(RC, n));
    hit = (d <= radius);

    if (hit) {
        vector3d O =vector3d::crossProduct( RC, axis);
        float t = -vector3d::dotProduct(O, n) / ln;
        vector3d O_cross = vector3d::crossProduct(n, axis);
        O_cross.normalize();
        float s = std::fabs(std::sqrt(radius * radius - d * d) / vector3d::dotProduct(dir, O_cross));
float din = static_cast<float>(t - s);  // Distância de entrada
float dout = static_cast<float>(t + s);  // Distância de saída
in=origin+(dir*din);
out=origin+(dir*dout);
    }
    return hit;
}
}
