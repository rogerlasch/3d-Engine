
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
}

gpp_world::gpp_world(const WorldProperties& pw){
hstate.store(GWF_DEFAULT);
hprops=NULL;
hgeo=NULL;
    create(pw);
}

gpp_world::~gpp_world() {
    stop();
if(hprops) delete hprops;
if(hgeo) delete hgeo;
hprops=NULL;
hgeo=NULL;
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

bool gpp_world::create(const WorldProperties& props) {
    if (hstate != GWF_DEFAULT) {
        return false; // O world já foi criado
    }

    hprops = new WorldProperties(props);
hgeo=new octree();
hgeo->create(hprops->center, hprops->radius);

    hstate = GWF_ACTIVE;

    return true;
}

bool gpp_world::addBody(RigidBody* rb) {
    profiler_snap();
    // Adiciona um corpo rígido ao mundo
    if (hstate.load() != GWF_ACTIVE) {
        return false; // O mundo não está ativo
    }
auto it=find_if(bodies.begin(), bodies.end(), [&](RigidBody* r)->bool{return r->id==rb->id;});
if(it!=bodies.end())return false;
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
profiler_snap();
if(hgeo!=NULL){
hgeo->rayCast(origin, dir, infos);
}
}

bool gpp_world::start() {
if(hstate.load()>=GWF_RUNNING)return false;
handle=thread(std::bind(&gpp_world::gameLoop, this));
while(hstate.load()!=GWF_RUNNING){
this_thread::yield();
}
return true;
}

void gpp_world::stop() {
if(hstate.load()==GWF_STOPPING) return;
hstate.store(GWF_STOPPING);
handle.join();
hstate.store(GWF_ACTIVE);
}

void gpp_world::gameLoop(){
hstate.store(GWF_RUNNING);
_GINFO("Entrando no loop principal do jogo.");
using mClock=chrono::high_resolution_clock;

int64 frameTime=1000000000/hprops->fps;
hprops->timeStep=1.0f/(decimal)hprops->fps;
mClock::time_point tps;
while(hstate.load()==GWF_RUNNING){
tps=mClock::now();
internalUpdate(hprops->timeStep);
mClock::time_point tend=mClock::now();
int64 x=chrono::duration_cast<chrono::nanoseconds>(tend-tps).count();
x=frameTime-x;
_GINFO("sobrou {}ms", x/1000000);
if(x>0){
this_thread::sleep_for(chrono::microseconds(x/1000));
}
}
_GINFO("Saindo do loop do jogo.");
}

void gpp_world::update(decimal dt){
if(hstate.load()>GWF_ACTIVE)return;
internalUpdate(dt);
}

void gpp_world::internalUpdate(float dt) {
    // Atualiza a simulação do mundo com o tempo decorrido dt
profiler_snap();
vector<RigidBody*> hbodies;
vector<CollisionInfo*> hcollisions;
hprops->advance(dt);
selectAndPrepareBodies(hbodies);
integrateBodies(hbodies);
broadPhase(hcollisions);
narrowPhase(hcollisions);
solveCollisions();
dispatchCollisions();
}

void gpp_world::selectAndPrepareBodies(vector<RigidBody*>& hbodies) {
    // Seleciona e prepara os corpos rígidos para a simulação
profiler_snap();
for(auto& it : bodies){
//it->onLoop();
if(it->handler->onLoop) it->handler->onLoop(it);
if(it->handler->preStep){
if(it->handler->preStep(it, hprops)){
hbodies.push_back(it);
}
}
}
}

void gpp_world::integrateBodies(vector<RigidBody*>& hbodies) {
    // Integra os corpos rígidos ao longo do tempo
profiler_snap();
for(auto& it : hbodies){
if(it->handler->step){
it->handler->step(it, hprops->timeStep);
}
}
}

void gpp_world::broadPhase(vector<CollisionInfo*>& collisions) {
    // Fase de broad-phase para detecção inicial de colisões
profiler_snap();
hgeo->broadPhase(collisions);
}

void gpp_world::narrowPhase(vector<CollisionInfo*>& hcollisions) {
    // Fase de narrow-phase para detecção precisa de colisões
profiler_snap();
for(auto& c : hcollisions){
bool del=true;
auto it=collisions.find(c->getId());
if(CollisionDispatcher::dispatch(c->r1->hbody, c->r2->hbody, c)){
if(it==collisions.end()){
c->type=COL_BEGIN;
c->frames=hprops->currentFrame;
collisions.insert(make_pair(c->id, c));
del=false;
}
else{
it->second->type=((it->second->type==COL_END) ? COL_BEGIN : COL_CONTINUOUS);
it->second->frames=hprops->currentFrame;
it->second->update(c);
}
}
else{
if(it!=collisions.end()){
it->second->type=COL_END;
it->second->frames=0;
}
}
if(del){
delete c;
}
}
}

void gpp_world::solveCollisions() {
profiler_snap();
    // Resolve as colisões detectadas
for(auto it=collisions.begin(); it!=collisions.end(); ++it){
CollisionInfo* c=it->second;
if(c->type==COL_END)continue;
solvePair(c);
}
}

void gpp_world::dispatchCollisions(){
profiler_snap();
vector<uint64> hkeys;
for(auto it=collisions.begin(); it!=collisions.end(); ++it){
CollisionInfo* info=it->second;
if(info->type!=COL_END){
if(hprops->currentFrame>info->frames){
info->type=COL_END;
info->frames=0;
}
}

FN_BODY3 fn1, fn2;
switch(info->type){
case COL_BEGIN:{
//info->r1->beginContact(info->r2);
//info->r2->beginContact(info->r1);
fn1=info->r1->handler->beginContact;
fn2=info->r1->handler->endContact;
break;
}
case COL_CONTINUOUS:{
//info->r1->updateContact(info->r2);
//info->r2->updateContact(info->r1);
fn1=info->r1->handler->updateContact;
fn2=info->r1->handler->endContact;

break;
}
case COL_END:{
if(info->frames==0){
//info->r1->endContact(info->r2);
//info->r2->endContact(info->r1);
fn1=info->r1->handler->endContact;
fn2=info->r1->handler->endContact;
}
info->frames++;
if(info->frames>3){
hkeys.push_back(info->id);
}
break;
}
}

if(fn1){
fn1(info->r1, info->r2);
}

if(fn2){
fn2(info->r2, info->r1);
}

}

for(auto& it : hkeys){
auto c=collisions.find(it);
if(c!=collisions.end()){
delete c->second;
collisions.erase(it);
}
}
}

void gpp_world::solvePair(CollisionInfo* info) {
    // Resolve uma única colisão entre dois corpos
profiler_snap();
RigidBody* r1=info->r1;
RigidBody* r2=info->r2;
decimal totalMass= r1->invMass+ r2->invMass;
// Separate them out using projection
vector3d ts1=- ( info-> normal * info-> depth *( r1->invMass / totalMass ));
vector3d ts2=( info-> normal * info-> depth *( r2->invMass / totalMass ));

vector3d pt1= info->point + r1->getPosition();
vector3d pt2= info->point - r2->getPosition();

vector3d angVelocityA= vector3d :: cross( r1->angularVelocity , pt1 );
vector3d angVelocityB= vector3d::cross( r2->angularVelocity , pt2 );

 vector3d fullVelocityA= r1->velocity + angVelocityA;
 vector3d fullVelocityB= r2-> velocity + angVelocityB;

vector3d vr=fullVelocityB - fullVelocityA;
 decimal impulseForce= vector3d::dot ( vr , info-> normal );

if(impulseForce<0.0f)return;
 // now to work out the effect of inertia ....

 vector3d inertiaA= vector3d::cross(r1->invInertia *  vector3d::cross( pt1 , info-> normal ) , pt1 )+numeric_limits<decimal>::epsilon();

 vector3d inertiaB= vector3d::cross(r2->invInertia *  vector3d::cross( pt2 , info-> normal ) , pt2 )+numeric_limits<decimal>::epsilon();

 decimal angularEffect= vector3d::dot ( inertiaA + inertiaB , info-> normal );

 decimal cRestitution=( r1->restitution+r2->restitution)*0.5f;

 decimal j=(-(1.0f+cRestitution)*impulseForce)/(totalMass+angularEffect);

 vector3d fullImpulse= info-> normal * j+numeric_limits<decimal>::epsilon();

r1->translate(-ts1);
r2->translate(ts2);
r1->applyLinearImpulse(-fullImpulse);
    r2->applyLinearImpulse( fullImpulse);
}
}
