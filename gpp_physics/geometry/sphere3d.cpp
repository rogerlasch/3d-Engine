

#include<sstream>
#include"geometry.h"

using namespace std;

namespace gpp
{
sphere3d::sphere3d(const vector3d& center, float radius):
GeometricShape(GTYPE_SPHERE)
{
this->center=center;
this->radius=radius;
}

sphere3d::sphere3d(const sphere3d& b):
GeometricShape(GTYPE_SPHERE)
{
*this=b;
}

sphere3d& sphere3d::operator=(const sphere3d& b)
{
this->center=b.center;
this->radius=b.radius;
return *this;
}

sphere3d:: ~sphere3d()
{
}

string sphere3d::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Tipo: "<<getGeometricType()<<": esfera"<<endl;
ss<<"Centro: "<<center<<endl;
ss<<"Raio: "<<radius<<endl;
return ss.str();
}

 vector3d sphere3d::getCenter()const
{
return center;
}

   void sphere3d::translate(const vector3d& translation)
{
setLastPosition(center);
center+=translation;
}

   void sphere3d::scale(float s)
{
radius*=s;
}

 void sphere3d::scale(const vector3d& sc)
{
}

void sphere3d::rotate(const quaternion& orientation)
{
vector3d origem=this->getCenter();
translate(vector3d::inverse(origem));
center=quaternion_vector_rotate(orientation, center);
translate(origem);
}

 matrix3x3 sphere3d::getInertiaTensor(float mass)const
{
  float i = (2.0f/5.0f) * mass * radius * radius;
  matrix3x3 inertiaTensor;
  inertiaTensor.setDiagonal(vector3d(i,i,i));
  return inertiaTensor;
}
}
