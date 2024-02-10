

#include"../common.h"
#include"../gpp_physics.h"
#include"CollisionSolver.h"

using namespace std;

namespace gpp
{

void CollisionSolver::solve(CollisionCache* hcache)
{
unordered_set<uint64> hremove;
auto htable=hcache->getTable();
uint32 size=htable.size();
for(uint32 i=0; i<size; i++){
uint32 size1=htable[i]->size();
for(uint32 i1=0; i1<size1; i1++)
{
if(!solvePair(htable[i]->at(i1).get())){
hremove.insert(htable[i]->at(i1)->id);
}
}
}
if(hremove.size()>0){
hcache->removeByHashs(hremove);
}
}

bool CollisionSolver::solvePair(CollisionInfo* info)
{
iRigidBody* r1=info->r1;
iRigidBody* r2=info->r2;
if(r1->mass<=0.0f) swap(r1, r2);
//Caso r1 seja dinâmico e r2 seja estático...
if(r2->mass<=0.0f){
    float j = (1 + r1->restitution) * (r1->velocity* info->normal) / (1 / r1->mass);
    vector3d impulse = j * info->normal;
r1->velocity-=impulse/r1->mass;
if(info->depth>0.01f){
r1->translate(info->normal*info->depth);
}
return true;
}
else//Caso r1 e r2 sejam dinãmicos...
{
float eps=numeric_limits<float>::epsilon();
vector3d vr=(r1->velocity-r2->velocity)+eps;
_GINFO("vrn:{}", (info->normal*vr));
_GINFO("vr:{}\npt:{}", vr.toString(), info->point.toString());
//j=-(vr • n) * (e + 1) / [1/m1 + 1/m2]
float j=-(vr*info->normal) * (r1->restitution + 1) / ((1/r1->mass) + (1/r2->mass));
vector3d vimpulse=(j*info->normal);
vector3d v1=vimpulse/r1->mass;
vector3d v2=-vimpulse/r2->mass;
_GINFO("vp:{}\nv1:{}\nv2:{}", vimpulse.toString(), v1.toString(), v2.toString());
r1->applyVelocity(v1);
r2->applyVelocity(v2);
if(info->depth>0.01f){
vector3d tv=info->normal*(info->depth*0.5f);
tv+=numeric_limits<float>::epsilon();
r1->translate(tv, true);
r2->translate(-tv, true);
}
}
return false;
}
}
