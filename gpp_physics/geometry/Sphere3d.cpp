#include "geometry.h"

using namespace std;

namespace gpp{
Sphere3d::Sphere3d(const vector3d& position, decimal radius, const quaternion& orientation) :
    GeometricShape(GTYPE_SPHERE, position, orientation) {
    this->radius = radius;
}

Sphere3d::~Sphere3d() {
}

string Sphere3d::toString() const {
    vector3d m1, m2;
    getAABB(m1, m2);
    string axis = "XYZ";
    stringstream ss;
    ss << fixed;
    ss.precision(2);
    ss << "Type=" << getType() << "(Esfera), Raio=" << radius << endl;
    ss << "Position=" << position << endl;
    ss << "AABB={" << endl;
    for (uint32_t i = 0; i < 3; i++) {
        ss << axis[i] << "=" << m1[i] << ", " << m2[i] << endl;
    }
    ss << "}";
    return ss.str();
}

bool Sphere3d::collidingPoint(const vector3d& pt) {
    vector3d v = position - pt;
    decimal sqDist = v * v;
    return sqDist <= (radius * radius);
}

vector3d Sphere3d::getClosestPoint(const vector3d& pt) {
    vector3d v = position - pt;
    v.normalize();
    return position + (v * radius);
}

void Sphere3d::rotate(const quaternion& q) {
    this->orientation =this->orientation*  q;
    this->orientation.normalize();
}

void Sphere3d::rotate(const vector3d& origin, const quaternion& q) {
    position -= origin;
    position = quaternion_vector_rotate(q, position) + origin;
    orientation =orientation  * q;
    orientation.normalize();
}

void Sphere3d::translate(const vector3d& ts) {
setLastPosition(position);
    this->position += ts;
}

void Sphere3d::getAABB(vector3d& tMin, vector3d& tMax)const {
    tMin = position - radius;
    tMax = position + radius;
}
}
