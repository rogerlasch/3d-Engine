

#include"geometry.h"

using namespace std;

namespace gpp
{
aabb::aabb(uint32 type)
{
this->atype=type;
sh=NULL;
}

aabb::~aabb()
{
if(sh!=NULL)
{
delete sh;
}
sh=NULL;
}

uint32 aabb::getType()const
{
return this->atype;
}

bool aabb::isBoundingBox()const
{
return this->atype==AABB_BOX;
}

bool aabb::isBoundingSphere()const
{
return this->atype==AABB_SPHERE;
}

void aabb::setShape(shape* sh)
{
if(this->sh!=NULL)
{
delete this->sh;
}
this->sh=sh;
}

shape* aabb::getShape()const
{
return this->sh;
}

void aabb::translate(const vector3d& v)
{
}

void aabb::scale(float sk)
{
}

void aabb::recalculate()
{
}
}
