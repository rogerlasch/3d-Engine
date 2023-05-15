#ifndef INARROWPHASE_H
#define INARROWPHASE_H

namespace gpp
{
    class iNarrowPhase
    {
    public:
        iNarrowPhase() = default;
        virtual ~iNarrowPhase() = default;
virtual void detectCollisions(std::vector<CollisionInfo>& iPhase, std::vector<CollisionInfo>& collisions)=0;
    };
} // namespace gpp

#endif
