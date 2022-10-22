

#include"geometry.h"
#include"circle2d.h"

using namespace std;

namespace gpp
{
circle2d::circle2d(const vector3d& position, float radius):
shape2d(SD2_CIRCLE)
{
this->position=position;
this->radius=radius;
}

circle2d::circle2d(const circle2d& c):
shape2d(SD2_CIRCLE)
{
*this=c;
}

circle2d& circle2d::operator=(const circle2d& c)
{
this->position=c.position;
this->radius=c.radius;
return *this;
}

circle2d:: ~circle2d()
{
}

 float circle2d::getArea()
{
return GPP_PI*(radius*radius);
}

 vector3d circle2d::getCenter()
{
return this->position;
}

BoundingBox2d circle2d::getBoundingBox()const
{
return BoundingBox2d({position.x-radius, position.y-radius}, {position.x+radius, position.y-radius}, {position.x-radius, position.y+radius}, {position.x+radius, position.y+radius});
}

void circle2d::translate(const vector3d& v)
{
position+=v;
}

void circle2d::rotate(float angle, const vector3d& origin)
{
gpp_rotate_single_point(position, angle, origin);
}

bool circle2d::isColiding(shape2d* ch)
{
Shape2dConverter df;
return df(this, ch);
}
}
