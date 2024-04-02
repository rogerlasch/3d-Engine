

#include"collision.h"

using namespace std;

namespace gpp{
bool collisionBoxSphere(Box3d* b, Sphere3d* s, CollisionInfo* info){
profiler_snap();
vector3d closestPoint=b->getClosestPoint(s->position);
vector3d d=closestPoint-s->position;

decimal dist=vector3d::dot(d, d);
decimal sqRadius=s->radius*s->radius;

if(dist>sqRadius){
return false;
}

if(dist<=0.001f){
vector3d origin;
vector3d rdir;
vector3d v1=s->position-s->lastPosition;
vector3d v2=b->position-b->lastPosition;
float d1=v1*v1;
float d2=v2*v2;
if((d1<=0.0f)&&(d2<=0.0f)){
rdir=(s->position-quaternion_vector_rotate(s->orientation, {0, -1, 0})).normalize();
origin=s->position;
}
else if(d1>0.0f){
origin=s->position;
rdir=v1.normalize();
}
else{
origin=b->position;
rdir=v2.normalize().inverse();
}
RayInfo rinfo;
_GASSERT(rayOBB(origin, rdir, b->position, b->alf, b->axis, &rinfo));
v1=origin-rinfo.ePoint;
d1=v1*v1;

if(info!=NULL){
info->normal=(rdir.inverse())+numeric_limits<decimal>::epsilon();
info->point=rinfo.ePoint;
info->depth=fabs(rinfo.eDist)+s->radius;
}

return true;
}
else{
if(info!=NULL){
info->normal=((d.normalize()).inverse())+numeric_limits<decimal>::epsilon();
info->point=closestPoint;
info->depth=s->radius-sqrt(dist);
}
return true;
}
return false;
}
}
