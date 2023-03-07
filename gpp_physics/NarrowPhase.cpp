
#include"gpp_physics.h"
#include"NarrowPhase.h"

using namespace std;

namespace gpp
{
void NarrowPhase::scan(CollisionPairList& pairs, CollisionList& collisions)
{
for(auto& it : pairs)
{
CollisionInfo info;
if(collision3d::isColliding(it.first->getGeometricShape(), it.second->getGeometricShape(), &info))
{
info.b1=it.first;
info.b2=it.second;
collisions.push_back(info);
}
}
}

void NarrowPhase::scan(CollisionPairList& pairs, CollisionList& collisions, shared_mutex& m)
{
unique_lock<shared_mutex> lck(m, defer_lock);
while(!lck.try_lock())
{
}
this->scan(pairs, collisions);
}

}
