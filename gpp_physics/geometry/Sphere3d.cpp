
#include<cmath>
#include"geometry.h"

using namespace std;

namespace gpp {

Sphere3d::Sphere3d(decimal radius, const Transform& transform)
    : GeometricShape(GTYPE_SPHERE, transform), radius(radius) {}

string Sphere3d::toString() const {
stringstream ss;
ss<<fixed;
ss.precision(2);
quaternion q=transform.getOrientation();
AABB ab=getAABB();
ss<<"Type="<<type<<" Sphere3d, Radius="<<radius<<endl;
ss<<"Position="<<transform.getPosition()<<endl;
ss<<"Scale="<<transform.getScale()<<endl;
ss<<"Orientation Euler="<<quaternion_extract_euler_angles(q)<<endl;
ss<<"Orientation="<<q<<endl;
ss<<ab.toString();
return ss.str();
}

vector3d Sphere3d::getClosestPoint(const vector3d& pt) const {
    vector3d center = transform.getPosition();
    vector3d direction = (pt - center).normalize();
    return center + direction * radius;
}

bool Sphere3d::contains(const vector3d& pt) const {
    return (pt - transform.getPosition()).lengthSquared() <= radius * radius;
}

bool Sphere3d::rayCast(RayInfo* info) const {
    vector3d center = transform.getPosition();
    vector3d oc = info->origin - center;
    decimal a = info->dir.lengthSquared();
    decimal b = 2.0f * vector3d::dot(oc, info->dir);
    decimal c = oc.lengthSquared() - radius * radius;
    decimal discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return false;  // Sem interseção
    } else {
        decimal sqrtDiscriminant = std::sqrt(discriminant);
        decimal t1 = (-b - sqrtDiscriminant) / (2.0f * a);
        decimal t2 = (-b + sqrtDiscriminant) / (2.0f * a);

        // Ordena t1 e t2 para garantir que t1 seja o ponto de entrada e t2 o ponto de saída
        if (t1 > t2) std::swap(t1, t2);

        // Verifica se o raio começa dentro da esfera
        if (t1 < 0) {
            t1 = t2;  // Se t1 < 0, o raio começa dentro da esfera, então t2 é o ponto de saída
            if (t1 < 0) return false;  // Se t2 também for negativo, não há interseção
        }

        // Preenche os dados de interseção
        info->enterDist = t1;
        info->enterPoint = info->origin + info->dir * t1;
        info->outDist = t2;
        info->outPoint = info->origin + info->dir * t2;
        info->colliding = true;

        return true;
    }
}

AABB Sphere3d::getAABB() const {
    vector3d center = transform.getPosition();
    return AABB(center - vector3d(radius, radius, radius),
                center + vector3d(radius, radius, radius));
}

decimal Sphere3d::getVolume() const {
    return (4.0f / 3.0f) * GPP_PI * radius * radius * radius;
}

decimal Sphere3d::getSurfaceArea() const {
    return 4.0f * GPP_PI * radius * radius;
}

matrix3x3 Sphere3d::getInertiaTensor(decimal mass) const {
    decimal I = (2.0f / 5.0f) * mass * radius * radius;
    return matrix3x3().setDiagonal({I, I, I});
}
} // namespace gpp
