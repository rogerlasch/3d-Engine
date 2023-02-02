

#include<algorithm>
#include<sstream>
#include"geometry.h"

using namespace std;

namespace gpp
{
AABB::AABB(const vector3d& min, const vector3d& max, gpp_shape* sh)
{
this->shape=NULL;
this->min=min;
this->max=max;
this->setShape(sh);
}

AABB::~AABB()
{
if(shape!=NULL)
{
delete shape;
}
shape=NULL;
}

float AABB::getVolume()const
{
vector3d v=(max-min);
return ((v.x*v.y)*v.z);
}

void AABB::setShape(uint32 gtype)
{
gpp_shape* sh=shape_create_from_type(gtype);
if(sh!=NULL)
{
this->setShape(sh);
}
}

void AABB::setShape(gpp_shape* sh)
{
if(this->shape!=NULL)
{
delete this->shape;
}
this->shape=sh;
this->recalculateBoundingBox();
}

gpp_shape* AABB::getShape()const
{
return this->shape;
}

void AABB::setMin(const vector3d& m)
{
this->min=m;
}

vector3d AABB::getMin()const
{
return this->min;
}

void AABB::setMax(const vector3d& m)
{
this->max=m;
}

vector3d AABB::getMax()const
{
return this->max;
}

void AABB::translate(const vector3d& v)
{
min+=v;
max+=v;
if(shape!=NULL)
{
shape->translate(v);
}
}

void AABB::scale(float sk)
{
max*=sk;
if(shape!=NULL)
{
shape->scale(sk);
}
}

void AABB::recalculateBoundingBox()
{
gpp_shape* sh=this->getShape();
if(sh==NULL)
{
return;
}
switch(sh->getGType())
{
case GTYPE_SPHERE:
{
sphere3d* s=(sphere3d*)sh;
this->min=(s->center-s->radius);
this->max=(s->center+s->radius);
break;
}
case GTYPE_BOX:
{
box3d* b=(box3d*)sh;
min=b->min;
max=(b->min+b->measures);
break;
}
case GTYPE_CAPSULE:
case GTYPE_CONE:
case GTYPE_POLYHEDRON:
{
break;
}
case GTYPE_LINE:
{
line3d* l=(line3d*)sh;
vector<vector3d> arr={l->min, l->max};
calculateBoundingBox(arr, *this);
break;
}
}
}

string AABB::toString()const
{
stringstream ss;
ss.precision(2);
ss<<fixed;
ss<<"AABB: "<<min.x<<":"<<max.x<<", "<<min.y<<":"<<max.y<<", "<<min.z<<":"<<max.z<<endl;
return ss.str();
}
}
