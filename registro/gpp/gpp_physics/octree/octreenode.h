

#ifndef OCTREENODE_H
#define OCTREENODE_H

namespace gpp
{

class octreenode
{
public:
uint32 level;
float radius;
vector3d center;
octreenode* parent;
std::vector<octreenode*> childs;
std::vector<iRigidBody*> bodies;
octreenode();
octreenode(const octreenode& oc)=delete;
octreenode& operator=(const octreenode& oct)=delete;
virtual ~octreenode();
std::string toString()const;
};
}
#endif
