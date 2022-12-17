

#ifndef NARROWPHASE_H
#define NARROWPHASE_H


namespace gpp
{

class NarrowPhase
{
public:
void scan(const CollisionPairList& pairs, CollisionPairList& colisions);
void scan(const CollisionPairList& pairs, CollisionPairList& colisions, std::shared_mutex& m);
};
}
#endif
