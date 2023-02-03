

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
}
