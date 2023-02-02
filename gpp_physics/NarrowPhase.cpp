
#include"gpp_physics.h"
#include"NarrowPhase.h"

using namespace std;

namespace gpp
{
void NarrowPhase::scan(const CollisionPairList& pairs, CollisionPairList& collisions)
{
collisions.reserve(pairs.size());
for(auto& it : pairs)
{
gpp_shape* s1=it.first->getShape();
gpp_shape* s2=it.second->getShape();
if(collision3d::isColliding(s1, s2))
{
collisions.push_back(it);
}
}
}

void NarrowPhase::scan(const CollisionPairList& pairs, CollisionPairList& collisions, shared_mutex& m)
{
unique_lock<shared_mutex> lck(m, defer_lock);
while(!lck.try_lock())
{
}
this->scan(pairs, collisions);
}

}
