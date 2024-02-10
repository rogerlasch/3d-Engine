

#include<algorithm>
#include"../gpp_physics.h"
#include"BroadPhase.h"

using namespace std;

namespace gpp
{

BroadPhase::BroadPhase()
{
gsortaxis=0;
}

void BroadPhase::scan(vector<iRigidBody*>& bodies, CollisionCache* cache)
{
    // Sort the array on currently selected sorting axis (gsortAxis)
std::sort(bodies.begin(), bodies.end(), [&](iRigidBody* r1, iRigidBody*r2)->bool{return r1->aabb->min[gsortaxis]<r2->aabb->min[gsortaxis];});
    // Sweep the array for collisions
    float s[3] = { 0.0f, 0.0f, 0.0f }, s2[3] = { 0.0f, 0.0f, 0.0f }, v[3];
uint32 nsize=bodies.size();
    for (uint32 i=0; i<nsize; i++) {
          // Determine AABB center point
          vector3d p = 0.5f * (bodies[i]->aabb->min + bodies[i]->aabb->max);
          // Update sum and sum2 for computing variance of AABB centers
          for (uint32 c = 0; c < 3; c++) {
                s[c] += p[c];
                s2[c] += p[c] * p[c];
          }
          // Test collisions against all possible overlapping AABBs following current one
          for (uint32 j = i + 1; j < nsize; j++) {
                // Stop when tested AABBs are beyond the end of current AABB
                if (bodies[j]->aabb->min[gsortaxis] > bodies[i]->aabb->max[gsortaxis])
                      break;
uint64 hash=get_hash_from_index(bodies[i]->index, bodies[j]->index);
bool contains=cache->contains(hash);
if(!contains){
                if (aabbOverlap(bodies[i]->aabb, bodies[j]->aabb))
{
shared_collisioninfo info=make_shared<CollisionInfo>();
info->id=hash;
info->r1=bodies[i];
info->r2=bodies[j];
cache->insert(info);
}
}
          }
      }
    // Compute variance (less a, for comparison unnecessary, constant factor)
        for (uint32 c = 0; c < 3; c++)
              v[c] = s2[c] - s[c] * s[c] / bodies.size();
        // Update axis sorted to be the one with greatest AABB variance
uint32 x=0;
        if (v[1] > v[0]) x = 1;
        if (v[2] > v[gsortaxis]) x = 2;
setgSortAxis(x);
}

uint32 BroadPhase::getgSortAxis()const
{
return this->gsortaxis;
}

void BroadPhase::setgSortAxis(uint32 gsortaxis)
{
if(gsortaxis>2) gsortaxis=2;
this->gsortaxis=gsortaxis;
}
}
