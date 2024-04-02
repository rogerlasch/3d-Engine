

#include"math.h"

using namespace std;

namespace gpp{

bool raySphereAUX(const vector3d& origin, const vector3d& dir, const vector3d& center, float radius, float& tMin, float& tMax){
        vector3d CO = origin - center;

        float a = vector3d::dot(dir, dir);
        float b = 2.0f * vector3d::dot(CO, dir);
        float c = vector3d::dot(CO, CO) - (radius * radius);

        float discriminant = b * b - 4.0f * a * c;
        if(discriminant < 0.0f)
                return false;

        tMin = (-b - sqrtf(discriminant)) / (2.0f * a);
        tMax = (-b + sqrtf(discriminant)) / (2.0f * a);
        if(tMin > tMax)
        {
swap(tMin, tMax);
        }

        return true;
}

bool raySphere(const vector3d& origin, const vector3d& dir, const vector3d& center, decimal radius, RayInfo* info) {
decimal tMin, tMax;
if(!raySphereAUX(origin, dir, center, radius, tMin, tMax)){
return false;
}

info->eDist=tMin;
info->oDist=tMax;
info->ePoint=origin+(dir*tMin);
info->oPoint=origin+(dir*tMax);
    return true;
}

bool rayBox(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, RayInfo* info)
{
    decimal tmin = 0.0f;
    decimal tmax = FLT_MAX;

    for (int i = 0; i < 3; i++) {
        if (fabs(dir[i]) < GPP_EPSILON) {
            if (origin[i] < min[i] || origin[i] > max[i]) return false;
        } else {
            decimal ood = 1.0f / dir[i];
            decimal t1 = (min[i] - origin[i]) * ood;
            decimal t2 = (max[i] - origin[i]) * ood;
            if (t1 > t2) swap(t1, t2);

            if (t1 > tmin) tmin = t1;
            if (t2 < tmax) tmax = t2;

            if (tmin > tmax) return false;
        }
    }

if(info!=NULL){
info->eDist=tmin;
info->ePoint= origin + dir * tmin;
info->oDist=tmax;
info->oPoint = origin + dir * tmax;
}
    return true;
}

bool rayCylinder(const vector3d& origin, const vector3d& dir, const vector3d& min, const vector3d& max, decimal radius, RayInfo* info)
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
    decimal tBottom = vector3d::dot((bottomCenter - origin),bottomNormal) / vector3d::dot(dir, bottomNormal);

    // Verificar a interseção com a tampa superior
    decimal tTop = vector3d::dot((topCenter - origin), topNormal) /
vector3d::dot(dir, topNormal);

    // Verificar se o raio intersecta alguma tampa
    if (tBottom > 0 && tTop > 0) {
        // Calcular os pontos de interseção
if(info!=NULL){
info->eDist=tBottom;
info->ePoint= origin + dir * tBottom;
info->oDist=tTop;
info->oPoint = origin + dir * tTop;
if(tBottom>tTop){
swap(info->eDist, info->oDist);
swap(info->ePoint, info->oPoint);
}
}

        return true;
    }

//Testar com o corpo do cilindro...
    vector3d RC = origin - min;
    vector3d n =vector3d::cross( dir, axis);
    decimal ln = n.length();

    if (ln == 0.0) {  // Raio paralelo ao cilindro
        decimal d = vector3d::dot(RC, axis);
        vector3d D = RC - d * axis;
        d = D.length();
        return (d <= radius);  // Verdadeiro se o raio estiver dentro do cilindro
    }

    n.normalize();
    decimal d = std::fabs(vector3d::dot(RC, n));
    hit = (d <= radius);

    if (hit) {
        vector3d O =vector3d::cross( RC, axis);
        decimal t = -vector3d::dot(O, n) / ln;
        vector3d O_cross = vector3d::cross(n, axis);
        O_cross.normalize();
        decimal s = std::fabs(std::sqrt(radius * radius - d * d) / vector3d::dot(dir, O_cross));
decimal din = static_cast<decimal>(t - s);  // Distância de entrada
decimal dout = static_cast<decimal>(t + s);  // Distância de saída

if(info!=NULL){
info->eDist=din;
info->oDist=dout;
info->ePoint=origin+(dir*din);
info->oPoint=origin+(dir*dout);
}
    }
    return hit;
}

