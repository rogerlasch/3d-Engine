
#include"geometry.h"
#include"colision3d.h"

using namespace std;

namespace gpp
{
 bool colision3d::isColiding(box3d* b1, box3d* b2)
{
vector3d m1((b1->min+b1->measures));
vector3d m2((b2->min+b2->measures));
vector3d v1=(b1->min-m2);
vector3d v2=(b2->min-m1);
if((v1.x>0.0f)||(v1.y>0.0f)||(v1.z>0.0f))
{
return false;
}
if((v2.x>0.0f)||(v2.y>0.0f)||(v2.z>0.0f))
{
return false;
}
return true;
}

 bool colision3d::isColiding(box3d* b, sphere3d* c)
{
vector3d v;
vector3d max=b->min+b->measures;
if(c->center.x<b->min.x)
{
v.x=b->min.x;
}
else if(c->center.x>max.x)
{
v.x=max.x;
}
if(c->center.y<b->min.y)
{
v.y=b->min.y;
}
else if(c->center.y>max.y)
{
v.y=max.y;
}
if(c->center.z<b->min.z)
{
v.z=b->min.z;
}
else if(c->center.z>max.z)
{
v.z=max.z;
}
float dist=vector3d::get_distance(c->center, v);
return dist<=c->radius;
}

 bool colision3d::isColiding(box3d* b, line3d* l)
{
return ColisionLine3d::isColiding(b, l);
}

 bool colision3d::isColiding(sphere3d* c1, sphere3d* c2)
{
vector3d v((c1->center-c2->center));
v.x=v.x*v.x;
v.y=v.y*v.y;
v.z=v.z*v.z;
float dist=(v.x+v.y)+v.z;
float radius=(c1->radius+c2->radius);
radius=radius*radius;
return dist<=radius;
}

 bool colision3d::isColiding(sphere3d* c, box3d* b)
{
return isColiding(b, c);
}

 bool colision3d::isColiding(sphere3d* c, line3d* l)
{
return ColisionLine3d::isColiding(c, l);
}

 bool colision3d::isColiding(line3d* l1, line3d* l2)
{
float f1=0.0f, f2=0.0f;
vector3d v1, v2;
return ClosestPtSegmentSegment(l1->min, l1->max, l2->min, l2->max, f1, f2, v1, v2)<=0.5f;
}

 bool colision3d::isColiding(line3d* l, box3d* b)
{
return ColisionLine3d::isColiding(b, l);
}

 bool colision3d::isColiding(line3d* l, sphere3d* c)
{
return ColisionLine3d::isColiding(c, l);
}
}
