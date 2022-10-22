

#include"geometry.h"

using namespace std;

namespace gpp
{
box2d::box2d(const vector3d& position, const vector3d& measures):
shape2d(SD2_BOX)
{
this->min=position;
this->measures=measures;
this->max=(this->min+this->measures);
}

box2d::box2d(const box2d& b):
shape2d(SD2_BOX)
{
*this=b;
}

box2d& box2d::operator=(const box2d& b)
{
this->min=b.min;
this->measures=b.measures;
this->max=b.max;
return *this;
}

box2d:: ~box2d()
{
}

 float box2d::getArea()
{
return (measures.x*measures.y);
}

 vector3d box2d::getCenter()
{
return (measures/(float)2)+min;
}

BoundingBox2d box2d::getBoundingBox()const
{
return BoundingBox2d(min, {min.x+measures.x, min.y}, {min.x, min.y+measures.y}, max);
}

void box2d::translate(const vector3d& v)
{
min+=v;
max+=v;
}

void box2d::rotate(float angle, const vector3d& origin)
{
gpp_rotate_single_point(min, angle, origin);
gpp_rotate_single_point(max, angle, origin);
}

bool box2d::isColiding(shape2d* ch)
{
Shape2dConverter df;
return df(this, ch);
}
}
