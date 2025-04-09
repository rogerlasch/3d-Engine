
#include<algorithm>
#include<numeric>
#include"geometry.h"

using namespace std;

namespace gpp {

Capsule3d::Capsule3d(const vector3d& startPoint, const vector3d& endPoint, decimal radius, Transform* transform)
    : GeometricShape(GTYPE_CAPSULE, transform), radius(radius){
vector3d pos=(startPoint+endPoint)*0.5f;

this->transform->setPosition(pos);
this->axis=(endPoint-startPoint).normalize();
this->eight=(endPoint-startPoint).length();
}

string Capsule3d::toString() const {
    stringstream ss;
    ss << fixed;
    ss.precision(2);

    quaternion q = transform->getOrientation();
    AABB ab;
getAABB(&ab);
vector3d tstart, tend;
getSegment(tstart, tend);

    ss << "Type=" << type << " Capsule3d, radius=" << radius << endl;
ss<<"axis="<<axis<<endl;
    ss << "Start Point=" <<tstart<<endl;
    ss << "End Point=" <<tend<<endl;
    ss << "Position=" << transform->getPosition() << endl;
    ss << "Scale=" << transform->getScale() << endl;
    ss << "Orientation Euler=" << quaternion_extract_euler_angles(q) << endl;
    ss << "Orientation=" << q << endl;
    ss << ab.toString();

    return ss.str();
}

string Capsule3d::getShortDescription()const{
stringstream ss;
ss<<fixed;
ss.precision(2);

ss<<"Type=Capsule3d, Axis="<<axis<<", Radius="<<radius<<", Height="<<eight;
return ss.str();
}

void Capsule3d::getSegment(vector3d& tstart, vector3d& tend)const {
decimal alf=eight*0.5;
vector3d position=transform->getPosition();
tstart=position-(axis*alf);
tend=position+(axis*alf);
}

vector3d Capsule3d::getClosestPoint(const vector3d& pt) const {
vector3d tstart, tend;
getSegment(tstart, tend);
    vector3d segment = tend-tstart;
    vector3d toPt = pt - tstart;
    decimal t = vector3d::dot(toPt, segment) / segment.lengthSquared();

    // Limita t ao intervalo [0, 1] para garantir que o ponto esteja no segmento
    t = std::max(0.0, std::min(1.0, t));

    // Ponto mais pr�ximo no segmento
    vector3d closestOnSegment = tstart+ segment * t;

    // Dire��o do ponto mais pr�ximo no segmento para o ponto dado
    vector3d direction = (pt - closestOnSegment).normalize();

    // Ponto mais pr�ximo na superf�cie da c�psula
    return closestOnSegment + direction * radius;
}

vector3d Capsule3d::getNormal(const vector3d& pt)const{
decimal t=0.0f;
vector3d p1, p2, closestPoint;
getSegment(p1, p2);
ClosestPtPointSegment(pt, p1, p2, t, closestPoint);

return (pt-closestPoint).normalize();
}

bool Capsule3d::contains(const vector3d& pt) const {

vector3d startPoint, endPoint;
getSegment(startPoint, endPoint);

    vector3d segment = endPoint - startPoint;
    vector3d toPt = pt - startPoint;
    decimal t = vector3d::dot(toPt, segment) / segment.lengthSquared();

    // Limita t ao intervalo [0, 1]
    t = std::max(0.0, std::min(1.0, t));

    // Ponto mais pr�ximo no segmento
    vector3d closestOnSegment = startPoint + segment * t;

    // Verifica se a dist�ncia ao ponto mais pr�ximo � menor ou igual ao raio
    return (pt - closestOnSegment).lengthSquared() <= radius * radius;
}

bool Capsule3d::rayCast(RayInfo* info) const {

//Fun��o lambda...

auto raySphereAux=[this] (const vector3d& origin, const vector3d& dir, const vector3d& center, decimal radius, decimal& tMin, decimal& tMax)->bool{
    vector3d CO = origin - center;
    decimal a = vector3d::dot(dir, dir);
    decimal b = 2.0f * vector3d::dot(CO, dir);
    decimal c = vector3d::dot(CO, CO) - (radius * radius);
    decimal discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f) {
        return false; // Nenhuma interse��o
    }

    tMin = (-b - std::sqrt(discriminant)) / (2.0f * a);
    tMax = (-b + std::sqrt(discriminant)) / (2.0f * a);
    if (tMin > tMax) std::swap(tMin, tMax);

    return true;
};

    // Pontos extremos da c�psula
vector3d startPoint, endPoint;
getSegment(startPoint, endPoint);

    vector3d cMA = startPoint;
    vector3d cMB = endPoint;

    // Vetor AB (dire��o da c�psula) e comprimento
    vector3d AB = cMB - cMA;
    decimal lengthAB = AB.length();
    AB.normalize();

    // Origem e dire��o do raio
    vector3d origin = info->origin;
    vector3d dir = info->dir;

    // Vetor AO (origem do raio para o ponto inicial da c�psula)
    vector3d AO = origin - cMA;

