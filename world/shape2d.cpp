

#include"geometry.h"

using namespace std;

namespace gpp
{
shape2d::shape2d(int gtype):
shape(gtype)
{
}

shape2d::~shape2d()
{
}

float shape2d::getArea()
{
return 0.0f;
}

float shape2d::getVolume()
{
return 0.0f;
}

float shape2d::getSurfaceArea()
{
return 0.0f;
}

 vector3d shape2d::getCenter()
{
return vector3d();
}

BoundingBox2d shape2d::getBoundingBox()const
{
return BoundingBox2d();
}

 bool shape2d::is2d()const
{
return true;
}

 bool shape2d::is3d()const
{
return false;
}

 void shape2d::translate(const vector3d& v)
{
}

 void shape2d::rotate(float angle, const vector3d& origin)
{
}

bool shape2d::isColiding(shape2d* ch)
{
return false;
}
}
