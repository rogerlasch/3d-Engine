

#include<algorithm>
#include"gpp_physics.h"
#include"gpp_world.h"

using namespace std;

namespace gpp
{
gpp_world::gpp_world():
AABB()
{
setMin({0,0,-1000000});
setMax({1000000, 1000000, 1000000});
bdebug.clear();
}

gpp_world::~gpp_world()
{
}

void gpp_world::setGravity(const vector3d& gravity)
{
this->gravity=gravity;
}

vector3d gpp_world::getGravity()const
{
return this->gravity;
}

void gpp_world::setBodyDebug(const gpp_index& id)
{
bdebug.insert(id);
}

void gpp_world::removeBodyDebug(const gpp_index& id)
{
bdebug.erase(id);
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
for(auto& it : bodies)
{
it->setForce(gravity);
if(bdebug.find(it->getIndex())!=bdebug.end())
{
_FLOG("{}", it->getPosition().toString());
}
}
IntegratorVerlet vt;
BroadPhase ph;
NarrowPhase nr;
CollisionSolver cs;
CollisionPairList pcollisions;
CollisionList collisions;
vt.integrate(bodies, dt);
ph.scan(bodies, pcollisions);
nr.scan(pcollisions, collisions);
cs.solve(collisions);
}

string gpp_world::toString()const
{
stringstream ss;
ss<<fixed;
ss<<"Estatísticas do mundo:"<<endl;
ss<<"Gravidade: "<<gravity<<endl;
ss<<"LowerBound: "<<min<<endl;
ss<<"HigherBound: "<<max<<endl;
ss<<"Total de corpos no mundo: "<<bodies.size()<<endl;
ss<<"Listando corpos:"<<endl<<endl;
for(auto& it : bodies)
{
ss<<it->toString()<<endl;
}
return ss.str();
}
}
