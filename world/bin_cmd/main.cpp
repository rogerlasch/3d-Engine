
#include<signal.h>
#include"..\\gpp_world.h"

using namespace std;
using namespace gpp;

RigidBodyList bodies;
 void fillBodies(RigidBodyList& blist, int32 n);
 void dumpBodies(RigidBodyList& bodies);
void printBodies(const RigidBodyList& blist);
void sig_callback(int param);
void SortAndSweepAABBArray(RigidBodyList& bs, CollisionPairList& blist);
void sortBodies(RigidBodyList& hinput, RigidBodyList& hout);
int main()
{
wait_ms(1000);
setlocale(LC_ALL, "Portuguese");
    signal(SIGTERM, sig_callback);
    signal(SIGINT, sig_callback);
    signal(SIGABRT, sig_callback);
log_set_stdout(true);
profiler_start();
log_open_file("main.txt");
fillBodies(bodies, 10000);
int64 start=get_timestamp_ms();
cout<<"Trabalhando"<<endl;
while((get_timestamp_ms()-start)<60000)
{
CollisionPairList blist;
SortAndSweepAABBArray(bodies, blist);
}
profiler_dump("main_dump.txt");
return 0;
}

void sig_callback(int param)
{
profiler_dump("main_dump.txt");
dumpBodies(bodies);
}

void fillBodies(RigidBodyList& blist, int32 n)
{
profiler_snap();
for(uint32 i=0; i<n; i++)
{
RigidBody* rb=new RigidBody();
rb->setIndex({i, i+1});
vector3d m1(random_float(0, 980000), random_float(0, 980000), random_float(0, 980000));
vector3d m2(random_float(m1.x+2, random_float(m1.x+2, m1.x+256)), random_float(m1.y+2, random_float(m1.y+2, m1.y+256)), random_float(m1.z+2, random_float(m1.z+2, m1.z+256)));
rb->min=m1;
rb->max=m2;
blist.push_back(rb);
}
}

void dumpBodies(RigidBodyList& blist)
{
profiler_snap();
for(auto& it : bodies)
{
delete it;
}
bodies.clear();
}

void printBodies(const RigidBodyList& blist)
{
for(auto& it : blist)
{
_FLOG("{}", it->toString());
}
}

void SortAndSweepAABBArray(RigidBodyList& bs, CollisionPairList& blist)
{
profiler_snap();
static uint32 gaxis=0;
static RigidBodyList bodies;
bodies.clear();
sortBodies(bs, bodies);
/*
struct bcomp
{
bool operator()(RigidBody* b1, RigidBody* b2)const
{
return (b1->min.get(gaxis)<b2->min.get(gaxis));
}
};
static bcomp bc;
profiler_set_int("sort_timer", get_timestamp_ms());
std::sort(bodies.begin(), bodies.end(), bc);
profiler_set_int("sort_timer", get_timestamp_ms()-profiler_get_int("sort_timer"));
*/
    float s[3] = { 0.0f, 0.0f, 0.0f }, s2[3] = { 0.0f, 0.0f, 0.0f }, v[3];
    for (uint32 i=0; i<bodies.size(); i++){
          // Determine AABB center point
          vector3d p =(bodies[i]->min + bodies[i]->max)*0.5f;
          // Update sum and sum2 for computing variance of AABB centers
          for (int32 c = 0; c < 3; c++) {
                s[c] += p.get(c);
                s2[c] += p.get(c) * p.get(c);
          }
          // Test collisions against all possible overlapping AABBs following current one
          for (int i1 = i + 1; i1 < bodies.size(); i1++) {
                // Stop when tested AABBs are beyond the end of current AABB
                if (bodies[i1]->min.get(gaxis) > bodies[i]->max.get(gaxis))
{
                      break;
}
                if (aabbOverlap(bodies[i], bodies[i1]))
{
blist.emplace_back(bodies[i], bodies[i1]);
}
          }
      }
    // Compute variance (less a, for comparison unnecessary, constant factor)
        for (int c = 0; c < 3; c++)
              v[c] = s2[c] - s[c] * s[c] / bodies.size();
        // Update axis sorted to be the one with greatest AABB variance
        gaxis = 0;
        if (v[1] > v[0]) gaxis = 1;
        if (v[2] > v[gaxis]) gaxis = 2;
    }

void sortBodies(RigidBodyList& hinput, RigidBodyList& hout)
{
profiler_snap();
struct AabbComparator
{
bool operator()(RigidBody* a1, RigidBody* a2)
{
return a1->index<a2->index;
}
};
static AabbComparator abc;
for(auto& it : hinput)
{
auto it2=std::upper_bound(hout.begin(), hout.end(), it, abc);
hout.insert(it2, it);
}
}
