
#include<stack>
#include<algorithm>
#include<functional>
#include<string.h>
#include<sstream>
#include<libmorton/morton.h>
#include<gpp/debug_system.h>
#include"../types.h"
#include"../math/math.h"
#include"../geometry/geometry.h"
#include"../world/RigidBody.h"
#include"../collision/collision.h"
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
if(nodes.size()<10000){
    OCTREEVISITORCALLBACK hvisitor = [&](octreenode* hnode)->bool {
ss<<hnode->toString()<<endl;
return true;
        };
    traverse(hvisitor, 0);
}
return ss.str();
}

void octree::create(const vector3d& center, float radius){
profiler_snap();
_GASSERT_MSG(root==NULL, "Já existe uma octree válida aqui. Limpe antes de chamar create novamente.");
vector3d hpoint;
if(center.length()<=0.0001f){
hpoint={1, 2, 3};
}
else{
hpoint=center;
}

octreenode* node=new octreenode;
node->id=_encodeMortonKey(hpoint);
node->parent=0;
node->center=center;
node->radius=radius;
node->level=0;
node->pchilds=0;
node->setOctree(this);
root=node;
insertNode(node);
node->createChilds();//Cria apenas o nível 1... Ela inicia com 9 nós...
}

/**
*Método de travecia da árvore...
*Como esta árvore é diferente, resolvi escrever um método de travecia e passar um retorno de chamada para iterar em cada nó.
*Se o retorno de chamada retornar true, o método continua descendo pela árvore até que não aja mais filhos.
*Se retornar false, a descida é interrompida e continua em um nó irmão, ou nó pai, depende de qual foi empilhado.
*start_node pode ser especificado para começar em um nó específico. Se 0 for passado, o nó raíz será usado.
**/
void octree::traverse(OCTREEVISITORCALLBACK hvisitor, uint64 start_id)const{
profiler_snap();
if((!hvisitor)||(nodes.size()==0)) return;
stack<octreenode*> pstack;
if((start_id==0)||(start_id==root->id)){
pstack.push(root);
}
else{
auto it=nodes.find(start_id);
_GASSERT(it!=nodes.end());
pstack.push(it->second);
}

while(!pstack.empty()){
octreenode* node=pstack.top();
pstack.pop();
_GASSERT(node!=NULL);
if (!hvisitor(node)) continue;
if(node->pchilds>0){
for(uint64 i=0; i<8; i++){
if((node->pchilds&(1<<i))){
uint64 id=(node->id<<3)+i;
pstack.push(nodes.at(id));
}
}
}
}
}

void octree::getDeepest(RigidBody* rb, vector<octreenode*>& pnodes, uint64 start_id) {
    vector3d m1, m2;
    rb->hbody->getAABB(m1, m2);
pnodes.clear();
    OCTREEVISITORCALLBACK hvisitor = [&](octreenode* hnode)->bool {
        if (aabbInsideAll(hnode->center, hnode->radius, m1, m2)) {
pnodes.push_back(hnode);
            return true;
        }
        return false;
        };
    traverse(hvisitor, start_id);
}

