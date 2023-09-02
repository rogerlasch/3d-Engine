

/**
*Uma implementação básica de uma octhree.
*No momento ela não está sendo utilizada, mas talvez seja futuramente.
**/
#ifndef OCTREE_H
#define OCTREE_H

#include"octreeinfo.h"
#include"octreenode.h"

namespace gpp
{

class octree
{
private:
octreeinfo info;
octreenode* root;
public:
octree();
octree(const octree& oc)=delete;
octree& operator=(const octree& oc)=delete;
virtual ~octree();
std::string toString()const;
octreeinfo getInfo()const;
void create(const vector3d& min, uint32 max_depth, uint32 blimit, float alph);
void clear();
void insert(iRigidBody* rb);
void remove(iRigidBody* rb);
void BroadPhase(std::vector<iRigidBody*>& hbodies, std::vector<CollisionInfo>& collisions);
void broadPhase(iRigidBody* rb, std::vector<CollisionInfo>& hcollisions);
private:
octreenode* getDeepest(iRigidBody* rb, octreenode* start=NULL);
void splitAndRedistribute(octreenode* node);
void splitNode(octreenode* node);
void redistributeNode(octreenode* node);
void moveObject(octreenode* from, octreenode* to, iRigidBody* rb);
void extractObjectsNode(octreenode* node, std::vector<iRigidBody*>& hbodies);
void computeInfos();
};
}
#endif
