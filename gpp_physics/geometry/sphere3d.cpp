

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

 vector3d sphere3d::GetCenter()const
{
return center;
}

   bool sphere3d::Contains(const vector3d& point) const
{
float sqdist=vector3d::get_squared_distance(center, point);
return sqdist<=(radius*radius);
}

   void sphere3d::Translate(const vector3d& translation)
{
center+=translation;
}

   void sphere3d::Scale(float scale)
{
radius/=scale;
}

 void sphere3d::Scale(const vector3d& scale)
{
}

void sphere3d::Rotate(const quaternion& orientation)
{
vector3d origem=this->GetCenter();
Translate(vector3d::reverse(origem));
center=quaternion_vector_rotate(orientation, center);
Translate(origem);
}

   vector3d sphere3d::ClosestPointOnSurface(const vector3d& point) const
{
vector3d direction=center-point;
direction.normalize();
return center+direction*radius;
}

   float sphere3d::Volume() const
{
return (4.0f / 3.0f) * GPP_PI * radius * radius * radius;
}

 matrix3x3 sphere3d::GetInertiaTensor(float mass)const
{
  float i = (2.0f/5.0f) * mass * radius * radius;
  matrix3x3 inertiaTensor;
  inertiaTensor.setDiagonal(vector3d(i,i,i));
  return inertiaTensor;
}
}