void octree::insert(RigidBody* rb){
profiler_snap();
vector<octreenode*> hnodes;
getDeepest(rb, hnodes, 0);
//Tentar adicionar o objeto no nó mais profundo...
octreenode* sf=hnodes.back();

if(sf->hbodies.size()<info.blimit){
insertBodyAtNode(sf, rb);
return;
}

//Caso não possa adicionar no nó mais profundo, e não seja mais possível criar nós filhos...
//Então percorre os nós do mais profundo para o mais raso tentando encaixar.
if(sf->pchilds>0){
bool inserted=false;
if(hnodes.size()==1){
insertBodyAtNode(hnodes[0], rb);
return;
}

for(uint64 i=hnodes.size(); i>0; i--){
if(hnodes[i-1]->hbodies.size()<info.blimit){
insertBodyAtNode(hnodes[i-1], rb);
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

uint64 hsize=sf->hbodies.size();

sf->createChilds();
redistributeObjects(sf);

//Se nada mudou...
if(sf->hbodies.size()==hsize){
throw runtime_error("Erro ao adicionar o objeto na octree. A redistribuição não foi capaz de abrir espaço!");
}

//Vamos recuperar novamente o nó mais profundo e tentar adicionar de novo.
hnodes.clear();
getDeepest(rb, hnodes, 0);
if(sf==hnodes.back()){
insertBodyAtNode(sf, rb);
}
else{
if(hnodes.back()->hbodies.size()<info.blimit){
insertBodyAtNode(hnodes.back(), rb);
}
else{
insertBodyAtNode(sf, rb);
}
}
}

void octree::remove(RigidBody* rb){
octreenode* hnode=rb->getOctreeNode();
if(hnode!=NULL){
hnode->hbodies.erase(rb->id);
rb->setOctreeNode(NULL);
}
}

void octree::transiction(RigidBody* rb){
octreenode* sf=rb->getOctreeNode();
vector3d m1, m2;
rb->hbody->getAABB(m1, m2);
if(aabbInsideAll(sf->center, sf->radius, m1, m2)){
if(sf->pchilds==0)return;
for(uint64 i=0; i<8; i++){
uint64 id=(sf->id<<3)+i;
auto it=nodes.find(id);
_GASSERT(it!=nodes.end());
if(aabbInsideAll(it->second->center, it->second->radius, m1, m2)){
it->second->hbodies.insert(make_pair(rb->id, rb));
sf->hbodies.erase(rb->id);
rb->setOctreeNode(it->second);
break;
}
}
}
else{
octreenode* sf=rb->getOctreeNode();
bool done=false;
while(done==false){
if(sf->parent==0)break;
auto it=nodes.find(sf->parent);
_GASSERT(it!=nodes.end());
sf=it->second;
if(aabbInsideAll(sf->center, sf->radius, m1, m2)){
done=true;
break;
}
}
if(done==false)return;
vector<octreenode*> hnodes;
getDeepest(rb, hnodes, sf->id);
_GASSERT(hnodes.size()!=0);
sf=hnodes.back();
sf->hbodies.insert(make_pair(rb->id, rb));
rb->getOctreeNode()->hbodies.erase(rb->id);
rb->setOctreeNode(sf);
}
}

void octree::broadPhase(vector<CollisionInfo*>& collisions){
profiler_snap();
collisions.clear();

OCTREEVISITORCALLBACK hvisitor=[&](octreenode* node)->bool{
for(auto it=node->hbodies.begin(); it!= node->hbodies.end(); ++it){
broadPhaseAuxiliary(it, node->id, collisions);
}
return true;
};

traverse(hvisitor);
}

void octree::broadPhaseAuxiliary(const std::unordered_map<uint64, RigidBody*>::iterator& begin_loop, uint64 start_node, vector<CollisionInfo*>& collisions){
profiler_snap();
stack<octreenode*> pstack;
pstack.push(nodes.at(start_node));
vector3d m1, m2, m3, m4;
RigidBody* rb=begin_loop->second;
rb->hbody->getAABB(m1, m2);
while(!pstack.empty()){
octreenode* sf=pstack.top();
pstack.pop();
unordered_map<uint64, RigidBody*>::iterator obj;
if(sf->id==start_node){
obj=begin_loop;
}
else{
obj=sf->hbodies.begin();
}
while(obj!=sf->hbodies.end()){
if(rb==obj->second||rb->mass<=0.0f&&obj->second->mass<=0.0f){
obj++;
continue;
}
obj->second->hbody->getAABB(m3, m4);
if(aabbOverlap(m1, m2, m3, m4)){
CollisionInfo* c=new CollisionInfo();
c->r1=rb;
c->r2=obj->second;
c->id=getCollisionId(c->r1->id, c->r2->id);
if(c->r1->mass<=0.0f)swap(c->r1, c->r2);
collisions.push_back(c);
}
obj++;
}

if(sf->pchilds>0){
for(uint64 i=0; i<8; i++){
if((sf->pchilds&(1<<i))==0)continue;
octreenode* node=nodes.at((sf->id<<3)+i);
_GASSERT(node!=NULL);
if(aabbOverlap(node->center, node->radius, m1, m2)){
pstack.push(node);
}
}
}
}
}

void octree::rayCast(const vector3d& origin, const vector3d& dir, vector<RayInfo>& infos){
profiler_snap();
RayInfo info;
    OCTREEVISITORCALLBACK hvisitor = [&](octreenode* hnode)->bool {
profiler_snap();
if(rayBox(origin, dir, hnode->center-hnode->radius, hnode->center+hnode->radius, NULL)){
for(auto it=hnode->hbodies.begin(); it!=hnode->hbodies.end(); ++it){
if(CollisionDispatcher::rayCast(origin, dir, it->second->hbody, &info)){
info.targetId=it->second->id;
infos.push_back(info);
}
}
return true;
}
return false;
        };
    traverse(hvisitor, 0);
}

void octree::redistributeObjects(octreenode* parent){
vector<uint64> hkeys;
for(uint64 i=0; i<8; i++){
octreenode* sf=getNode(parent->id, i);
_GASSERT(sf!=NULL);
for(auto it=parent->hbodies.begin(); it!=parent->hbodies.end(); ++it){
vector3d m1, m2;
it->second->hbody->getAABB(m1, m2);
if(aabbInsideAll(sf->center, sf->radius, m1, m2)){
sf->hbodies.insert(make_pair(it->first, it->second));
hkeys.push_back(it->first);
}
}
}
for(auto& it : hkeys){
parent->hbodies.erase(it);
}
}

octreenode* octree::getNode(uint64 hkey, uint32 index){
uint64 id=(hkey<<3)+index;
auto it= nodes.find(id);
return ((it==nodes.end()) ? NULL : it->second);
}

void octree::insertNode(octreenode* hnode){
auto it=nodes.find(hnode->id);
_GASSERT_MSG(it==nodes.end(), "Erro, tentando adicionar nó com chave duplicada! Nó:\n{}", hnode->toString());
nodes.insert(make_pair(hnode->id, hnode));
info.nodes=nodes.size();
}

void octree::insertBodyAtNode(octreenode* node, RigidBody* rb){
node->hbodies.insert(make_pair(rb->id, rb));
rb->setOctreeNode(node);
info.nbodies++;
}
}