    // C�lculos intermedi�rios
    decimal AB_dot_d = vector3d::dot(AB, dir);
    decimal AB_dot_AO = vector3d::dot(AB, AO);
    decimal AB_dot_AB = vector3d::dot(AB, AB);
    decimal m = AB_dot_d / AB_dot_AB;
    decimal n = AB_dot_AO / AB_dot_AB;

    // Vetores Q e R
    vector3d Q = dir - (AB * m);
    vector3d R = AO - (AB * n);

    // Coeficientes da equa��o quadr�tica
    decimal a = vector3d::dot(Q, Q);
    decimal b = 2.0f * vector3d::dot(Q, R);
    decimal c = vector3d::dot(R, R) - (radius * radius);

    // Caso especial: AB e dire��o do raio s�o paralelos
    if (a == 0.0f) {
        decimal atmin, atmax, btmin, btmax;
        if (!raySphereAux(origin, dir, cMA, radius, atmin, atmax) ||
            !raySphereAux(origin, dir, cMB, radius, btmin, btmax)) {
            return false; // Nenhuma interse��o
        }

        // Determina os pontos de entrada e sa�da
        info->enterDist = std::min(atmin, btmin);
        info->outDist = std::max(atmax, btmax);
        info->enterPoint = origin + dir * info->enterDist;
        info->outPoint = origin + dir * info->outDist;
        info->colliding = true;
        return true;
    }

    // Discriminante da equa��o quadr�tica
    decimal discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f) {
        return false; // Nenhuma interse��o
    }

    // Solu��es da equa��o quadr�tica
    decimal tmin = (-b - std::sqrt(discriminant)) / (2.0f * a);
    decimal tmax = (-b + std::sqrt(discriminant)) / (2.0f * a);
    if (tmin > tmax) std::swap(tmin, tmax);

    // Verifica se as interse��es est�o dentro do segmento da c�psula
    decimal t_k1 = tmin * m + n;
    decimal t_k2 = tmax * m + n;

    // Ponto de entrada
    if (t_k1 < 0.0f) {
        // Interse��o com a esfera em cMA
        decimal stmin, stmax;
        if (!raySphereAux(origin, dir, cMA, radius, stmin, stmax)) {
            return false;
        }
        info->enterDist = stmin;
        info->enterPoint = origin + dir * stmin;
    } else if (t_k1 > 1.0f) {
        // Interse��o com a esfera em cMB
        decimal stmin, stmax;
        if (!raySphereAux(origin, dir, cMB, radius, stmin, stmax)) {
            return false;
        }
        info->enterDist = stmin;
        info->enterPoint = origin + dir * stmin;
    } else {
        // Interse��o com o cilindro
        info->enterDist = tmin;
        info->enterPoint = origin + dir * tmin;
    }

    // Ponto de sa�da
    if (t_k2 < 0.0f) {
        // Interse��o com a esfera em cMA
        decimal stmin, stmax;
        if (!raySphereAux(origin, dir, cMA, radius, stmin, stmax)) {
            return false;
        }
        info->outDist = stmax;
        info->outPoint = origin + dir * stmax;
    } else if (t_k2 > 1.0f) {
        // Interse��o com a esfera em cMB
        decimal stmin, stmax;
        if (!raySphereAux(origin, dir, cMB, radius, stmin, stmax)) {
            return false;
        }
        info->outDist = stmax;
        info->outPoint = origin + dir * stmax;
    } else {
        // Interse��o com o cilindro
        info->outDist = tmax;
        info->outPoint = origin + dir * tmax;
    }

    info->colliding = true;
    return true;
}

void Capsule3d::getAABB(AABB* ab) const {
vector3d tmin, tmax;
getSegment(tmin, tmax);

for(uint32 i=0; i<3; i++){
if(tmin[i]>tmax[i]) std::swap(tmin[i], tmax[i]);
}

ab->min=tmin-radius;
ab->max=tmax+radius;
}

decimal Capsule3d::getVolume() const {
    decimal cylinderVolume = GPP_PI * radius * radius * eight;
    decimal sphereVolume = (4.0f / 3.0f) * GPP_PI * radius * radius * radius;
    return cylinderVolume + sphereVolume;
}

decimal Capsule3d::getSurfaceArea() const {
    decimal cylinderArea = 2 * GPP_PI * radius * eight;
    decimal sphereArea = 4 * GPP_PI * radius * radius;
    return cylinderArea + sphereArea;
}

matrix3x3 Capsule3d::getInertiaTensor(decimal mass) const {
    // Tensor de in�rcia aproximado para uma c�psula
    decimal Ixx = (1.0f / 12.0f) * mass * (3 * radius * radius + eight * eight);
    decimal Iyy = Ixx;
    decimal Izz = 0.5f * mass * radius * radius;
    return matrix3x3().setDiagonal({Ixx, Iyy, Izz});
}

void Capsule3d::rotate(const quaternion& q){
transform->rotate(q);
axis=quaternion_vector_rotate(transform->getOrientation(), axis);
}
} // namespace gpp
