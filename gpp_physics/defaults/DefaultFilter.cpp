

#include"../gpp_physics.h"
#include"DefaultFilter.h"

using namespace std;

namespace gpp{
void DefaultFilter::filter(vector<iRigidBody*>& hinput, vector<iRigidBody*>& hout, WorldInfo* info)
{
        uint32 size = hout.size();
        for (uint32 i = 0; i < size; i++) {
                filter_and_prepare(hinput[i], hout, info);
        }
}

void DefaultFilter::filter_and_prepare(iRigidBody* rb, vector<iRigidBody*>& hbodies, WorldInfo* info)
{
if(rb->mass<=0.0f) return;
if((info->flags&GWF_GRAVITY)==GWF_GRAVITY)
{
rb->applyForce(info->gravity*rb->mass);
}
hbodies.push_back(rb);
}
}
