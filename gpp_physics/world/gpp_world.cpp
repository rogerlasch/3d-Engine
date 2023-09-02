

#include<algorithm>
#include<sstream>
#include"../gpp_physics.h"
#include"gpp_world.h"

using namespace std;

namespace gpp
{
gpp_world::gpp_world(WorldInfo* winfo, WorldCallbacks* hcon)
{
profiler_snap();
geo=make_unique<octree>();
this->hcon=make_unique<WorldCallbacks>();
this->info=make_unique<WorldInfo>();
((hcon==NULL) ? this->hcon->createDefault() : this->hcon->copy_from(hcon));
if(winfo==NULL) info->toDefault();
else *info=*winfo;
geo->create(info->min, 15, 64, fabs(info->max.x-info->min.x));
}

gpp_world::~gpp_world()
{
hcon->cleanup();
hcon.reset();
geo->clear();
geo.reset();
info->cleanup();
info.reset();
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
ss<<info->toString()<<endl;
ss<<"Total de corpos no mundo: "<<bodies.size()<<endl;
ss<<"Listando corpos:"<<endl<<endl;
for(auto& it : bodies)
{
ss<<it->toString()<<endl;
}
return ss.str();
}

void gpp_world::addBody(iRigidBody* rb)
{
profiler_snap();
if(BinaryUtils::find(bodies, rb->index, NULL))
{
return;
}
BinaryUtils::insert(bodies, rb);
if(rb->mass<=0.0f)
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
_GASSERT(hcon->isValid());
profiler_snap();
/*
vector<iRigidBody*> activeBodies;
vector<CollisionInfo> bcollisions, ncollisions;
hcon->filter->filter(dynamicbodies, activeBodies, info.get());
hcon->hMotion->moveBodyList(activeBodies, dt);
gew->broadPhase(activeBodyes, nbodies);
hcon->bPhase->scan(mbodies, bcollisions);
hcon->nPhase->detectCollisions(bcollisions, ncollisions);
hcon->cSolver->solve(ncollisions);
*/
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
