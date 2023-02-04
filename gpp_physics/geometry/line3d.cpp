

#include"geometry.h"

using namespace std;

namespace gpp
{
line3d::line3d(const vector3d& min, const vector3d& max):
GeometricShape(GTYPE_LINE)
{
this->min=min;
this->max=max;
}

line3d::line3d(const line3d& b):
GeometricShape(GTYPE_LINE)
{
*this=b;
}

line3d& line3d::operator=(const line3d& b)
{
this->min=b.min;
this->max=b.max;
return *this;
}

line3d:: ~line3d()
{
}

 vector3d line3d::GetCenter()const
{
return (min+max)*0.5f;
}

   bool line3d::Contains(const vector3d& point) const
{
float dist=SqDistPointSegment(min, max, point);
return dist<=0.01f;
}

   void line3d::Translate(const vector3d& translation)
{
min+=translation;
max+=translation;
}

   void line3d::Scale(float scale)
{
min=min*scale;
max=max*scale;
}

 void line3d::Scale(const vector3d& scale)
{
min=min*scale;
max=max*scale;
}

void line3d::Rotate(const quaternion& orientation)
{
vector3d start=min;
vector3d end=max;
start=quaternion_vector_rotate(orientation, start);
end=quaternion_vector_rotate(orientation, end);
min = vector3d(std::min(start.x, end.x), std::min(start.y, end.y), std::min(start.z, end.z));
max = vector3d(std::max(start.x, end.x), std::max(start.y, end.y), std::max(start.z, end.z));
}

   vector3d line3d::ClosestPointOnSurface(const vector3d& point) const
{
float f=0;
vector3d f2;
ClosestPtPointSegment(point, min, max, f, f2);
return f2;
}

   float line3d::Volume() const
{
return 0.0f;
}

 matrix3x3 line3d::GetInertiaTensor(float mass)const
{
return matrix3x3();
}
}
