

/**
*Uma implementação básica de uma octhree.
**/
#ifndef OCTREE_H
#define OCTREE_H

#include<functional>
#include<unordered_map>
#include"octreeinfo.h"
#include"octreenode.h"

namespace gpp
{


class CollisionInfo;
        typedef std::function<bool(octreenode*)> OCTREEVISITORCALLBACK;
class octree
{
private:
octreeinfo info;
octreenode* root;
std::unordered_map<uint64, octreenode*> nodes;
public:
octree();
octree(const octree& oc)=delete;
octree& operator=(const octree& oc)=delete;
virtual ~octree();
std::string toString()const;
octreeinfo getInfo()const;

void create(const vector3d& center, decimal radius);
void traverse(OCTREEVISITORCALLBACK hvisitor, uint64 start_id=0)const;

void insert(RigidBody* rb);
void remove(RigidBody* rb);
void transiction(RigidBody* rb);
void broadPhase(std::vector<CollisionInfo*>& collisions);
void rayCast(const vector3d& origin, const vector3d& dir, std::vector<RayInfo>& infos);
private:
void broadPhaseAuxiliary(const std::unordered_map<uint64, RigidBody*>::iterator& begin_loop, uint64 start_node, std::vector<CollisionInfo*>& collisions);
void getDeepest(RigidBody* rb, std::vector<octreenode*>& hnodes, uint64 start_id);
void redistributeObjects(octreenode* hnode);
octreenode* getNode(uint64 hkey, uint32 index);
void insertNode(octreenode* hnode);
void insertBodyAtNode(octreenode* node, RigidBody* rb);
friend class octreenode;
};
}
#endif
