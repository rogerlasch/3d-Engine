

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
setGravity({0,0,-9.81f});
geo=make_shared<octree>();
}

gpp_world::~gpp_world()
{
geo->clear();
geo.reset();
dynamicbodies.clear();
rbDeletions.clear();
for(auto& it : bodies)
{
delete it;
}
bodies.clear();
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

void gpp_world::setGravity(const vector3d& gravity)
{
this->gravity=gravity;
}

vector3d gpp_world::getGravity()const
{
return this->gravity;
}

void gpp_world::addBody(iRigidBody* rb)
{
profiler_snap();
if(BinaryUtils::find(bodies, rb->index, NULL))
{
return;
}
BinaryUtils::insert(bodies, rb);
if(rb->mass==0.0f)
{
geo->insert(rb);
}
else
{
dynamicbodies.push_back(rb);
}
}

bool gpp_world::removeBody(const gpp_index& id)
{
iRigidBody* rb=getBody(id);
if(rb!=NULL)
{
pushBodyDelete(rb);
return true;
}
return false;
}

iRigidBody* gpp_world::getBody(const gpp_index& id)const
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

void gpp_world::pushBodyDelete(iRigidBody* rb)
{
rbDeletions.push_back(rb);
}

void gpp_world::processDeletions()
{
if(rbDeletions.size()==0)return;
for(auto& it : rbDeletions)
{
iRigidBody* r=NULL;
geo->remove(it);
BinaryUtils::remove(bodies, it->index, &r);

}
rbDeletions.clear();
}
}
