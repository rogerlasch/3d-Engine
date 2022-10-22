

#include"geometry.h"

using namespace std;

namespace gpp
{
sphere3d::sphere3d(const vector3d& center, float radius):
shape(GTYPE_SPHERE)
{
this->center=center;
this->radius=radius;
}

sphere3d::sphere3d(const sphere3d& b)
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

 float sphere3d::getVolume()
{
return (float(4)/float(3))*GPP_PI*radius*radius*radius;
}

 vector3d sphere3d::getCenter()
{
return this->center;
}

 void sphere3d::scale(float sk)
{
this->radius*=sk;
}

 void sphere3d::translate(const vector3d& v)
{
center+=v;
}

 void sphere3d::rotate(float angle, const vector3d& origin)
{
}
}
