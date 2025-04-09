
#ifndef GPP_SPATIALOCTREE_H
#define GPP_SPATIALOCTREE_H

#include <string>
#include <tbb/concurrent_unordered_map.h>
#include<tbb/concurrent_set.h>
#include<tbb/concurrent_unordered_set.h>
#include<tbb/concurrent_vector.h>
#include<tbb/concurrent_priority_queue.h>
#include<tbb/parallel_for_each.h>


namespace gpp {

class SpatialOctree {
public:

    struct OctreeNode {
uint32 depth=0;
        AABB bounds;
tbb::concurrent_set<RigidBody*> bodies;
        OctreeNode* hparent=NULL;
tbb::concurrent_vector<OctreeNode*> childs;

        bool isLeaf() const {
            return childs.empty();
        }

        std::string toString() const {
std::stringstream ss;
ss<<std::fixed;
ss.precision(2);
ss<<"Bodies="<<bodies.size()<<", Depth="<<depth<<", IsLeaph="<<std::boolalpha<<isLeaf()<<", Bounds="<<bounds.getShortDescription()<<std::endl;
for(auto& it : childs){
ss<<it->toString();
}

return ss.str();
        }
    };

private:
    uint32 threshold;
    uint32 maxDepth;
    OctreeNode* root;
    tbb::concurrent_unordered_map<RigidBody*, OctreeNode*> objectsNodeMap;
tbb::concurrent_vector<std::pair<RigidBody*, OctreeNode*>> removeObjectsNode;

ProfilerManager* hprofiler;
public:
    SpatialOctree(const AABB& bounds, uint32 maxDepth=6, uint32 threshold = 32);
    SpatialOctree(const SpatialOctree& so) = delete;
    SpatialOctree& operator=(const SpatialOctree& so) = delete;
    ~SpatialOctree();

    std::string toString() const;
    void cleanup();

    void insert(RigidBody* rb);
    void remove(RigidBody* rb);
void removePendingObjects();
    void update(RigidBody* rb);
    void rayCast(const vector3d& origin, const vector3d& dir, tbb::concurrent_vector<RayInfo>& infos);
    void broadPhase(tbb::concurrent_vector<std::pair<RigidBody*, RigidBody*>>& pairs);

private:
    void clearNode(OctreeNode* hnode);
    bool insertInternal(RigidBody* rb, const AABB& ab, OctreeNode* hnode, uint32 depth);
    void removeInternal(RigidBody* rb, OctreeNode* hnode);
    void subDivide(OctreeNode* hnode);
    void redistribute(OctreeNode* hnode);
};
} // namespace gpp
#endif // GPP_SPATIALOCTREE_H
