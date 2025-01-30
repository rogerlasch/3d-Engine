#include "geometry.h"

using namespace std;

namespace gpp{
Capsule3d::Capsule3d(const vector3d& p1, const vector3d& p2, decimal radius, const quaternion& orientation) :
    GeometricShape(GTYPE_CAPSULE, {0.0f, 0.0f, 0.0f}, orientation) {
this->radius=radius;
position=(p1+p2)*0.5f;
length=(p2-p1).length();
axis=(p2-p1).normalize();
}

Capsule3d::~Capsule3d() {
}

string Capsule3d::toString() const {
    vector3d m1, m2;
vector3d p1, p2;
    getAABB(m1, m2);
getSegment(p1, p2);
    string axis = "XYZ";
    stringstream ss;
    ss << fixed;
    ss.precision(2);
    ss << "Type=" << getType() << "(Cápsula), Radius=" <<radius<<", Length="<<length<< endl;
    ss << "Position=" << position << endl;
    ss << "AABB={" << endl;
    for (uint32_t i = 0; i < 3; i++) {
        ss << axis[i] << "=" << m1[i] << ", " << m2[i] << endl;
    }
    ss << "}";
    return ss.str();
}

bool Capsule3d::collidingPoint(const vector3d& pt) {
decimal t=0.0f;
vector3d p1, p2, closestPoint;
getSegment(p1, p2);
ClosestPtPointSegment(pt, p1, p2, t, closestPoint);
vector3d v=closestPoint-pt;
t=vector3d::dot(v, v);
return t<=(radius*radius);
}

vector3d Capsule3d::getSupportPoint(const vector3d& dir)const{
vector3d a=position-(axis*length*0.5f);
vector3d b=position+(axis*length*0.5f);
vector3d ab=b-a;

decimal d=dir*ab;

if(d>0.0){
return b+(dir*radius);
} else if(d<0.0f){
return a+(dir*radius);
}
return position+(dir*radius);
}

vector3d Capsule3d::getClosestPoint(const vector3d& pt) {
vector3d closestPoint, p1, p2, vdir;
decimal t=0.0f;
getSegment(p1, p2);
ClosestPtPointSegment(pt, p1, p2, t, closestPoint);
vdir=(pt-closestPoint).normalize();
closestPoint=closestPoint+(vdir*radius);
return closestPoint;
}

void Capsule3d::rotate(const quaternion& q) {
    this->orientation =this->orientation*  q;
    this->orientation.normalize();
position=quaternion_vector_rotate(q, position);
axis=quaternion_vector_rotate(q, axis);
}

void Capsule3d::rotate(const vector3d& origin, const quaternion& q) {
    position -= origin;
    position = quaternion_vector_rotate(q, position) + origin;
axis = quaternion_vector_rotate(q, axis);
    orientation =orientation  * q;
    orientation.normalize();
}

void Capsule3d::translate(const vector3d& ts) {
setLastPosition(position);
    this->position += ts;
}

void Capsule3d::getAABB(vector3d& tMin, vector3d& tMax)const {
tMin=position-(axis*(length*0.5f));
tMax=position+(axis*(length*0.5f));
for(uint32 i=0; i<3; i++){
if(tMin[i]>tMax[i])swap(tMin[i], tMax[i]);
}
tMin-=radius;
tMax+=radius;
}

void Capsule3d::getSegment(vector3d& p1, vector3d& p2)const{
decimal alf=length*0.5f;
p1=position-(axis*alf);
p2=position+(axis*alf);
}
}
