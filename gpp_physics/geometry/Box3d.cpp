#include<numeric>
#include "geometry.h"

using namespace std;

namespace gpp{

Box3d::Box3d(const vector3d& position, const vector3d& alf, const quaternion& orientation) :
    GeometricShape(GTYPE_BOX, position, orientation) {
this->alf=alf;
axis[0]=quaternion_vector_rotate(orientation, {1.0f, 0, 0});
axis[1]=quaternion_vector_rotate(orientation, {0, 1.0f, 0});
axis[2]=quaternion_vector_rotate(orientation, {0, 0, 1.0f});
}

Box3d::~Box3d() {
}

string Box3d::toString() const {
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Position="<<position<<endl;
ss<<"Orientation="<<orientation<<endl;
ss<<"Euler="<<quaternion_extract_euler_angles(orientation)<<endl;
ss<<"Axis={"<<endl;
for(auto& it : axis) ss<<it<<endl;
ss<<"}"<<endl;
ss<<"Vertices={"<<endl;
vector<vector3d> vertices;
getVertices(vertices);
for(auto& it : vertices) ss<<it<<endl;
ss<<"}"<<endl;
    return ss.str();
}

bool Box3d::collidingPoint(const vector3d& pt) {
for(uint32 i=0; i<3; i++){
vector3d v1=pt-position;
vector3d v2=alf[i]*axis[i];
vector3d vdir=v1-v2;
decimal dist=vdir*axis[i];

decimal len=alf[i]*-2.0f;
if(dist<len||dist>0.0f) return false;
}
return true;
}

vector3d Box3d::getSupportPoint(const vector3d& dir)const{

vector<vector3d> vertices;
decimal proj=numeric_limits<decimal>::min();
int32 index=0;
getVertices(vertices);

for(int32 i=0; i<vertices.size(); i++){
decimal d=dir*vertices[i];
if(d>proj){
proj=d;
index=i;
}
}

return vertices[index];
}

vector3d Box3d::getClosestPoint(const vector3d& pt) {

vector3d closestPoint=position;
vector3d d =pt-position;

for(uint32 i=0; i<3; i++){
decimal dist=vector3d::dot(d, axis[i]);
if(dist>alf[i]) dist=alf[i];
if(dist<(-alf[i])) dist=-alf[i];
closestPoint+=dist*axis[i];
}

return closestPoint;
}

void Box3d::rotate(const quaternion& q) {
updateOrientation(q);
position=quaternion_vector_rotate(q, position);
for(uint32 i=0; i<3; i++){
axis[i]=quaternion_vector_rotate(q, axis[i]).normalize();
}
}

void Box3d::rotate(const vector3d& origin, const quaternion& q) {
updateOrientation(q);

    position -= origin;
    position = quaternion_vector_rotate(q, position) + origin;
for(uint32 i=0; i<3; i++){
axis[i]=quaternion_vector_rotate(q, axis[i]).normalize();
}
}

void Box3d::translate(const vector3d& ts) {
setLastPosition(position);
    this->position += ts;
}

void Box3d::getAABB(vector3d& tMin, vector3d& tMax)const {
vector<vector3d> vertices;
getVertices(vertices);
tMin=tMax=position;

for(uint32 i=0; i<8; i++){
for(uint32 i1=0; i1<3; i1++){
if(vertices[i][i1]<tMin[i1]) tMin[i1]=vertices[i][i1];
else if(vertices[i][i1]>tMax[i1]) tMax[i1]=vertices[i][i1];
}
}
}

void Box3d::getVertices(vector<vector3d>& vertices)const{
vertices.clear();
vertices={
/*
            position - axis[0] - axis[1] - axis[2],
            position + axis[0] - axis[1] - axis[2],
            position - axis[0] + axis[1] - axis[2],
            position + axis[0] + axis[1] - axis[2],
            position - axis[0] - axis[1] + axis[2],
            position + axis[0] - axis[1] + axis[2],
            position - axis[0] + axis[1] + axis[2],
            position + axis[0] + axis[1] + axis[2]
*/
            position - axis[0]*alf[0] - axis[1]*alf[1] - axis[2]*alf[2],
            position + axis[0]*alf[0] - axis[1]*alf[1] - axis[2]*alf[2],
            position - axis[0]*alf[0] + axis[1]*alf[1] - axis[2]*alf[2],
            position + axis[0]*alf[0] + axis[1]*alf[1] - axis[2]*alf[2],
            position - axis[0]*alf[0] - axis[1]*alf[1] + axis[2]*alf[2],
            position + axis[0]*alf[0] - axis[1]*alf[1] + axis[2]*alf[2],
            position - axis[0]*alf[0] + axis[1]*alf[1] + axis[2]*alf[2],
            position + axis[0]*alf[0] + axis[1]*alf[1] + axis[2]*alf[2]

};
}
}
