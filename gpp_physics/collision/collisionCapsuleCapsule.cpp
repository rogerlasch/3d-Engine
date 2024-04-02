

#include"collision.h"

using namespace std;

namespace gpp{

bool collisionCapsuleCapsule(Capsule3d* cs1, Capsule3d* cs2, CollisionInfo* info){
profiler_snap();
static float s=0.0f, t=0.0f;
float radius=cs1->radius+cs2->radius;
vector3d p1, p2, q1, q2, c1, c2;
cs1->getSegment(p1, p2);
cs2->getSegment(q1, q2);
float dist=ClosestPtSegmentSegment(p1, p2, q1, q2, s, t, c1, c2);
if(dist>(radius*radius)){
return false;
}

if(info!=NULL){
info->normal=(c1-c2).normalize();
info->point=c2+(info->normal*cs2->radius);
info->depth=radius-sqrtf(dist);
}

return true;
}
}
