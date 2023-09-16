

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
}
return false;
}
}
