
#include<unordered_set>
#include"../gpp_physics.h"
#include"NarrowPhase.h"

using namespace std;

namespace gpp
{
void NarrowPhase::detectCollisions(CollisionCache* cache)
{
unordered_set<uint64> hremove;
auto& hmap=cache->getMapped();
for(auto it=hmap.begin(); it!=hmap.end(); ++it){
if(!collision3d::isColliding(it->second->r1->getGeometricShape(), it->second->r2->getGeometricShape(), it->second.get())) {
hremove.insert(it->first);
}
}
if(hremove.size()>0){
cache->removeByHashs(hremove);
}
}
}
