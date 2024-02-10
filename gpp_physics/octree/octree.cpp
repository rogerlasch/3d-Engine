
#include<stack>
#include<algorithm>
#include<functional>
#include<string.h>
#include<sstream>
#include<libmorton/morton.h>
#include<gpp/debug_system.h>
#include"../types.h"
#include"../math/math.h"
#include"../RigidBody.h"
#include"../CollisionInfo.h"
#include"octree.h"

using namespace std;
using namespace libmorton;

namespace gpp
{

#define _encodeMortonKey(VX) morton3D_64_encode(static_cast<uint32>(VX.x), static_cast<uint32>(VX.y), static_cast<uint32>(VX.z))

octree::octree()
{
info.cleanup();
root=NULL;
}

octree::~octree()
{
info.cleanup();
root=NULL;
for(auto& it : nodes){
delete it.second;
it.second=NULL;
}
nodes.clear();
}

string octree::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<info.toString();
ss<<"Limites: ";
vector3d m1=root->center-root->radius;
vector3d m2=root->center+root->radius;
for(uint32 i=0; i<3; i++)
{
ss<<m1[i]<<":"<<m2[i]<<", ";
}
//ss << root->toString();
return ss.str();
}

void octree::create(const vector3d& center, float radius){
profiler_snap();
_GASSERT_MSG(root==NULL, "Já existe uma octree válida aqui. Limpe antes de chamar create novamente.");
info.nbodies=64;
octreenode* node=new octreenode;
node->id=_encodeMortonKey(center);
node->center=center;
node->radius=radius;
node->level=0;
node->pchilds=0;
root=node;
nodes.insert(make_pair(node->id, node));
}

void octree::traverse(OCTREEVISITORCALLBACK hvisitor){
profiler_snap();
if((!hvisitor)||(nodes.size()==0)) return;
stack<uint64> pstack;
pstack.push(root->id);
while(!pstack.empty()){
uint64 id=pstack.top();
pstack.pop();
auto it=nodes.find(id);
_GASSERT(it!=nodes.end());
if (!hvisitor(it->second)) continue;
if(it->second->pchilds>0){
for(uint32 i=0; i<8; i++){
if((it->second->pchilds&(1<<i))){
pstack.push((it->second->id<<3)+i);
}
}
}
}
}

void octree::createChilds(octreenode* parent, uint8 pchilds){
profiler_snap();
float step=parent->radius*0.5f;
uint32 level=parent->level+1;
for(uint32 i=0; i<8; i++){
if((pchilds&(1<<i))==0) continue;
if((parent->pchilds&(1<<i))==0){
octreenode* sf = new octreenode();
            sf->center.x = ((i & 1) ? step : -step);
            sf->center.y = ((i & 2) ? step : -step);
            sf->center.z = ((i & 4) ? step : -step);
            sf->center +=parent->center;
            sf->radius = step;
sf->id=(parent->id<<3)+i;
sf->pchilds=0;
sf->level=level;
parent->pchilds|=1<<i;
}
}
}

void octree::getDeepest(RigidBody* rb, std::vector<octreenode*>& hnodes) {
    vector3d m1, m2;
    rb->getAabb(m1, m2);
    hnodes.clear();
    OCTREEVISITORCALLBACK hvisitor = [&](octreenode* hnode)->bool {
        if (aabbInsideAll(hnode->center, hnode->radius, m1, m2) {
            hnodes.push_back(hnode);
            return true;
        }
        return false;
        };
    traverse(hvisitor);
}
}
