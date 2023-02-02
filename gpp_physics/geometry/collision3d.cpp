
#include <limits>
#include<gpp/debug_system.h>
#include"geometry.h"
#include"collision3d.h"

using namespace std;

namespace gpp
{
bool collision3d::isColliding(gpp_shape* s1, gpp_shape* s2)
{
if((s1==NULL)||(s2==NULL))
{
return false;
}
switch((s1->getGType()|s2->getGType()))
{
case GTYPE_SPHERE:
return sphereSphere((sphere3d*)s1, (sphere3d*)s2);
case GTYPE_SPHERE|GTYPE_BOX:
{
return sphereBox(((s1->gtype==GTYPE_SPHERE) ? (sphere3d*)s1 : (sphere3d*)s2), ((s1->gtype==GTYPE_BOX) ? (box3d*)s1 : (box3d*)s2));
}
case GTYPE_SPHERE|GTYPE_LINE:
return sphereLine((sphere3d*)s1, (line3d*)s2);
case GTYPE_BOX:
return boxBox((box3d*)s1, (box3d*)s2);
case GTYPE_BOX|GTYPE_LINE:
return boxLine((box3d*)s1, (line3d*)s2);
case GTYPE_LINE:
return lineLine((line3d*)s1, (line3d*)s2);
}
return false;
}

bool collision3d::isColliding(gpp_shape* sh, const vector3d& v)
{
if(sh==NULL)
{
return false;
}
switch(sh->getGType())
{
case GTYPE_SPHERE:
return spherePoint((sphere3d*)sh, v);
case GTYPE_BOX:
return boxPoint((box3d*)sh, v);
case GTYPE_LINE:
return linePoint((line3d*)sh, v);
}
return false;
}

bool collision3d::sphereSphere(sphere3d* s1, sphere3d* s2)
{
float dist=vector3d::get_squared_distance(s1->center, s2->center);
float radius=s1->radius+s2->radius;
return dist<=(radius*radius);
}

bool collision3d::sphereLine(sphere3d* s, line3d* l)
{
float f=0.0f;
vector3d p;
ClosestPtPointSegment(s->center, l->min, l->max, f, p);
return vector3d::get_squared_distance(s->center, p)<=(s->radius*s->radius);
}

bool collision3d::sphereBox(sphere3d* s, box3d* b)
{
vector3d& min=b->min;
vector3d max=(b->min+b->measures);
float sqdist=0;
if(s->center.x<=min.x)
sqdist+=((min.x-s->center.x)*(min.x-s->center.x));
else if(s->center.x>max.x)
sqdist+=((s->center.x-max.x)*(s->center.x-max.x));

if(s->center.y<=min.y)
sqdist+=((min.y-s->center.y)*(min.y-s->center.y));
else if(s->center.y>max.y)
sqdist+=((s->center.y-max.y)*(s->center.y-max.y));

if(s->center.z<=min.z)
sqdist+=((min.z-s->center.z)*(min.z-s->center.z));
else if(s->center.z>max.z)
sqdist+=((s->center.z-max.z)*(s->center.z-max.z));
float sqradius=s->radius*s->radius;
return sqdist<=sqradius;
}

bool collision3d::boxBox(box3d* b1, box3d* b2)
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

bool collision3d::boxLine(box3d* b, line3d* l)
{
vector3d bmax=(b->min+b->measures);
           vector3d c = (b->min + bmax) * 0.5f; // Box center-point
           vector3d e = bmax - c;                // Box halflength extents
           vector3d m = (l->min + l->max) * 0.5f;          // Segment midpoint
           vector3d d = l->max - m;                   // Segment halflength vector
           m = m - c;                           // Translate box and segment to origin
           // Try world coordinate axes as separating axes
           float adx = abs(d.x);
           if (abs(m.x) > e.x + adx) return false;
           float ady = abs(d.y);
           if (abs(m.y) > e.y + ady) return false;
           float adz = abs(d.z);
           if (abs(m.z) > e.z + adz) return false;
           // Add in an epsilon term to counteract arithmetic errors when segment is
           // (near) parallel to a coordinate axis (see text for detail)
           adx += numeric_limits<float>::epsilon(); ady += numeric_limits<float>::epsilon(); adz += numeric_limits<float>::epsilon();
          // Try cross products of segment direction vector with coordinate axes
          if (abs(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady) return false;
          if (abs(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx) return false;
          if (abs(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx) return false;
          // No separating axis found; segment must be overlapping AABB
          return true;

}

bool collision3d::lineLine(line3d* l1, line3d* l2)
{
static float f1=0.0f, f2=0.0f;
static vector3d v1, v2;
return ClosestPtSegmentSegment(l1->min, l1->max, l2->min, l2->max, f1, f2, v1, v2)<=0.01f;
}

//Auxiliary, don't use it!
bool collision3d::spherePoint(sphere3d* s, const vector3d& v)
{
float dist=vector3d::get_squared_distance(s->center, v);
return dist<=(s->radius*s->radius);
}

bool collision3d::boxPoint(box3d* b, const vector3d& v)
{
vector3d max=b->min+b->measures;
if((b->min.x<=v.x)&&(max.x>=v.x)&&(b->min.y<=v.y)&&(max.y>=v.y)&&(b->min.z<=v.z)&&(max.z>=v.z))
{
return true;
}
return false;
}

bool collision3d::linePoint(line3d* l, const vector3d& v)
{
float f=0.0f;
vector3d p;
ClosestPtPointSegment(v, l->min, l->max, f, p);
return vector3d::get_squared_distance(v, p)<=0.01f;
}
}
