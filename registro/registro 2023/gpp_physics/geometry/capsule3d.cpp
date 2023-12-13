

#include<sstream>
#include"geometry.h"

using namespace std;

namespace gpp
{
capsule3d::capsule3d(const vector3d& min, const vector3d& max, float radius):
GeometricShape(GTYPE_CAPSULE)
{
this->min=min;
this->max=max;
this->radius=radius;
}

capsule3d::capsule3d(const capsule3d& b):
GeometricShape(GTYPE_CAPSULE)
{
*this=b;
}

capsule3d& capsule3d::operator=(const capsule3d& b)
{
this->min=b.min;
this->max=b.max;
this->radius=b.radius;
return *this;
}

capsule3d:: ~capsule3d()
{
}

string capsule3d::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Tipo: "<<getGeometricType()<<": cápsula"<<endl;
string s="XYZ";
for(uint32 i=0; i<3; i++){
ss<<s[i]<<": "<<min[i]<<":"<<max[i]<<endl;
}
ss<<"Raio: "<<radius<<endl;
return ss.str();
}

float capsule3d::getProjectedArea()const{
return 0.0f;
}

 vector3d capsule3d::getCenter()const
{
return (min+max)*0.5f;
}

   void capsule3d::translate(const vector3d& translation, bool correction)
{
if(!correction){
setLastPosition((min+max)*0.5f);
}
this->min+=translation;
this->max+=translation;
}

   void capsule3d::scale(const vector3d& origin, float s)
{
radius*=s;
min-=origin;
max-=origin;
min*=s;
max*=s;
min+=origin;
max+=origin;
}

 void capsule3d::scale(const vector3d& origin, const vector3d& sc)
{

}

void capsule3d::rotate(const vector3d& origin, const quaternion& orientation)
{
min-=origin;
max-=origin;
min=quaternion_vector_rotate(orientation, min);
max=quaternion_vector_rotate(orientation, max);
min+=origin;
max+=origin;
for(uint32 i=0; i<3; i++){
if(max[i]<min[i]) swap(min[i], max[i]);
}
}

 matrix3x3 capsule3d::getInertiaTensor(float mass)const
{
  matrix3x3 inertiaTensor;
  return inertiaTensor;
}
}
