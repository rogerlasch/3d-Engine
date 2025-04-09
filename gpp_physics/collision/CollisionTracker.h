
#ifndef COLLISIONTRACKER_H
#define COLLISIONTRACKER_H

#include<tbb/parallel_for_each.h>
#include<tbb/concurrent_vector.h>
#include<tbb/concurrent_unordered_map.h>
#include<tbb/concurrent_unordered_set.h>

namespace gpp {

class CollisionTracker {
private:
    tbb::concurrent_unordered_set<uint64> lastCollisionFrame;
    tbb::concurrent_unordered_map<uint64, CollisionInfo> activeCollisions;

public:
    CollisionTracker();
    CollisionTracker(const CollisionTracker& ct) = delete;
    CollisionTracker& operator=(const CollisionTracker& ct) = delete;

    void cleanup();
    void narrowPhase(const tbb::concurrent_vector<std::pair<RigidBody*, RigidBody*>>& newCollisions);
    void resolveCollisions();
};
} // namespace gpp
#endif // COLLISIONTRACKER_H
