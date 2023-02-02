

#ifndef BROADPHASE_H
#define BROADPHASE_H


namespace gpp
{

typedef std::pair<RigidBody*, RigidBody*> CollisionPair;
typedef std::vector<CollisionPair> CollisionPairList;
class BroadPhase
{
public:
void scan(const RigidBodyList& bodies, CollisionPairList& collisions);
void scan(const RigidBodyList& bodies, CollisionPairList& collisions, std::shared_mutex& m);
};
}
#endif
