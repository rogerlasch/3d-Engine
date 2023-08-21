

#include"geometry.h"
#include"aabbutil.h"

using namespace std;

namespace gpp
{

bool aabbOverlap(const vector3d& v1, float r1, const vector3d& v2, float r2)
{
vector3d m1=(v1-r1);
vector3d m2=(v1+r1);
vector3d m3=(v2-r2);
vector3d m4=(v2+r2);
if((m2.x<m3.x)||(m1.x>m4.x)||(m2.y<m3.y)||(m1.y>m4.y)||(m2.z<m3.z)||(m1.z>m4.z))
{
return false;
}
return true;
}

bool aabbOverlap(const vector3d& center, float radius, AABB* b)
{
AABB a((center-radius), (center+radius));
if((a.max.x<b->min.x)||(a.min.x>b->max.x)||(a.max.y<b->min.y)||(a.min.y>b->max.y)||(a.max.z<b->min.z)||(a.min.z>b->max.z))
{
return false;
}
return true;
}

bool aabbOverlap(AABB* a, AABB* b)
{
_GASSERT(a!=NULL);
_GASSERT(b!=NULL);
if((a->max.x<b->min.x)||(a->min.x>b->max.x)||(a->max.y<b->min.y)||(a->min.y>b->max.y)||(a->max.z<b->min.z)||(a->min.z>b->max.z))
{
return false;
}
return true;
}

bool aabbInsideAll(const vector3d& v1, float r1, const vector3d& v2, float r2)
{
if(r2>r1)
{
return false;
}
vector3d m1=(v1-r1);
vector3d m2=(v1+r1);
vector3d m3=(v2-r2);
vector3d m4=(v2+r2);
if((m1.x<=m3.x)&&(v2.x>=m4.x)&&(m1.y<=m3.y)&&(v2.y>=m4.y)&&(m1.z<=m3.z)&&(v2.z>=m4.z))
{
return true;
}
return false;
}

bool aabbInsideAll(const vector3d& center, float radius, AABB* b)
{
_GASSERT(b!=NULL);
AABB a((center-radius), (center+radius));
if((a.min.x<=b->min.x)&&(a.max.x>=b->max.x)&&(a.min.y<=b->min.y)&&(a.max.y>=b->max.y)&&(a.min.z<=b->min.z)&&(a.max.z>=b->max.z))
{
return true;
}
return false;
}

bool aabbInsideAll(AABB* a, AABB* b)
{
_GASSERT(a!=NULL);
_GASSERT(b!=NULL);
if((a->min.x<=b->min.x)&&(a->max.x>=b->max.x)&&(a->min.y<=b->min.y)&&(a->max.y>=b->max.y)&&(a->min.z<=b->min.z)&&(a->max.z>=b->max.z))
{
return true;
}
return false;
}

bool aabbInsidePoint(AABB* a, const vector3d& v)
{
_GASSERT(a!=NULL);
if((a->min.x<=v.x)&&(a->max.x>=v.x)&&(a->min.y<=v.y)&&(a->max.y>=v.y)&&(a->min.z<=v.z)&&(a->max.z>=v.z))
{
return true;
}
return false;
}

vector3d get_closest_point_from_aabb(const vector3d& pt, const vector3d& min, const vector3d& max)
{
vector3d closestPoint;
                       for (int i = 0; i < 3; i++) {
                             float v = pt[i];
                             if (v < min[i]) v = min[i]; // v = max(v, min[i])
                             if (v > max[i]) v = max[i]; // v = min(v, max[i])
closestPoint[i] = v;
                       }
return closestPoint;
}

bool point_in_aabb(const vector3d& pt, const vector3d& min, const vector3d& max)
{
for(uint32 i=0; i<3; i++)
{
if((pt[i]<min[i])||(pt[i]>max[i])) return false;
}
return true;
}
}
