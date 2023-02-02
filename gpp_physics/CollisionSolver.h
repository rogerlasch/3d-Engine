

#ifndef COLLISIONSOLVER_H
#define COLLISIONSOLVER_H


namespace gpp
{

class CollisionSolver
{
public:
void solve(CollisionPairList& pairs);
void solve(CollisionPairList& pairs, std::shared_mutex& m);
};
}
#endif
