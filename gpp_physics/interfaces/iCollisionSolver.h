#ifndef ICOLLISIONSOLVER_H
#define ICOLLISIONSOLVER_H

namespace gpp
{
    class iCollisionSolver
    {
    public:
        iCollisionSolver() = default;
        virtual ~iCollisionSolver() = default;
virtual void solve(std::vector<CollisionInfo>& collisions)=0;
virtual void solvePair(CollisionInfo& info)=0;
    };
} // namespace gpp

#endif
