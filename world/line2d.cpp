

#include"geometry.h"

using namespace std;

namespace gpp
{
line2d::line2d(const vector3d& min, const vector3d& max):
shape2d(SD2_LINE)
{
this->min=min;
this->max=max;
}

line2d::line2d(const line2d& b):
shape2d(SD2_LINE)
{
*this=b;
}

line2d& line2d::operator=(const line2d& b)
{
this->min=b.min;
this->max=b.max;
return *this;
}

line2d::~line2d()
{
}

 float line2d::getArea()
{
vector3d v(min-max);
v=v*v;
return sqrt((v.x+v.y));
}

 vector3d line2d::getCenter()
{
vector3d v=((max-min)/2);
return (min+v);
}

void line2d::translate(const vector3d& v)
{
min+=v;
max+=v;
}

void line2d::rotate(float angle, const vector3d& origin)
{
gpp_rotate_single_point(min, angle, origin);
gpp_rotate_single_point(max, angle, origin);
}

bool line2d::isColiding(shape2d* ch)
{
Shape2dConverter df;
return df(this, ch);
}
}
