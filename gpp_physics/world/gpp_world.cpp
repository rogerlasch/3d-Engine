
#include"../types.h"
#include"../math/math.h"
#include"../geometry/geometry.h"
#include"../collision/collision.h"
#include"../octree/octree.h"
#include "gpp_world.h"


using namespace std;

namespace gpp {

gpp_world::gpp_world(){
hstate.store(GWF_DEFAULT);
hprops=NULL;
hgeo=NULL;
hclock=NULL;
}

gpp_world::gpp_world(const WorldInfo& pw){
hstate.store(GWF_DEFAULT);
hprops=NULL;
hgeo=NULL;
hclock=NULL;
    create(pw);
}

gpp_world::~gpp_world() {
if(hprops) delete hprops;
if(hgeo) delete hgeo;
if(hclock) delete hclock;
hprops=NULL;
hgeo=NULL;
hclock=NULL;
bodies.clear();
}

string gpp_world::toString()const{
stringstream ss;
ss<<fixed;
ss.precision(3);
ss<<hprops->toString()<<endl;
ss<<"Total de objetos no mundo: "<<bodies.size()<<endl;
ss<<"Tamanho em bites: "<<sizeof(gpp_world)<<endl;
return ss.str();
}

bool gpp_world::create(const WorldInfo& props) {
    if (hstate != GWF_DEFAULT) {
        return false; // O world já foi criado
    }

    this->hprops = new WorldInfo(props);
hgeo=new octree();
hclock = new ClockFrame();
hgeo->create(hprops->center, hprops->alf);

    hstate = GWF_ACTIVE;

    return true;
}

bool gpp_world::pushBody(RigidBody* rb) {
    // Adiciona um corpo rígido ao mundo
    if (hstate.load() != GWF_ACTIVE) {
        return false; // O mundo não está ativo
    }
auto it=find_if(bodies.begin(), bodies.end(), [&](RigidBody* r)->bool{return r->id==rb->id;});
if(it!=bodies.end()){
return false;
}
    bodies.push_back(rb);
hgeo->insert(rb);
    return true;
}

bool gpp_world::removeBody(uint32 id) {
    // Remove um corpo rígido pelo ID

auto it=find_if(bodies.begin(), bodies.end(), [&](RigidBody* r)->bool{return r->id==id;});
if(it!=bodies.end()){
bodies.erase(it);
hgeo->remove(*it);
return true;
}
return false;
}

bool gpp_world::removeBody(RigidBody* rb) {
    // Remove um corpo rígido
auto it=find_if(bodies.begin(), bodies.end(), [&](RigidBody* r)->bool{return r->id==rb->id;});
if(it!=bodies.end()){
bodies.erase(it);
hgeo->remove(rb);
return true;
}
return false;
}

RigidBody* gpp_world::getBody(uint32 id) const {
    // Obtém um corpo rígido pelo ID
auto it=find_if(bodies.begin(), bodies.end(), [&](RigidBody* r)->bool{return r->id==id;});
return ((it==bodies.end()) ? NULL : *it);
}

void gpp_world::rayCast(const vector3d& origin, const vector3d& dir, std::vector<RayInfo>& infos){
if(hgeo!=NULL){
hgeo->rayCast(origin, dir, infos);
}
}

uint32 gpp_world::pushEventClock(uint32 hframes, EVENTFRAMECALLBACK hcall, void* userData, void* v1, void* v2){
EventFrame ev;
ev.counter=0;
ev.frequence=hframes;
ev.hcall=hcall;
ev.userdata = userData;
ev.v2=v2;
return pushEventClock(&ev);
}

uint32 gpp_world::pushEventClock(EventFrame* ev){
    return hclock->pushEvent(ev);
}

bool gpp_world::removeEventClock(uint32 id){
    return hclock->removeEvent(id);
}

void gpp_world::update(decimal dt){
hclock->update();
}
}