bool rayCapsule(const vector3d& origin, const vector3d& dir, const vector3d& cPos, const vector3d& axis, float length, float radius, RayInfo* info){
//https://gist.github.com/jdryg/ecde24d34aa0ce2d4d87
        // http://pastebin.com/2XrrNcxb
        // Substituting equ. (1) - (6) to equ. (I) and solving for t' gives:
        //
        // t' = (t * dot(AB, d) + dot(AB, AO)) / dot(AB, AB); (7) or
        // t' = t * m + n where
        // m = dot(AB, d) / dot(AB, AB) and
        // n = dot(AB, AO) / dot(AB, AB)
        //
float alf=length*0.5f;
float eDist, oDist;
vector3d p1, p2;
vector3d cMA=cPos-(axis*alf);
vector3d cMB=cPos+(axis*alf);
        vector3d AB = cMB - cMA;
        vector3d AO = origin - cMA;
        float AB_dot_d = vector3d::dot(AB,dir);
        float AB_dot_AO = vector3d::dot(AB,AO);
        float AB_dot_AB = vector3d::dot(AB,AB);
        float m = AB_dot_d / AB_dot_AB;
        float n = AB_dot_AO / AB_dot_AB;
        // Substituting (7) into (II) and solving for t gives:
        //
        // dot(Q, Q)*t^2 + 2*dot(Q, R)*t + (dot(R, R) - r^2) = 0
        // where
        // Q = d - AB * m
        // R = AO - AB * n
        vector3d Q = dir - (AB * m);
        vector3d R = AO - (AB * n);
        float a = vector3d::dot(Q,Q);
        float b = 2.0f * vector3d::dot(Q,R);
        float c = vector3d::dot(R,R) - (radius * radius);
        if(a == 0.0f)
        {
                // Special case: AB and ray direction are parallel. If there is an intersection it will be on the end spheres...
                // NOTE: Why is that?
                // Q = d - AB * m =>
                // Q = d - AB * (|AB|*|d|*cos(AB,d) / |AB|^2) => |d| == 1.0
                // Q = d - AB * (|AB|*cos(AB,d)/|AB|^2) =>
                // Q = d - AB * cos(AB, d) / |AB| =>
                // Q = d - unit(AB) * cos(AB, d)
                //
                // |Q| == 0 means Q = (0, 0, 0) or d = unit(AB) * cos(AB,d)
                // both d and unit(AB) are unit vectors, so cos(AB, d) = 1 => AB and d are parallel.
                float atmin, atmax, btmin, btmax;
                if(        !raySphereAUX(origin, dir, cMA, radius, atmin, atmax) ||
                        !raySphereAUX(origin, dir, cMB, radius, btmin, btmax))
                {
                        // No intersection with one of the spheres means no intersection at all...
                        return false;
                }
                if(atmin < btmin)
                {
eDist=atmin;
                        p1 = origin + (dir * atmin);
                }
                else
                {
eDist=btmin;
                        p1 = origin + (dir * btmin);
                }
                if(atmax > btmax)
                {
oDist=atmax;
                        p2 = origin + (dir * atmax);
                }
                else
                {
oDist=btmax;
                        p2 = origin + (dir * btmax);
                }

if(info!=NULL){
info->eDist=eDist;
info->oDist=oDist;
info->ePoint=p1;
info->oPoint=p2;
}
                return true;
        }

        float discriminant = b * b - 4.0f * a * c;
        if(discriminant < 0.0f)
        {
                // The ray doesn't hit the infinite cylinder defined by (A, B).
                // No intersection.
                return false;
        }
        float tmin = (-b - sqrtf(discriminant)) / (2.0f * a);
        float tmax = (-b + sqrtf(discriminant)) / (2.0f * a);
        if(tmin > tmax)
        {
swap(tmin, tmax);
        }
        // Now check to see if K1 and K2 are inside the line segment defined by A,B
        float t_k1 = tmin * m + n;
        if(t_k1 < 0.0f)
        {
                // On sphere (A, r)...
                float stmin, stmax;
                if(raySphereAUX(origin, dir, cMA, radius, stmin, stmax))
                {
eDist=stmin;
                        p1 = origin + (dir * stmin);
                }
                else
                        return false;
        }
        else if(t_k1 > 1.0f)
        {
                // On sphere (B, r)...
                float stmin, stmax;
                if(raySphereAUX(origin, dir, cMB, radius, stmin, stmax))
                {
eDist=stmin;
                        p1 = origin + (dir * stmin);
                }
                else
                        return false;
        }
        else
        {
                // On the cylinder...
eDist=tmin;
                p1 = origin + (dir * tmin);
//                vector3d k1 = cMA + AB * t_k1;
        }
        float t_k2 = tmax * m + n;
        if(t_k2 < 0.0f)
        {
                // On sphere (A, r)...
                float stmin, stmax;
                if(raySphereAUX(origin, dir, cMA, radius, stmin, stmax))
                {
oDist=stmax;
                        p2 = origin + (dir * stmax);
                }
                else
                        return false;
        }
        else if(t_k2 > 1.0f)
        {
                // On sphere (B, r)...
                float stmin, stmax;
                if(raySphereAUX(origin, dir, cMB, radius, stmin, stmax))
                {
oDist=stmax;
                        p2 = origin + (dir * stmax);
                }
                else
                        return false;
        }
        else
        {
oDist=tmax;
                p2 = origin + (dir * tmax);
//                vector3d k2 = cMA + AB * t_k2;
        }
if(info!=NULL){
info->eDist=eDist;
info->oDist=oDist;
info->ePoint=p1;
info->oPoint=p2;
}
        return true;
}

    bool rayOBB(const vector3d& origin, const vector3d& dir, const vector3d& center, const vector3d& alf, const std::array<vector3d, 3>& axis, RayInfo* info){

        vector3d delta = center - origin;
        vector3d extents = alf;

        decimal tIn = -std::numeric_limits<decimal>::infinity();
        decimal tOut = std::numeric_limits<decimal>::infinity();

        for (int32 i = 0; i < 3; ++i)
        {
            decimal e = vector3d::dot(axis[i], delta);
            decimal f = vector3d::dot(dir, axis[i]);

            if (std::fabs(f) > 0.00001)
            {
                decimal t1 = (e - extents[i]) / f;
                decimal t2 = (e + extents[i]) / f;

                if (t1 > t2)
                    std::swap(t1, t2);

                if (t1 > tIn)
                    tIn = t1;

                if (t2 < tOut)
                    tOut = t2;

                if (tIn > tOut || tOut < 0)
                    return false;
            }
            else if (-e - extents[i] > 0 || -e + extents[i] < 0)
            {
                return false;
            }
        }

if(info!=NULL){
info->eDist=tIn;
info->ePoint = origin + dir * tIn;
info->oDist=tOut;
        info->oPoint = origin + dir * tOut;
}
        return true;
    }
}
