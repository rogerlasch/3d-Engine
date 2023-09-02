

#include<unordered_set>
#include<sstream>
#include<algorithm>
#include<functional>
#include <utility>
#include<unordered_map>
#include<vector>
#include"../gpp_physics.h"
#include"CollisionCache.h"

using namespace std;

namespace gpp{
CollisionCache::CollisionCache()
{
this->cleanup();
}

CollisionCache::~CollisionCache()
{
}

string CollisionCache::toString()const
{
stringstream ss;
ss<<fixed;
ss<<"Imprimindo CollisionCache..."<<endl;
ss<<"Número de índices mapeados: "<< hindexs.size()<<endl;
ss<<"Número de colisões registradas: "<< hcollisions.size()<<endl;
ss<<"Número de linhas na tabela... "<< htable.size()<<endl;
return ss.str();
}

void CollisionCache::cleanup()
{
hindexs.clear();
hcollisions.clear();
htable.clear();
}

void CollisionCache::insert(const shared_collisioninfo& info)
{
profiler_snap();
if(hcollisions.find(info->id)!=hcollisions.end()){
profiler_inc("hash_collision", 1);
 return;
}
//O objeto dominante é sempre o r1...
auto it=hindexs.find(make_pair(info->r1->index.vnum, info->r1->index.subvnum));
if(it==hindexs.end())
{
//Adicione uma nova linha, pois ele é o primeiro registro...
shared_collisionrow row=make_shared<CollisionRow>();
row->push_back(info);
htable.push_back(row);
hcollisions.insert(make_pair(info->id, info));
uint32 index=htable.size()-1;
hindexs.insert(make_pair(make_pair(info->r1->index.vnum, info->r1->index.subvnum),
index));
}
else
{
hcollisions.insert(make_pair(info->id, info));
htable[it->second]->push_back(info);
}
}

bool CollisionCache::removeByHash(uint64 id)
{
profiler_snap();
bool update=false;
if(hcollisions.find(id)==hcollisions.end()) return false;
hcollisions.erase(id);
uint32 size=htable.size();
for(uint32 i=0; i<size; i++){
auto it = std::find_if(htable[i]->begin(), htable[i]->end(), [id](const shared_collisioninfo& info) -> bool {     return info->id == id; });
if(it!=htable[i]->end())
{
if(htable[i]->size()==1)
{
gpp_index dg=htable[i]->at(0)->r1->index;
hindexs.erase(make_pair(dg.vnum, dg.subvnum));
}
htable[i]->erase(it);
if(htable[i]->size()==0){
update=true;
}
break;
}
}
if(update){
internalUpdateRows();
}
return true;
}

uint32 CollisionCache::removeByHashs(const std::vector<uint64>& hashs)
{
profiler_snap();
uint32 x=0;
vector<gpp_index> hremove;
for(auto& it : hashs)
{
if(hcollisions.find(it)!=hcollisions.end())
{
hcollisions.erase(it);
x++;
}
}
uint32 size=htable.size();
for(uint32 i=0; i<size; i++){
if(htable[i]->size()==0)continue;
for(auto& id : hashs){
auto it=std::find_if(htable[i]->begin(), htable[i]->end(), [id](const shared_collisioninfo& info)->bool{return info->id==id;});
if(it!=htable[i]->end())
{
if(htable[i]->size()==1){
hremove.push_back(htable[i]->at(0)->r1->index);
}
htable[i]->erase(it);
}
}
}
if(hremove.size()>0){
for(auto& it : hremove){
hindexs.erase(make_pair(it.vnum, it.subvnum));
}
internalUpdateRows();
}
return x;
}

bool CollisionCache::removeByIndex(const gpp_index& id)
{
profiler_snap();
unordered_set<gpp_index, IndexHasher> ids;
unordered_set<uint64> hashs;
uint32 size=htable.size();
for(uint32 i=0; i<size; i++){
auto it=std::find_if(htable[i]->begin(), htable[i]->end(), [id](const shared_collisioninfo& info)->bool{return info->r1->index==id||info->r2->index==id;});
if(it!=htable[i]->end()){
hashs.insert((*it)->id);
if(htable[i]->size()==1)
{
ids.insert((*it)->r1->index);
}
htable[i]->erase(it);
}
}
for(auto& it : hashs){
hcollisions.erase(it);
}
for(auto& it : ids){
hindexs.erase(make_pair(it.vnum, it.subvnum));
}
if(ids.size()>0){
internalUpdateRows();
}
return true;
}

uint32 CollisionCache::removeByIndexs(const std::vector<gpp_index>& ids)
{
profiler_snap();
uint32 x=0;
unordered_set<gpp_index, IndexHasher> handles;
unordered_set<uint64> hashs;
uint32 size=htable.size();
for(uint32 i=0; i<size; i++){
for(auto& index : ids){
auto it=std::find_if(htable[i]->begin(), htable[i]->end(), [index](const shared_collisioninfo& info)->bool{return info->r1->index==index||info->r2->index==index;});
if(it!=htable[i]->end()){
hashs.insert((*it)->id);
if(htable[i]->size()==1)
{
handles.insert((*it)->r1->index);
}
htable[i]->erase(it);
x++;
}
}
}
for(auto& it : hashs){
hcollisions.erase(it);
}
for(auto& it : handles){
hindexs.erase(make_pair(it.vnum, it.subvnum));
}
if(handles.size()>0){
internalUpdateRows();
}
return x;
}

bool CollisionCache::contains(uint64 id)const
{
profiler_snap();
return hcollisions.find(id)!=hcollisions.end();
}

bool CollisionCache::contains(const gpp_index& id)const
{
profiler_snap();
return hindexs.find(make_pair(id.vnum, id.subvnum))!=hindexs.end();
}

shared_collisioninfo CollisionCache::get(uint64 id)
{
profiler_snap();
auto it=hcollisions.find(id);
return ((it==hcollisions.end()) ? shared_collisioninfo() : it->second);
}

shared_collisionrow CollisionCache::getRow(const gpp_index& id)
{
profiler_snap();
auto it=hindexs.find(make_pair(id.vnum, id.subvnum));
return ((it==hindexs.end()) ? shared_collisionrow() : htable[it->second]);
}

void CollisionCache::internalUpdateRows()
{
profiler_snap();
if(hindexs.size()>htable.size())
{
//hindexs.clear();
}
for(uint32 i=0; i<htable.size(); i++)
{
if(htable[i]->size()==0)
{
htable[i].reset();
htable.erase(htable.begin()+i);
i--;
continue;
}
else
{
gpp_index& id=htable[i]->at(0)->r1->index;
auto it=hindexs.find(make_pair(id.vnum, id.subvnum));
if(it==hindexs.end())
{
hindexs.insert(make_pair(make_pair(id.vnum, id.subvnum), i));
}
else
{
it->second=i;
}
}
}
}
}
