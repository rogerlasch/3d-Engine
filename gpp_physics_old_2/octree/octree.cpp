
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
info.max_depth=10;
info.blimit=32;
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
for(uint64 i=0; i<3; i++)
{
ss<<m1[i]<<":"<<m2[i]<<", ";
}
ss << root->toString();
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
for(uint64 i=0; i<8; i++){
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
if(pchilds==0) pchilds=255;
for(uint64 i=0; i<8; i++){
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
        if (aabbInsideAll(hnode->center, hnode->radius, m1, m2)) {
            hnodes.push_back(hnode);
            return true;
        }
        return false;
        };
    traverse(hvisitor);
}

void octree::insert(RigidBody* rb){
profiler_snap();
vector<octreenode*> hnodes;
getDeepest(rb, hnodes);

//Tentar adicionar o objeto no nó mais profundo...
octreenode* sf=hnodes.back();

if(sf->bodies.size()<info.blimit){
sf->bodies.push_back(rb);
return;
}

//Caso não possa adicionar no nó mais profundo, e não seja mais possível criar nós filhos...
//Então percorre os nós do mais profundo para o mais raso tentando encaixar.

if(sf->pchilds>0){
bool inserted=false;
for(uint64 i=hnodes.size(); i>0; i--){
if(hnodes[i]->bodies.size()<info.blimit){
hnodes[i]->bodies.push_back(rb);
inserted=true;
break;
}
}

//Percorreu os nós e não pode adicionar em nenhum... Então lança uma exception.

if(!inserted){
throw runtime_error("Erro ao adicionar objeto na árvore... Ela está cheia!");
}
else{
return;
}
}

//Ok, medidas drásticas precisam ser tomadas aqui...
//Caso seja possível criar um nível novo a partir deste nó, então devemos criar e redistribuir os objetos para os nós criados.
//Caso não seja possível, então lançamos uma exception...

if(sf->level>=info.max_depth){
throw runtime_error("Erro ao adicionar o objeto na octree. A árvore não é capaz de crescer mais neste ramo!");
}

uint64 hsize=sf->bodies.size();

createChilds(sf, 0);//Vamos criar 8 filhos.
redistributeObjects(sf);

//Se nada mudou...
if(sf->bodies.size()==hsize){
throw runtime_error("Erro ao adicionar o objeto na octree. A redistribuição não foi capaz de abrir espaço!");
}

//Vamos recuperar novamente o nó mais profundo e tentar adicionar de novo.
hnodes.clear();
getDeepest(rb, hnodes);
if(sf==hnodes.back()){
sf->bodies.push_back(rb);
}
else{
if(hnodes.back()->bodies.size()<info.blimit){
hnodes.back()->bodies.push_back(rb);
}
else{
sf->bodies.push_back(rb);
}
}
}

void octree::remove(RigidBody* rb){
}

void octree::redistributeObjects(octreenode* parent){
for(uint64 i=0; i<8; i++){
octreenode* sf=getNode(parent->id, i);
_GASSERT(sf!=NULL);
for(uint64 i1=0; i1<parent->bodies.size(); i1++){
vector3d m1, m2;
parent->bodies[i1]->getAabb(m1, m2);
if(aabbInsideAll(sf->center, sf->radius, m1, m2)){
sf->bodies.push_back(parent->bodies[i1]);
parent->bodies.erase(parent->bodies.begin()+i1);
i1--;
}
}
}
}

octreenode* octree::getNode(uint64 hkey, uint32 index){
uint64 id=(hkey<<3)+index;
auto it= nodes.find(id);
return ((it==nodes.end()) ? NULL : it->second);
}
}
