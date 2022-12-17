


#include"gpp_world.h"
#include"CollisionSolver.h"

using namespace std;

namespace gpp
{
void CollisionSolver::solve( CollisionPairList& pairs)
{
for(auto& it : pairs)
{
vector3d v1=it.first->getVelocity();
vector3d v2=it.second->getVelocity();
if(vector3d::dotProduct(v1, v2)>=0)
{
v1/=6.35;
v2/=6.35;
v1*=-1;
v2*=-1;
it.first->setVelocity(v1);
it.second->setVelocity(v2);
}
v1=it.first->getForse();
v2=it.second->getForse();
if(vector3d::dotProduct(v1, v2)>=0)
{
v1*=-1;
v2*=-1;
it.first->setForse(v1);
it.second->setForse(v2);
}
}
}

void CollisionSolver::solve( CollisionPairList& pairs, shared_mutex& m)
{
unique_lock<shared_mutex> lck(m, defer_lock);
while(!lck.try_lock())
{
}
this->solve(pairs);
}
}
