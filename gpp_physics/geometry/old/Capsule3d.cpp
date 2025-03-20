#include "geometry.h"

using namespace std;

namespace gpp{
Capsule3d::Capsule3d(const vector3d& start, const vector3d& end, decimal radius, const quaternion& orientation) :
    GeometricShape(GTYPE_CAPSULE, (start+end)*0.5, orientation) {
this->radius=fabs(radius);
this->length=(end-start).length();
axis=((length>GPP_EPSILON) ? (end-start).normalize() : vector3d(0, 1, 0));

localStart=-axis*(length*0.5);
localEnd=axis*(length*0.5);
updateCachedPoints();
}

Capsule3d::~Capsule3d() {
}

void Capsule3d::getEndPointsLocal(vector3d& tstart, vector3d& tend)const{
tstart=localStart;
tend=localEnd;
}

void Capsule3d::getEndPointsGlobal(vector3d& tstart, vector3d& tend)const{
tstart=globalStart;
tend=globalEnd;
}

void Capsule3d::updateCachedPoints(){
matrix3x3 mt=orientation.toMatrix3x3();
globalStart=position+(mt*localStart);
globalEnd=position+(mt*localEnd);
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
    ss << "}"<<endl;

ss<<"World Start: " << globalStart << "\n"
       << "World End: " << globalEnd << "\n";


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
/*
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
*/

    // Find furthest point along direction in world space
    const vector3d normDir = vector3d::normalize(dir);
    const decimal dotStart = normDir * globalStart;
    const decimal dotEnd = normDir * globalEnd;
    const vector3d extremePoint = (dotStart > dotEnd) ? globalStart : globalEnd;
    return extremePoint + (normDir * radius);
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
/*
    this->orientation =this->orientation*  q;
    this->orientation.normalize();
position=quaternion_vector_rotate(q, position);
axis=quaternion_vector_rotate(q, axis);
*/

this->    orientation = q * orientation;
    orientation.normalize();
updateCachedPoints();

}

void Capsule3d::rotate(const vector3d& origin, const quaternion& q) {
/*
    position -= origin;
    position = quaternion_vector_rotate(q, position) + origin;
axis = quaternion_vector_rotate(q, axis);
    orientation =orientation  * q;
    orientation.normalize();
*/

    vector3d relativePos = position - origin;
    relativePos = quaternion_vector_rotate(q,  relativePos);
    position = origin + relativePos;
    orientation = q * orientation;
    orientation.normalize();
updateCachedPoints();

}

void Capsule3d::translate(const vector3d& ts) {
setLastPosition(position);
    this->position += ts;
updateCachedPoints();
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
