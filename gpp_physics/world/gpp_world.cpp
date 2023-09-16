

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
this->hcache=make_unique<CollisionCache>();
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
hcache->cleanup();
hcache.reset();
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
profiler_snap();
_GASSERT(hcon->isValid());
info->timestep=dt;
info->currenttime+=dt;
vector<iRigidBody*> activeBodies;
hcon->filter->filter(dynamicbodies, activeBodies, info.get(), hcache.get());
hcon->hMotion->moveBodyList(activeBodies, dt);
geo->BroadPhase(activeBodies, hcache.get());
hcon->bPhase->scan(activeBodies, hcache.get());
hcon->nPhase->detectCollisions(hcache.get());
hcon->iSolver->solve(hcache.get());
if(hcon->hcall!=NULL){
auto& htable=hcache->getTable();
uint32 size=htable.size();
for(uint32 i=0; i<size; i++){
hcon->hcall(htable[i]);
}
}
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
hcache->removeByIndex(it->index);
}
rbDeletions.clear();
}
}
