
#include<sstream>
#include"types.h"
#include<gpp/debug_system.h>
#include"math/math.h"
#include"RigidBody.h"
#include"Sphere3d.h"

using namespace std;

namespace gpp{

Sphere3d::Sphere3d(float radius):
RigidBody(){
this->radius=radius;
setGeometricType(GTYPE_SPHERE);
calculateInertia();
}

Sphere3d::~Sphere3d(){
}

string Sphere3d::toString()const{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Exibindo esfera de id : "<<id<<endl;
ss<<"Mass="<<mass<<", InverseMass="<<inverseMass<<endl;
ss<<"Position="<<position<<endl;
ss<<"Velocity="<<velocity<<", AngularVelocity="<<angularVelocity<<endl;
ss<<"EulerAngles="<<quaternion_extract_euler_angles(orientation)<<", Orientation="<<orientation<<endl;
ss<<"Radius="<<radius<<endl;
vector3d min, max;
getAabb(min, max);
string s="XYZ";
ss<<"AABB:"<<endl;
for(uint32 i=0; i<3; i++){
ss<<s[i]<<": "<<min[i]<<", "<<max[i]<<endl;
}
return ss.str();
}

void Sphere3d::calculateInertia(){
if(mass<=0.0f){
inertia.clear();
invInertia.clear();
vecInertia={0.0f, 0.0f, 0.0f};
return;
}
vector3d v;
        float sphereInertia = (2.0f / 5.0f) * mass * radius * radius;
v={sphereInertia, sphereInertia, sphereInertia};
        // Configuração do tensor de inércia
inertia.setDiagonal(v);
invInertia=inertia.inverse();
vecInertia=invInertia.getDiagonal();
}

float Sphere3d::getAabb(vector3d& min, vector3d& max)const{
min=position-radius;
max=position+radius;
vector3d v=max-min;
return v.x*v.y*v.z;
}

}
