


#ifndef LINEAR_OCTREE_H
#define LINEAR_OCTREE_H

#include<libmorton/morton.h>
using namespace libmorton;

namespace ln{

class OctreeNode2{
public:
uint64 id;
uint8 pchilds;
float radius;
vector3d center;
OctreeNode2(){
id=0;
pchilds=0;
radius=0.0f;
center={0.0f, 0.0f, 0.0f};
}
std::string toString()const{
std::stringstream ss;
ss<<std::fixed;
ss.precision(2);
ss<<"Id="<<id<<", pchilds="<<(uint64)pchilds<<std::endl;
vector3d min=center-radius;
vector3d max=center+radius;
std::string s="xyz";
for(uint64 i=0; i<3; i++){
ss<<s[i]<<": "<<min[i]<<", "<<max[i]<<std::endl;
}
return ss.str();
}
};

class LinearOctree{
public:
uint64 root;
std::unordered_map<uint64, OctreeNode2*> nodes;
LinearOctree(){
}
~LinearOctree(){
for(auto& it : nodes){
delete it.second;
}
nodes.clear();
}
std::string toString(){
std::stringstream ss;
ss<<"Número de nós na árvore: "<<nodes.size()<<std::endl;
for(auto& it : nodes){
ss<<it.second->toString()<<std::endl;
}
return ss.str();
}
void appendNode(OctreeNode2* node){
if(node->id==0){
node->id=morton3D_64_encode(static_cast<uint32>(node->center.x), static_cast<uint32>(node->center.y), static_cast<uint32>(node->center.z));
}
if(nodes.size()==0){
root=node->id;
}
nodes.insert(make_pair(node->id, node));
}
void build(OctreeNode2* parent){
float step=parent->radius*0.5f;
for(uint32 i=0; i<8; i++){
if((parent->pchilds&(1<<i))==0){
            OctreeNode2* sf = new OctreeNode2();
            sf->center.x = ((i & 1) ? step : -step);
            sf->center.y = ((i & 2) ? step : -step);
            sf->center.z = ((i & 4) ? step : -step);
            sf->center +=parent->center;
            sf->radius = step;
sf->id=(parent->id<<3)+i;
appendNode(sf);
sf->pchilds=0;
parent->pchilds|=1<<i;
}
}
}
void traverse(uint64 id){
OctreeNode2* root=nodes.find(id)->second;
for(uint32 i=0; i<8; i++){
if((root->pchilds&(1<<i))){
uint64 key=(root->id<<3)+i;
auto it=nodes.find(key);
_GINFO("Procurando {}", key);
if(it==nodes.end()){
_GINFO("Filho de id {} não encontrado.", key);
}
else{
_GINFO("Mostrando filho {}\n{}", key, it->second->toString());
}
}
}
}
void getIds(std::vector<uint64>& ids){
for(auto& it : nodes){
ids.push_back(it.first);
}
}
};
}
#endif
