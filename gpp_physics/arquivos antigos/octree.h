

/**
*Uma implementação básica de uma octhree.
*No momento ela não está sendo utilizada, mas talvez seja futuramente.
**/
#ifndef OCTREE_H
#define OCTREE_H

namespace gpp
{

struct octree_info
{
uint32 nodes=0;
uint32 bodies=0;
uint32 dept=0;
uint32 blimit=0;
vector3d min;
vector3d max;
};

class octree_node
{
public:
vector3d center;
float radius;
uint32 dept;
octree_node* parent;
std::vector<octree_node*> childs;
std::vector<RigidBody*> bodies;
octree_node();
octree_node(const octree_node& qw)=delete;
octree_node& operator=(const octree_node& qw)=delete;
virtual ~octree_node();
std::string toString()const;
};

typedef std::vector<std::pair<RigidBody*, RigidBody*>> CollisionPairList;
class octree
{
private:
octree_info info;
octree_node* root;
public:
octree();
octree(const octree& qt)=delete;
octree& operator=(const octree& qt)=delete;
virtual ~octree();
uint32 getDept()const;
uint32 count()const;
uint32 countBodies()const;
uint32 countMemoryUsage()const;
std::string toString();
bool empty()const;
void create(const vector3d& min, float size);
void clear();
void reserve(uint32 dept);
void insert(RigidBody* rb);
private:
void spliteNode(octree_node* node);
void redistribute(octree_node* node);
void print(octree_node* node, std::map<uint32, uint32>& depts);
};
}
#endif
