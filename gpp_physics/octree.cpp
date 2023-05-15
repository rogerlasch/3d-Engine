

#include<map>
#include<string.h>
#include"gpp_physics.h"
#include"octree.h"

using namespace std;

namespace gpp
{

octree_node::octree_node()
{
dept=0;
parent=NULL;
childs.clear();
bodies.clear();
}

octree_node::~octree_node()
{
for(auto& it : childs)
{
delete it;
}
dept=0;
bodies.clear();
parent=NULL;
}

string octree_node::toString()const
{
vector3d min=(center-radius);
vector3d max=(center+radius);
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Corpos neste nível: "<<bodies.size()<<endl;
ss<<min.x<<":"<<max.x<<", "<<min.y<<":"<<max.y<<", "<<min.z<<":"<<max.z<<endl;
for(auto& it : childs)
{
ss<<it->toString()<<endl;
}
return ss.str();
}

//The octree class...
octree::octree()
{
info.dept=0;
info.nodes=0;
info.bodies=0;
info.blimit=64;
root=NULL;
}

octree::~octree()
{
this->clear();
}

uint32 octree::getDept()const
{
return this->info.dept;
}

uint32 octree::count()const
{
return info.nodes;
}

uint32 octree::countBodies()const
{
return this->info.bodies;
}

uint32 octree::countMemoryUsage()const
{
uint32 x=sizeof(octree_info);
x+=sizeof(root);
x+=sizeof(octree_node)*info.nodes;
return x;
}

string octree::toString()
{
stringstream ss;
ss<<"Imprimindo octree..."<<endl;
ss<<"Número de nós: "<<info.nodes<<endl;
ss<<"Profundidade: "<<info.dept<<endl;
ss<<"Memória usada: "<<countMemoryUsage()<<endl;
ss<<"Imprimindo árvore..."<<endl;
//ss<<root->toString()<<endl;
map<uint32, uint32> depts;
print(root, depts);
ss<<"Imprimindo número de corpos por nível:"<<endl;
for(auto& it : depts)
{
ss<<it.first<<":"<<it.second<<endl;
}
return ss.str();
}

bool octree::empty()const
{
return root==NULL;
}

void octree::create(const vector3d& min, float size)
{
profiler_snap();
if(root!=NULL)
{
return;
}
info.nodes++;
root=new octree_node();
float radius=size*0.5f;
root->center=(min+radius);
root->radius=radius;
root->dept=0;
info.min=min;
info.max=(min+size);
}

void octree::clear()
{
if(root!=NULL)
{
delete root;
}
root=NULL;
memset(&info, 0, sizeof(info));
}

void octree::reserve(uint32 dept)
{
profiler_snap();
if((info.dept>0)||(root==NULL))
{
return;
}
spliteNode(root);
for(auto& it : root->childs)
{
vector<uint32> stack;
uint32 index=0;
octree_node* sf=it;
stack.push_back(0);
while(stack.size()>0)
{
spliteNode(sf);
for(auto& it : sf->childs)
{
if(sf->dept>=dept)
{
sf=sf->parent;
stack.erase(stack.begin()+index);
index--;
if(stack[index]<7)
{
stack[index]+=1;
sf=sf->childs[stack[index]];
}
break;
}
else
{
stack.push_back(0);
index++;
sf=it;
}
}
}
}
}

void octree::insert(iRigidBody* rb)
{
_GASSERT(rb!=NULL);
_GASSERT(root!=NULL);
profiler_snap();
octree_node* sf=root;
bool done=false;
while(done==false)
{
bool inside=aabbInsideAll(sf->center, sf->radius, rb->aabb);
if((inside)&&(sf->childs.size()==0))
{
if(sf->bodies.size()>=info.blimit)
{
spliteNode(sf);
redistribute(sf);
continue;
}
else
{
BinaryUtils::insert(sf->bodies, rb);
done=true;
break;
}
}
else if((inside)&&(sf->childs.size()>0))
{
bool found=false;
for(auto& it : sf->childs)
{
if(!aabbInsideAll(it->center, it->radius, rb->aabb))
{
continue;
}
found=true;
sf=it;
break;
}
if(!found)
{
BinaryUtils::insert(sf->bodies, rb);
done=true;
break;
}
}
else
{
BinaryUtils::insert(root->bodies, rb);
done=true;
break;
}
}
}

void octree::spliteNode(octree_node* node)
{
profiler_snap();
if(node->childs.size()>0)
{
return;
}
float step=node->radius*0.5f;
for(uint32 i=0; i<8; i++)
{
info.nodes++;
octree_node* sf=new octree_node();
                  sf->center.x = node->center.x+((i & 1) ? step : -step);
                  sf->center.y = node->center.y+((i & 2) ? step : -step);
                  sf->center.z = node->center.z+((i & 4) ? step : -step);
sf->radius=step;
sf->parent=node;
sf->dept=node->dept+1;
node->childs.push_back(sf);
if(info.dept<sf->dept)
{
info.dept=sf->dept;
}
}
}

void octree::redistribute(octree_node* node)
{
profiler_snap();
if(node->bodies.size()<info.blimit)
{
return;
}
if(node->childs.size()==0)
{
spliteNode(node);
}
for(uint32 i=0; i<node->bodies.size(); i++)
{
iRigidBody* rb=node->bodies[i];
for(auto& it2 : node->childs)
{
if(aabbInsideAll(it2->center, it2->radius, rb->aabb))
{
BinaryUtils::insert(it2->bodies, rb);
node->bodies.erase(node->bodies.begin()+i);
i--;
}
}
}
}

void octree::print(octree_node* node, map<uint32, uint32>& depts)
{
if(depts.find(node->dept)==depts.end())
{
depts[node->dept]=node->bodies.size();
}
else
{
auto it=depts.find(node->dept);
it->second+=node->bodies.size();
}
for(auto& it : node->childs)
{
print(it, depts);
}
}
}
