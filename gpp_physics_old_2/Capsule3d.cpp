
#include<sstream>
#include"types.h"
#include<gpp/debug_system.h>
#include"math/math.h"
#include"RigidBody.h"
#include"Capsule3d.h"

using namespace std;

namespace gpp{

Capsule3d::Capsule3d(float alf, float radius):
RigidBody(){
this->alf=alf;
this->radius=radius;
setGeometricType(GTYPE_CAPSULE);
calculateInertia();
}

Capsule3d::~Capsule3d(){
}

string Capsule3d::toString()const{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Exibindo cápsula de id : "<<id<<endl;
ss<<"Mass="<<mass<<", InverseMass="<<inverseMass<<endl;
ss<<"Position="<<position<<endl;
ss<<"Velocity="<<velocity<<", AngularVelocity="<<angularVelocity<<endl;
ss<<"EulerAngles="<<quaternion_extract_euler_angles(orientation)<<", Orientation="<<orientation<<endl;
ss<<"Length="<<(alf*2)<<", Radius="<<radius<<endl;
vector3d min, max;
getAabb(min, max);
string s="XYZ";
ss<<"AABB:"<<endl;
for(uint32 i=0; i<3; i++){
ss<<s[i]<<": "<<min[i]<<", "<<max[i]<<endl;
}
return ss.str();
}

void Capsule3d::calculateInertia(){
if(mass<=0.0f){
inertia.clear();
invInertia.clear();
vecInertia={0.0f, 0.0f, 0.0f};
return;
}
vector3d v;
float height=alf*2.0f;
        // Cálculo da matriz de inércia para o cilindro
        float cylinderInertiaX = (1.0f / 12.0f) * mass * (3 * radius * radius + height * height);
        float cylinderInertiaY = (1.0f / 12.0f) * mass * (radius * radius + (height * height) / 4.0f);
        float cylinderInertiaZ = (1.0f / 12.0f) * mass * (3 * radius * radius + height * height);

        // Cálculo da matriz de inércia para as esferas
        float sphereInertia = (2.0f / 5.0f) * mass * radius * radius;

        // Soma das contribuições do cilindro e das duas esferas
v.x = cylinderInertiaX + 2 * sphereInertia;
v.y = cylinderInertiaY + 2 * sphereInertia;
v.z = cylinderInertiaZ + 2 * sphereInertia;
inertia.setDiagonal(v);
invInertia=inertia.inverse();
vecInertia=invInertia.getDiagonal();
}

void Capsule3d::getLine(vector3d& min,vector3d& max)const{
vector3d top={0,0,1};
top=quaternion_vector_rotate(orientation, top)*alf;
min=position-top;
max=position+top;
for(uint32 i=0; i<3; i++){
if(min[i]>max[i])swap(min[i], max[i]);
}
}

float Capsule3d::getAabb(vector3d& min, vector3d& max)const{
vector3d top={0,0,1};
top=quaternion_vector_rotate(orientation, top)*alf;
min=position-top;
max=position+top;
for(uint32 i=0; i<3; i++){
if(min[i]>max[i])swap(min[i], max[i]);
}
min-=radius;
max+=radius;
vector3d v=max-min;
return v.x*v.y*v.z;
}
}
