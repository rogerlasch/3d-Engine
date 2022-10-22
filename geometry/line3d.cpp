

#include"..\\gpp\\gpp.h"
#include"geometry.h"

using namespace std;

namespace gpp
{
line3d::line3d(const vector3d& min, const vector3d& max):
shape(GTYPE_LINE)
{
this->min=min;
this->max=max;
}

line3d::line3d(const line3d& b)
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

 float line3d::getVolume()
{
return vector3d::get_distance(min, max);
}

 vector3d line3d::getCenter()
{
return (min+max)/2;
}

 void line3d::scale(float sk)
{
max*=sk;
}

 void line3d::translate(const vector3d& v)
{
min+=v;
max+=v;
}

 void line3d::rotate(float angle, const vector3d& origin)
{
}
}
