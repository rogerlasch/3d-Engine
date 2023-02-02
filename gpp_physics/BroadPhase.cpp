

#include"gpp_physics.h"
#include"BroadPhase.h"

using namespace std;

namespace gpp
{
void BroadPhase::scan(const RigidBodyList& bodies, CollisionPairList& collisions)
{
}

void BroadPhase::scan(const RigidBodyList& bodies, CollisionPairList& collisions, std::shared_mutex& m)
{
unique_lock<shared_mutex> lck(m, defer_lock);
while(!lck.try_lock())
{
}
this->scan(bodies, collisions);
}
}
