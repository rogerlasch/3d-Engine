
#include"../geometry/geometry.h"
#include "collision.h"
#include"../world/RigidBody.h"

using namespace std;

namespace gpp {

CollisionTracker::CollisionTracker() {}

void CollisionTracker::cleanup() {
_PROFILER_SNAP();
    lastCollisionFrame.clear();
    activeCollisions.clear();
}

void CollisionTracker::narrowPhase(const tbb::concurrent_vector<pair<RigidBody*, RigidBody*>>& newCollisions) {
_PROFILER_SNAP();
    tbb::concurrent_unordered_set<uint64> currentFrame;

if(!newCollisions.empty()){
tbb::parallel_for_each(newCollisions.begin(), newCollisions.end(), [&](const std::pair<RigidBody*, RigidBody*>& hpair){
        uint64 id = getCollisionId(reinterpret_cast<uint64>(hpair.first), reinterpret_cast<uint64>(hpair.second));

        CollisionInfo info;

        if (detectCollisionWith(hpair.first, hpair.second, &info)) {
            info.id = id;
info.type=COL_BEGIN;
            currentFrame.insert(id);

            if (lastCollisionFrame.find(id) == lastCollisionFrame.end()) {
                info.type = COL_BEGIN;
                activeCollisions.insert(make_pair(id, info));
            } else {
                auto entry = activeCollisions.find(id);
                if (entry != activeCollisions.end()) {
                    entry->second.update(&info);
//                    entry->second.type = COL_CONTINUOUS;
                }
            }
        }
    });
}

tbb::parallel_for_each(lastCollisionFrame.begin(), lastCollisionFrame.end(), [&](uint64 id){
        if (currentFrame.find(id) == currentFrame.end()) {
            auto entry = activeCollisions.find(id);
            if (entry != activeCollisions.end()) {
entry->second.lastType=entry->second.type;
                entry->second.type = COL_END;
            }
        }
    });

    lastCollisionFrame = std::move(currentFrame);
}

void CollisionTracker::resolveCollisions() {
_PROFILER_SNAP();
    tbb::concurrent_unordered_set<uint64> deleteCollisions;

    for (auto it = activeCollisions.begin(); it != activeCollisions.end(); ++it) {

        it->second.r1->executeCollisionCallback(it->second.type, it->second.r2, &it->second);
        it->second.r2->executeCollisionCallback(it->second.type, it->second.r1, &it->second);

        if (it->second.type != COL_END) {
            it->second.r1->resolveCollision(it->second.r2, &it->second);
        } else {
            deleteCollisions.insert(it->first);
        }
    }

    for (const auto& it : deleteCollisions) {
        activeCollisions.unsafe_erase(it);
    }
}
} // namespace gpp
