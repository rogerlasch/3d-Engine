

#include"geometry.h"

using namespace std;

namespace gpp
{
BoundingSphere::BoundingSphere(const vector3d& center, float radius):
aabb(AABB_SPHERE)
{
this->center=center;
this->radius=radius;
}

BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::translate(const vector3d& v)
{
center+=v;
if(sh!=NULL)
{
sh->translate(v);
}
}

void BoundingSphere::scale(float sk)
{
this->radius*=sk;
if(sh!=NULL)
{
sh->scale(sk);
}
}

void BoundingSphere::recalculate()
{
}
}
