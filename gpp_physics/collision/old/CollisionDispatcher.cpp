

#include"collision.h"
#include"collisionBoxSphere.h"
#include"collisionSphereCapsule.h"
#include"collisionSphereSphere.h"
#include"collisionCapsuleCapsule.h"
#include"GJK.h"

using namespace std;

namespace gpp{
bool CollisionDispatcher::dispatch(GeometricShape* s1, GeometricShape* s2, CollisionInfo* info){
uint32 gtype=s1->getType()|s2->getType();
switch(gtype){
case GTYPE_SPHERE:{
return collisionSphereSphere((Sphere3d*)s1, (Sphere3d*)s2, info);
};
case GTYPE_BOX|GTYPE_SPHERE:{
Box3d* b=((s1->getType()==GTYPE_BOX) ? (Box3d*)s1 : (Box3d*)s2);
Sphere3d* s=((s1->getType()==GTYPE_SPHERE) ? (Sphere3d*)s1 : (Sphere3d*)s2);
return collisionBoxSphere(b, s, info);
}
case GTYPE_CAPSULE|GTYPE_SPHERE:{
Capsule3d* c=((s1->getType()==GTYPE_CAPSULE) ? (Capsule3d*)s1 : (Capsule3d*)s2);
Sphere3d* s=((s1->getType()==GTYPE_SPHERE) ? (Sphere3d*)s1 : (Sphere3d*)s2);
return collisionSphereCapsule(s, c, info);
}
case GTYPE_CAPSULE:{
Capsule3d* cs1=(Capsule3d*)s1;
Capsule3d* cs2=(Capsule3d*)s2;
return collisionCapsuleCapsule(cs1, cs2, info);
}
}
return false;
}

bool CollisionDispatcher::rayCast(const vector3d& origin, const vector3d& dir, GeometricShape* sh, RayInfo* info){
_GASSERT(sh!=NULL);
switch(sh->getType()){
case     GTYPE_SPHERE:{
Sphere3d* s=reinterpret_cast<Sphere3d*>(sh);
_GASSERT(s!=NULL);
return raySphere(origin, dir, s->position, s->radius, info);
}
case     GTYPE_BOX:{
Box3d* b=reinterpret_cast<Box3d*>(sh);
_GASSERT(b!=NULL);
return rayOBB(origin, dir, b->position, b->alf, b->axis, info);
}
case     GTYPE_CAPSULE:{
Capsule3d* c=reinterpret_cast<Capsule3d*>(sh);
_GASSERT(c!=NULL);
return rayCapsule(origin, dir, c->position, c->axis, c->length, c->radius, info);
}
}
return false;
}
}
