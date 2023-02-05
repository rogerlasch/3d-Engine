
#include"gpp_physics.h"
#include"NarrowPhase.h"

using namespace std;

namespace gpp
{
void NarrowPhase::scan(const CollisionPairList& pairs, CollisionPairList& collisions)
{
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
