#ifndef ICOLLISIONSOLVER_H
#define ICOLLISIONSOLVER_H

namespace gpp
{
    class iCollisionSolver
    {
    public:
        iCollisionSolver() = default;
        virtual ~iCollisionSolver() = default;
virtual void solve(CollisionCache* hcache)=0;
virtual bool solvePair(CollisionInfo* info)=0;
    };
} // namespace gpp

#endif
