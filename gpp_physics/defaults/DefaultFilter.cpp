

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
vector3d forces;
if(fabs(rb->velocity.z)<1) rb->velocity.z=0.0f;
if((info->flags&GWF_GRAVITY)==GWF_GRAVITY){
if(rb->contacts.load()==0){
forces+=info->gravity*rb->mass;
}
else{
forces.x+=info->gravity.x*rb->mass;
forces.y+=info->gravity.y*rb->mass;
}
}
rb->applyForce(forces);
hbodies.push_back(rb);
}
}
