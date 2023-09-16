

#include<sstream>
#include"geometry.h"

using namespace std;

namespace gpp
{
cylinder3d::cylinder3d(const vector3d& min, const vector3d& max, float radius):
GeometricShape(GTYPE_CYLINDER)
{
this->min=min;
this->max=max;
this->radius=radius;
}

cylinder3d::cylinder3d(const cylinder3d& b):
GeometricShape(GTYPE_CYLINDER)
{
*this=b;
}

cylinder3d& cylinder3d::operator=(const cylinder3d& b)
{
this->min=b.min;
this->max=b.max;
this->radius=b.radius;
return *this;
}

cylinder3d:: ~cylinder3d()
{
}

string cylinder3d::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Tipo: "<<getGeometricType()<<": cilindro"<<endl;
string s="XYZ";
for(uint32 i=0; i<3; i++){
ss<<s[i]<<": "<<min[i]<<":"<<max[i]<<endl;
}
ss<<"Raio: "<<radius<<endl;
return ss.str();
}

 vector3d cylinder3d::getCenter()const
{
return (min+max)*0.5f;
}

   void cylinder3d::translate(const vector3d& translation)
{
setLastPosition((min+max)*0.5f);
this->min+=translation;
this->max+=translation;
}

   void cylinder3d::scale(const vector3d& origin, float s)
{
radius*=s;
min-=origin;
max-=origin;
min*=s;
max*=s;
min+=origin;
max+=origin;
}

 void cylinder3d::scale(const vector3d& origin, const vector3d& sc)
{

}

void cylinder3d::rotate(const vector3d& origin, const quaternion& orientation)
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

 matrix3x3 cylinder3d::getInertiaTensor(float mass)const
{
  matrix3x3 inertiaTensor;
  return inertiaTensor;
}
}
