

#include"collision.h"

using namespace std;

namespace gpp{

bool collisionSphereCapsule(Sphere3d* s, Capsule3d* c, CollisionInfo* info){
profiler_snap();
static float t=0.0f;
float radius=s->radius+c->radius;
vector3d pt, a, b;
c->getSegment(a, b);
float dist=ClosestPtPointSegment(s->position, a, b, t, pt);

if(dist>(radius*radius)){
return false;
}

if(info!=NULL){
info->normal=(s->position-pt).normalize();
info->point=pt+(info->normal*c->radius);
info->depth=radius-sqrtf(dist);
}

return true;
}
}
