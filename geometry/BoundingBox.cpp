

#include"geometry.h"

using namespace std;

namespace gpp
{
BoundingBox::BoundingBox(const vector3d& min, const vector3d& max):
aabb(AABB_BOX)
{
this->min=min;
this->max=max;
}

BoundingBox::~BoundingBox()
{
}

void BoundingBox::translate(const vector3d& v)
{
min+=v;
max+=v;
if(sh!=NULL)
{
sh->translate(v);
}
}

void BoundingBox::scale(float sk)
{
max*=sk;
if(sh!=NULL)
{
sh->scale(sk);
}
}

void BoundingBox::recalculate()
{
}
}
