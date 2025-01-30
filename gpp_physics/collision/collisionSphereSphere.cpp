

#include"collision.h"

using namespace std;

namespace gpp{

bool collisionSphereSphere(Sphere3d* s1, Sphere3d* s2, CollisionInfo* info){
vector3d dir=s1->position-s2->position;
decimal sqdist=dir*dir;
decimal radius=s1->radius+s2->radius;

if(sqdist>(radius*radius)){
return false;
}

if(info!=NULL){
info->normal=dir.normalize();
info->point=s2->position+(info->normal*s2->radius);
info->depth=radius-sqrtf(sqdist);
}

return true;
}
}
