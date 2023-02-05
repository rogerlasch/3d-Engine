

#ifndef COLLISIONSOLVER_H
#define COLLISIONSOLVER_H


namespace gpp
{

class CollisionSolver
{
public:
void solve(CollisionPairList& pairs);
void solve(RigidBody* r1, RigidBody* r2, CollisionData* data);
};
}
#endif
