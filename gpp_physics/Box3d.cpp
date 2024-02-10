
#include<sstream>
#include<gpp/debug_system.h>
#include"types.h"
#include"math/math.h"
#include"RigidBody.h"
#include"Box3d.h"

using namespace std;

namespace gpp{

Box3d::Box3d(const vector3d& alf):
RigidBody(){
this->alf=alf;
setGeometricType(GTYPE_BOX);
calculateInertia();
}

Box3d::~Box3d(){
}

string Box3d::toString()const{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Exibindo caixa de id : "<<id<<endl;
ss<<"Mass="<<mass<<", InverseMass="<<inverseMass<<endl;
ss<<"Position="<<position<<endl;
ss<<"Velocity="<<velocity<<", AngularVelocity="<<angularVelocity<<endl;
ss<<"EulerAngles="<<quaternion_extract_euler_angles(orientation)<<", Orientation="<<orientation<<endl;
vector3d min, max;
getAabb(min, max);
string s="XYZ";
ss<<"AABB:"<<endl;
for(uint32 i=0; i<3; i++){
ss<<s[i]<<": "<<min[i]<<", "<<max[i]<<endl;
}
return ss.str();
}

void Box3d::calculateInertia(){
if(mass<=0.0f){
inertia.clear();
invInertia.clear();
vecInertia={0.0f, 0.0f, 0.0f};
return;
}
vector3d v;
vector3d min, max;
vector3d af=alf*2.0f;

v.x=mass * (af.x * af.x + v.y * v.y) / 12.0;
v.y=mass * (v.y * v.y + v.z * v.z) / 12.0;
v.z=mass * (af.x * v.x + v.z * v.z) / 12.0;
inertia.setDiagonal(v);
invInertia = inertia.inverse();
vecInertia=invInertia.getDiagonal();
}

void Box3d::getMinMax(vector3d& min, vector3d& max) {
    vector3d localMin =-alf;
    vector3d localMax = alf;

    // Transforma os pontos locais para o espaço do mundo
    vector3d worldMin = quaternion_vector_rotate(orientation, localMin) + position;
    vector3d worldMax = quaternion_vector_rotate(orientation, localMax) + position;

    // Atualiza os valores de mínimo e máximo
    min = vector3d(std::min(worldMin.x, worldMax.x), std::min(worldMin.y, worldMax.y), std::min(worldMin.z, worldMax.z))+numeric_limits<float>::epsilon();
    max = vector3d(std::max(worldMin.x, worldMax.x), std::max(worldMin.y, worldMax.y), std::max(worldMin.z, worldMax.z))+numeric_limits<float>::epsilon();
}

float Box3d::getAabb(vector3d& min, vector3d& max)const{
min=position-alf;
max=position+alf;
vector3d v=max-min;
return v.x*v.y*v.z;
}
}
