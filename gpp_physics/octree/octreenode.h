

#ifndef OCTREENODE_H
#define OCTREENODE_H

namespace gpp
{

class octree;
class octreenode
{
public:
uint8 pchilds;
uint32 level;
uint64 id;
uint64 parent;
decimal radius;
vector3d center;
std::unordered_map<uint64, RigidBody*> hbodies;
octree* oct;
octreenode();
octreenode(const octreenode& oc)=delete;
octreenode& operator=(const octreenode& oct)=delete;
virtual ~octreenode();
std::string toString()const;
void setOctree(octree* oct);
octree* getOctree()const;
void createChilds();
};
}
#endif
