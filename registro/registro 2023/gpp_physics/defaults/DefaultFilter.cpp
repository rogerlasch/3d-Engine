

#include"../gpp_physics.h"
#include"DefaultFilter.h"

using namespace std;

namespace gpp{
void DefaultFilter::filter(vector<iRigidBody*>& hinput, vector<iRigidBody*>& hout, WorldInfo* info, CollisionCache* hcache)
{
        uint32 size = hinput.size();
        for (uint32 i = 0; i < size; i++) {
                filter_and_prepare(hinput[i], hout, info, hcache);
        }
}

void DefaultFilter::filter_and_prepare(iRigidBody* rb, vector<iRigidBody*>& hbodies, WorldInfo* info, CollisionCache* hcache)
{
profiler_snap();
_GASSERT(rb!=NULL);
if(rb->mass<=0.0f) return;
rb->calcLoads(info);
hbodies.push_back(rb);
}
}
