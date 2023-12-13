#ifndef INARROWPHASE_H
#define INARROWPHASE_H

namespace gpp
{
    class iNarrowPhase
    {
    public:
        iNarrowPhase() = default;
        virtual ~iNarrowPhase() = default;
virtual void detectCollisions(CollisionCache* cache)=0;
    };
} // namespace gpp

#endif
