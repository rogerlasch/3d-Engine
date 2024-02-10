

/**
*Uma implementação básica de uma octhree.
**/
#ifndef OCTREE_H
#define OCTREE_H

#include"octreeinfo.h"
#include"octreenode.h"

namespace gpp
{


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

void create(const vector3d& center, float radius);
void traverse(OCTREEVISITORCALLBACK hvisitor);

void insert(RigidBody* rb);
void remove(RigidBody* rb);

private:
void createChilds(octreenode* parent, uint8 pchilds);
void getDeepest(RigidBody* rb, std::vector<octreenode*>& hnodes);

};
}
#endif
