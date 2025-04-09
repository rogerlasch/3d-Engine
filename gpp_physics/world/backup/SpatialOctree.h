
#ifndef GPP_SPATIALOCTREE_H
#define GPP_SPATIALOCTREE_H

#include <vector>
#include <unordered_map>
#include <string>

namespace gpp {

class SpatialOctree {
public:

    struct OctreeNode {
uint32 depth=0;
        AABB bounds;
        std::vector<RigidBody*> bodies;
        OctreeNode* hparent;
        std::vector<OctreeNode*> childs;

        bool isLeaph() const {
            return childs.empty();
        }

        std::string toString() const {
std::stringstream ss;
ss<<std::fixed;
ss.precision(2);
ss<<"Bodies="<<bodies.size()<<", Depth="<<depth<<", IsLeaph="<<std::boolalpha<<isLeaph()<<", Bounds="<<bounds.getShortDescription()<<std::endl;
for(auto& it : childs){
ss<<it->toString();
}

return ss.str();
        }
    };

private:
    uint32 treshold;
    uint32 maxDepth;
    OctreeNode* root;
    std::vector<std::pair<RigidBody*, OctreeNode*>> objectsNodeMap;
ProfilerManager* hprofiler;
public:
    SpatialOctree(const AABB& bounds, uint32 maxDepth=6, uint32 treshold = 32);
    SpatialOctree(const SpatialOctree& so) = delete;
    SpatialOctree& operator=(const SpatialOctree& so) = delete;
    ~SpatialOctree();

    std::string toString() const;
    void cleanup();

    void insert(RigidBody* rb);
    void remove(RigidBody* rb);
    void update(RigidBody* rb);
    void rayCast(const vector3d& origin, const vector3d& dir, std::vector<RayInfo>& infos);
    void broadPhase(std::vector<std::pair<RigidBody*, RigidBody*>>& pairs);

private:
    void clearNode(OctreeNode* hnode);
    bool insertInternal(RigidBody* rb, const AABB& ab, OctreeNode* hnode, uint32 depth);
    void removeInternal(RigidBody* rb, OctreeNode* hnode);
    void subDivide(OctreeNode* hnode);
    void redistribute(OctreeNode* hnode);
bool getNodeIndex(RigidBody* rb, uint32* index)const;

};

} // namespace gpp

#endif // GPP_SPATIALOCTREE_H
