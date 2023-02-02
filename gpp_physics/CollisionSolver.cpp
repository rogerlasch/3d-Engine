

#include"gpp_physics.h"
#include"CollisionSolver.h"

using namespace std;

namespace gpp
{
void CollisionSolver::solve( CollisionPairList& pairs)
{
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
