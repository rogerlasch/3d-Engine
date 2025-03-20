

#include"geometry.h"

using namespace std;

namespace gpp {

AABB::AABB(const vector3d& m1, const vector3d& m2)
    : min(m1), max(m2) {
    // Garante que min e max estejam corretamente ordenados
    if (min.x > max.x) swap(min.x, max.x);
    if (min.y > max.y) swap(min.y, max.y);
    if (min.z > max.z) swap(min.z, max.z);
}

AABB::AABB(const AABB& ab)
    : min(ab.min), max(ab.max) {}

AABB& AABB::operator=(const AABB& ab) {
    if (this != &ab) {
        min = ab.min;
        max = ab.max;
    }
    return *this;
}

string AABB::toString() const {
    stringstream ss;
ss<<fixed;
ss.precision(2);

    ss << "AABB:{\n";
string axis="XYZ";
ss<<"Center="<<getCenter()<<", Extents="<<getExtents()<<endl;
for(uint32 i=0; i<3; i++){
ss<<axis[i]<<": "<<min[i]<<", "<<max[i]<<endl;
}
ss<<"}";
    return ss.str();
}

vector3d AABB::getCenter() const {
    return (min + max) * 0.5f;
}

// Retorna as extens�es da AABB (largura, altura, profundidade)
vector3d AABB::getExtents() const {
    return (max - min) * 0.5f;
}

vector3d AABB::getDirection(const AABB& ab) const {
    return (ab.getCenter() - getCenter()).normalize();
}

// Verifica se h� interse��o com outra AABB
bool AABB::intersects(const AABB& ab) const {
    return (min.x <= ab.max.x && max.x >= ab.min.x) &&
           (min.y <= ab.max.y && max.y >= ab.min.y) &&
           (min.z <= ab.max.z && max.z >= ab.min.z);
}

void AABB::addAABB(const AABB& ab) {
    min.x = std::min(min.x, ab.min.x);
    min.y = std::min(min.y, ab.min.y);
    min.z = std::min(min.z, ab.min.z);

    max.x = std::max(max.x, ab.max.x);
    max.y = std::max(max.y, ab.max.y);
    max.z = std::max(max.z, ab.max.z);
}

bool AABB::rayCast(RayInfo* info) const {
    if (!info) return false;

    vector3d dirInv = vector3d(1.0f / info->dir.x, 1.0f / info->dir.y, 1.0f / info->dir.z);

    decimal t1 = (min.x - info->origin.x) * dirInv.x;
    decimal t2 = (max.x - info->origin.x) * dirInv.x;
    decimal t3 = (min.y - info->origin.y) * dirInv.y;
    decimal t4 = (max.y - info->origin.y) * dirInv.y;
    decimal t5 = (min.z - info->origin.z) * dirInv.z;
    decimal t6 = (max.z - info->origin.z) * dirInv.z;

    decimal tmin = std::max(std::max(std::min(t1, t2), std::min(t3, t4)), std::min(t5, t6));
    decimal tmax = std::min(std::min(std::max(t1, t2), std::max(t3, t4)), std::max(t5, t6));

    // Se tmax < 0, o raio est� apontando para longe da AABB
    if (tmax < 0) {
        info->colliding = false;
        return false;
    }

    // Se tmin > tmax, o raio n�o intersecta a AABB
    if (tmin > tmax) {
        info->colliding = false;
        return false;
    }

    // Atualiza as informa��es do RayInfo
    info->colliding = true;
    info->enterDist = tmin;
    info->outDist = tmax;
    info->enterPoint = info->origin + info->dir * tmin;
    info->outPoint = info->origin + info->dir * tmax;

    return true;
}
} // namespace gpp
