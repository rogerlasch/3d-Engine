

#include"geometry.h"

using namespace std;

namespace gpp
{
box3d::box3d(const vector3d& min, const vector3d& measures):
shape(GTYPE_BOX)
{
this->min=min;
this->measures=measures;
}

box3d::box3d(const box3d& b)
{
*this=b;
}

box3d& box3d::operator=(const box3d& b)
{
this->min=b.min;
this->measures=b.measures;
return *this;
}

box3d:: ~box3d()
{
}

 float box3d::getVolume()
{
return (measures.x*measures.y)*measures.z;
}

 vector3d box3d::getCenter()
{
vector3d v=measures/2;
return (min+v);
}

 void box3d::scale(float sk)
{
measures*=sk;
}

 void box3d::translate(const vector3d& v)
{
min+=v;
}

 void box3d::rotate(float angle, const vector3d& origin)
{
}
}
