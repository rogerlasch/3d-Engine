

#include"gpp_physics.h"
#include"BroadPhase.h"

using namespace std;

namespace gpp
{

struct SweepComparator
{
uint32 gSortAxis;
bool operator()(RigidBody* rb1, RigidBody* rb2)
{
return rb1->getMin()[gSortAxis]<rb2->getMin()[gSortAxis];
}
};

BroadPhase::BroadPhase()
{
gsortaxis=0;
}

void BroadPhase::scan(RigidBodyList& bodies, CollisionPairList& collisions)
{
    // Sort the array on currently selected sorting axis (gsortAxis)
SweepComparator scp;
scp.gSortAxis=getgSortAxis();
RigidBodyList bodies2(bodies.begin(), bodies.end());
std::sort(bodies2.begin(), bodies2.end(), scp);
    // Sweep the array for collisions
    float s[3] = { 0.0f, 0.0f, 0.0f }, s2[3] = { 0.0f, 0.0f, 0.0f }, v[3];
    for (uint32 i=0; i<bodies2.size(); i++) {
          // Determine AABB center point
          vector3d p = 0.5f * (bodies2[i]->min + bodies2[i]->max);
          // Update sum and sum2 for computing variance of AABB centers
          for (uint32 c = 0; c < 3; c++) {
                s[c] += p[c];
                s2[c] += p[c] * p[c];
          }
          // Test collisions against all possible overlapping AABBs following current one
          for (uint32 j = i + 1; j < bodies2.size(); j++) {
                // Stop when tested AABBs are beyond the end of current AABB
                if (bodies2[j]->min[gsortaxis] > bodies2[i]->max[gsortaxis])
                      break;
                if (aabbOverlap(bodies2[i], bodies2[j]))
{
collisions.emplace_back(bodies2[i], bodies2[j]);
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

void BroadPhase::scan(RigidBodyList& bodies, CollisionPairList& collisions, std::shared_mutex& m)
{
unique_lock<shared_mutex> lck(m, defer_lock);
while(!lck.try_lock())
{
}
this->scan(bodies, collisions);
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
