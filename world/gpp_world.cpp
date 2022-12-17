

#include<algorithm>
#include"gpp_world.h"

using namespace std;

namespace gpp
{
gpp_world::gpp_world():
AABB()
{
setMin({0,0,-1000000});
setMax({1000000, 1000000, 1000000});
}

gpp_world::~gpp_world()
{
profiler_snap();
}

void gpp_world::setWordEventCallBack(WORDEVENTCALLBACK hcall)
{
this->hcall=hcall;
}

WORDEVENTCALLBACK gpp_world::getWordEventCallBack()const
{
return this->hcall;
}

void gpp_world::setGravity(const vector3d& gravity)
{
this->gravity=gravity;
}

vector3d gpp_world::getGravity()const
{
return this->gravity;
}

vector<RigidBody*>& gpp_world::getBodies()
{
return bodies;
}

void gpp_world::addBody(RigidBody* rb)
{
if(BinaryUtils::find(bodies, rb->index, NULL))
{
return;
}
BinaryUtils::insert(bodies, rb);
}

bool gpp_world::removeBody(const gpp_index& id)
{
RigidBody* r=NULL;
return BinaryUtils::remove(bodies, id, &r);
}

RigidBody* gpp_world::getBody(const gpp_index& id)const
{
uint32 x=0;
if(BinaryUtils::find(bodies, id, &x))
{
return bodies[x];
}
return NULL;
}

void gpp_world::update(float dt)
{
profiler_snap();
_GASSERT(hcall!=NULL);
for(auto& it : bodies)
{
if((it->containsBodyFlag(RB_STATIC))||(it->getMass()==0))
{
continue;
}
vector3d v;
v=it->nextStep(dt);
if(!v.isNull())
{
it->translate(v);
runcallback(WE_STEP, it, NULL);
}
}
BroadPhase bs;
NarrowPhase ns;
CollisionSolver cv;
CollisionPairList BroadCollisions, NarrowCollisions;
bs.scan(bodies, BroadCollisions);
ns.scan(BroadCollisions, NarrowCollisions);
cv.solve(NarrowCollisions);
for(auto& it : NarrowCollisions)
{
runcallback(WE_COLLISION, it.first, it.second);
}
}

string gpp_world::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Estatísticas do mundo:"<<endl;
ss<<"Gravidade: "<<gravity<<endl;
ss<<"LowerBound: "<<min<<endl;
ss<<"HigherBound: "<<max<<endl;
ss<<"Total de corpos no mundo: "<<bodies.size()<<endl;
return ss.str();
}

void gpp_world::runcallback(uint32 type, void* v1, void* v2)
{
if(hcall)
{
WordEvent ev;
ev.type=type;
ev.v1=v1;
ev.v2=v2;
hcall(&ev);
}
}
}
