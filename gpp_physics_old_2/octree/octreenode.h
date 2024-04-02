

#ifndef OCTREENODE_H
#define OCTREENODE_H

namespace gpp
{

class octreenode
{
public:
uint8 pchilds;
uint32 level;
uint64 id;
float radius;
vector3d center;
std::vector<RigidBody*> bodies;
octreenode();
octreenode(const octreenode& oc)=delete;
octreenode& operator=(const octreenode& oct)=delete;
virtual ~octreenode();
std::string toString()const;
};
}
#endif
