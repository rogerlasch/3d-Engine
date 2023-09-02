

#include<algorithm>
#include<sstream>
#include"geometry.h"

using namespace std;

namespace gpp
{
AABB::AABB(const vector3d& min, const vector3d& max, GeometricShape* sh)
{
this->shape=NULL;
this->min=min;
this->max=max;
this->setGeometricShape(sh);
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
vector3d v(fabs(max.x-min.x), fabs(max.y-min.y), fabs(max.z-min.z));
return (v.x*v.y)*v.z;
}

void AABB::setGeometricShape(GeometricShape* sh)
{
if(this->shape!=NULL)
{
delete this->shape;
}
this->shape=sh;
this->recalculateBoundingBox();
}

GeometricShape* AABB::getGeometricShape()const
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
this->min+=v;
this->max+=v;
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

void AABB::rotate(const quaternion& orientation)
{
if(shape!=NULL)
{
shape->rotate(orientation);
recalculateBoundingBox();
}
}

void AABB::recalculateBoundingBox()
{
GeometricShape* sh=this->getGeometricShape();
if(sh==NULL)
{
return;
}
switch(sh->getGeometricType())
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
max=b->max;
break;
}
/*
case GTYPE_CAPSULE:
case GTYPE_CONE:
case GTYPE_POLYHEDRON:
{
break;
}
*/
}
}

string AABB::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Mostrando AABB..."<<endl;
vector<string> achis={"X", "Y", "Z"};
for(uint32 i=0; i<3; i++)
{
ss<<safe_format("{}: {:.2f}:{:.2f}", achis[i], min[i], max[i])<<endl;
}
if(shape==NULL)
{
ss<<"Forma geométrica: NULL"<<endl;
}
else
{
ss<<"Forma geométrica: "<<shape->toString();
}
return ss.str();
}
}
