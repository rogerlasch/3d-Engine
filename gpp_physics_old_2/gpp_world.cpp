


#include<sstream>
#include<algorithm>
#include<gpp/gpp.h>
#include"ClockGame.h"
#include"gpp_world.h"
#include"ArrayHelper.h"

using namespace std;

namespace gpp{

gpp_world::gpp_world(const vector3d& min, const vector3d& alfSize, uint32 flags){
for(uint32 i=0; i<3; i++){
if(fabs(alfSize[i])<100.0f){
throw runtime_error("O tamanho de mundo é pequeno de mais! É recomendável usar 100 unidades em cada eixo.");
}
}

gsortaxis=0;
hstate=new StateManager();
//hclock=new ClockGame();

this->min=min;
this->max=this->min+alfSize;
hstate->setState(GW_DEFAULT);
this->hstate->replaceFlags(flags);
setLinearDamping(0.3f);
setAngularDamping(0.7f);
setGravity({0.0f, 0.0f, -9.81f});
currentTime=0.0f;
timestep=0.2f;
geography=new octree();
float alf=0.0f;
for(uint32 i=0; i<3; i++){
float f=fabs(max[i]-min[i]);
if(f>alf)alf=f;
}
//geography->create(min, 8, 128, alf);
}

gpp_world::~gpp_world(){
removeAllObjects();
delete hstate;
delete geography;
geography=NULL;
}

string gpp_world::toString()const {
        stringstream ss;
        ss << fixed;
        ss.precision(2);
        ss << "Exibindo mundo..." << endl;
ss<<"CurrentTime="<<currentTime<<", TimeStep="<<timestep<<endl;
        string axis= "XYZ";
        for (uint32 i= 0; i < 3; i++) {
                ss << axis[i] << ": " << min[i] << ", " << max[i] << endl;
        }

        ss << "Linear damping: " << linearDamping << endl;
        ss << "Angular Damping: " << angularDamping << endl;
        ss << "Gravidade: " << gravity << endl;

        ss << "Usar gravidade? " << hstate->flag_contains(GWF_GRAVITY) << endl;
        ss << "Usar amortecimento ? " << hstate->flag_contains(GWF_DAMPING) << endl;
        ss << "Usar broadphase ? " << hstate->flag_contains(GWF_BROADPHASE) << endl;

ss<<safe_format("Existem um total de {} objetos dinâmicos e {} objetos estáticos no mundo.", dynamicBodies.size(), staticBodies.size())<<endl;

if(dynamicBodies.size()>0){
ss<<"Exibindo objetos dinâmicos:"<<endl;
for(auto& it : dynamicBodies){
ss<<it->toString()<<endl;
}
}

if(staticBodies.size()>0){
ss<<"Exibindo objetos estáticos:"<<endl;
for(auto& it : staticBodies){
ss<<it->toString()<<endl;
}
}
        return ss.str();
}

bool gpp_world::enable(uint32 f, bool en){
if(en){
return hstate->setFlag(f);
}
return hstate->removeFlag(en);
}

bool gpp_world::start(){
        switch (hstate->getStage()) {
        case GW_DEFAULT:
        case GW_FINISHED: {
                hstate->setState(GW_STARTING);
                function<void()> hfunc= std::bind(&gpp_world::onLoop, this);
                handle= thread(hfunc);
                hstate->waitForState([&](uint32 h)->bool {return h >= GW_RUNNING; });
                return true;
        }
}
        return false;
}

void gpp_world::stop() {
        uint32 x= hstate->getState();
        if ((x != GW_RUNNING) && (x != GW_CLOSING)) {
                return;
        }

        hstate->setState(GW_CLOSING);
        handle.join();
        hstate->setState(GW_FINISHED);

}

void gpp_world::addObject(RigidBody* rb){
if(rb->mass<=0.0f){
//geography->insert(rb);
staticBodies.push_back(rb);
}
else{
dynamicBodies.push_back(rb);
}
}

void gpp_world::removeObject(RigidBody* rb){
remove_from_array(dynamicBodies, rb);
remove_from_array(staticBodies, rb);
//geography->remove(rb);
}

    RigidBody* gpp_world::getObject(uint32 id)const{
RigidBody* rb=find_from_array(dynamicBodies, id);
if(rb==NULL){
rb=find_from_array(staticBodies, id);
}
return rb;
}

void gpp_world::removeAllObjects(){
//geography->clear();
staticBodies.clear();
dynamicBodies.clear();
//free_array_object(dynamicBodies);
//free_array_object(staticBodies);
}

void gpp_world::internalUpdate(float dt){
profiler_snap();
currentTime+=dt;
prepareBodies(dt);
integrate(dt);
if(isEnabled(GWF_BROADPHASE)){
vector<CollisionInfo> hcollisions;
internalBroadPhase(dt, hcollisions);
if(hcollisions.size()>0){
}
internalNarrowPhase(dt, hcollisions);
}
else{
basicDetectCollision(dt);
}
updateCollisions(dt);
}

void gpp_world::prepareBodies(float dt){
for(auto& it : dynamicBodies){
it->prepareBody(dt, gravity);
}
}

void gpp_world::integrate(float dt){
profiler_snap();
float eps=numeric_limits<float>::epsilon();
for(auto& it : dynamicBodies){
if(it->mass<=0.0f)continue;
//Movimento linear...

vector3d accel=it->forces*it->inverseMass;
it->velocity+=(accel*dt);
float ldamping=powf(linearDamping, dt);
it->velocity*=ldamping;

it->lastPosition=it->position;
it->position+=(it->velocity*dt);

//Orientação e movimento angular...

it->updateInertiaTensor();
it->angularVelocity+=(it->invInertia*it->torque)*dt;
float adamping=powf(angularDamping, dt);
it->angularVelocity*=adamping;

quaternion q(0.0f, (it->angularVelocity*dt)*0.5f);
it->orientation=(it->orientation+q*it->orientation);
it->orientation.normalize();

//Limpar as forças...
it->forces={0.0f, 0.0f, 0.0f};
it->torque={0.0f, 0.0f, 0.0f};
}
}

void gpp_world::internalBroadPhase(float dt, std::vector<CollisionInfo>& hcollisions){
//    geography->BroadPhase(dynamicBodies, hcollisions);
    // Sort the array on currently selected sorting axis (gsortAxis)
std::sort(dynamicBodies.begin(), dynamicBodies.end(), [&](RigidBody* r1, RigidBody*r2)->bool{return r1->position[gsortaxis]<r2->position[gsortaxis];});
    // Sweep the array for collisions
    float s[3]= { 0.0f, 0.0f, 0.0f }, s2[3]= { 0.0f, 0.0f, 0.0f }, v[3];
uint32 nsize=dynamicBodies.size();
    for (uint32 i=0; i<nsize; i++) {
vector3d m1, m2;
dynamicBodies[i]->getAabb(m1, m2);
          // Determine AABB center point
          vector3d p=0.5f*(m1+m2);
          // Update sum and sum2 for computing variance of AABB centers
          for (uint32 c= 0; c < 3; c++) {
                s[c] += p[c];
                s2[c] += p[c] * p[c];
          }
          // Test collisions against all possible overlapping AABBs following current one
          for (uint32 j= i + 1; j < nsize; j++) {
                // Stop when tested AABBs are beyond the end of current AABB
vector3d m3, m4;
dynamicBodies[j]->getAabb(m3, m4);
                if (m3[gsortaxis] > m2[gsortaxis])
                      break;
                if (aabbOverlap(m1, m2, m3, m4)) {
auto& info=hcollisions.emplace_back();
info.r1=dynamicBodies[i];
info.r2=dynamicBodies[j];
info.notify=true;
}
          }
      }
    // Compute variance (less a, for comparison unnecessary, constant factor)
        for (uint32 c= 0; c < 3; c++)
              v[c]= s2[c] - s[c] * s[c] / dynamicBodies.size();
        // Update axis sorted to be the one with greatest AABB variance
uint32 x=0;
        if (v[1] > v[0]) x= 1;
        if (v[2] > v[gsortaxis]) x= 2;
gsortaxis=x;
}

void gpp_world::internalNarrowPhase(float dt, std::vector<CollisionInfo>& hcollisions){
uint32 size=hcollisions.size();
for(auto& it : hcollisions){
if(collision3d::isColliding(it.r1, it.r2, &it)){
auto hk=collisions.find(it.getId());
if(hk==collisions.end()){
it.counter=NUMFRAMES;
collisions.insert(make_pair(it.getId(), it));
solveCollision(it.r1, it.r2, &it);
}
else{
it.counter=NUMFRAMES;
hk->second=it;
solveCollision(hk->second.r1, hk->second.r2, &hk->second);
}
}
}
}

void gpp_world::basicDetectCollision(float dt){
profiler_snap();
for(uint32 i=0; i<dynamicBodies.size(); i++){
for(uint32 i1=i+1; i1<dynamicBodies.size(); i1++){
CollisionInfo info;
if(collision3d::isColliding(dynamicBodies[i], dynamicBodies[i1], &info)){
info.counter=NUMFRAMES;
collisions.insert(make_pair(info.getId(), info));
solveCollision(info.r1, info.r2, &info);
}
}
for(uint32 i1=0; i1<staticBodies.size(); i1++){
CollisionInfo info;
if(collision3d::isColliding(dynamicBodies[i], staticBodies[i1], &info)){
info.counter=NUMFRAMES;
collisions.insert(make_pair(info.getId(), info));
solveCollision(info.r1, info.r2, &info);
}
}
}
}

void gpp_world::solveCollision(RigidBody* r1, RigidBody* r2, CollisionInfo* info){
float totalMass= r1->inverseMass+ r2->inverseMass;
// Separate them out using projection
vector3d ts1=- ( info-> normal * info-> depth *( r1->inverseMass / totalMass ));
vector3d ts2=( info-> normal * info-> depth *( r2->inverseMass / totalMass ));

vector3d pt1= info->point + r1->position;
vector3d pt2= info->point - r2->position;

vector3d angVelocityA= vector3d :: crossProduct( r1->angularVelocity , pt1 );
vector3d angVelocityB= vector3d::crossProduct( r2->angularVelocity , pt2 );

 vector3d fullVelocityA= r1->velocity + angVelocityA;
 vector3d fullVelocityB= r2-> velocity + angVelocityB;

vector3d vr=fullVelocityB - fullVelocityA;

 float impulseForce= vector3d::dotProduct ( vr , info-> normal );
 // now to work out the effect of inertia ....

 vector3d inertiaA= vector3d::crossProduct(r1->invInertia *  vector3d::crossProduct( pt1 , info-> normal ) , pt1 )+numeric_limits<float>::epsilon();

 vector3d inertiaB= vector3d::crossProduct(r2->invInertia *  vector3d::crossProduct( pt2 , info-> normal ) , pt2 )+numeric_limits<float>::epsilon();

 float angularEffect= vector3d::dotProduct ( inertiaA + inertiaB , info-> normal );

 float cRestitution=( r1->restitution+r2->restitution)*0.5f;


 float j=(-(1.0f+cRestitution)*impulseForce)/(totalMass+angularEffect);

 vector3d fullImpulse= info-> normal * j;

r1->position-=ts1;
r2->position+=ts2;
r1->applyLinearImpulse(vector3d::inverse(fullImpulse));
    r2->applyLinearImpulse( fullImpulse);

//    r1->ApplyAngularImpulse(vector3d::crossProduct(pt1 ,-fullImpulse ));
//    r2->ApplyAngularImpulse(vector3d::crossProduct(pt2 , fullImpulse ));

//Recalcular a velocidade relativa...

angVelocityA= vector3d :: crossProduct( r1->angularVelocity , pt1 );
angVelocityB= vector3d::crossProduct( r2->angularVelocity , pt2 );

fullVelocityA= r1->velocity + angVelocityA;
fullVelocityB= r2-> velocity + angVelocityB;

vr=fullVelocityB - fullVelocityA;
//Atrito...
    vector3d t = vr - (info->normal * vector3d::dotProduct( vr, info->normal));
    t.normalize();

    float jt = -vector3d::dotProduct( vr, t);
    jt /=totalMass;
    // Coulumb's law
    vector3d tangentImpulse;
float sf=(r1->staticFriction+r2->staticFriction)*0.5f;
float df=(r1->dynamicFriction+r2->dynamicFriction)*0.5f;

    if(std::abs( jt) < j * sf){
      tangentImpulse = t * jt;
//tangentImpulse=t*j;
}
    else
      tangentImpulse = t * j * df;
    // Apply friction impulse
r1->applyLinearImpulse( -tangentImpulse);
r2->applyLinearImpulse(  tangentImpulse);
if((r1->velocity*r1->velocity)<0.1f){
r1->setVelocity({0.0f, 0.0f, 0.0f});
r1->setAngularVelocity({0.0f, 0.0f, 0.0f});
}
if((r2->velocity*r2->velocity)<0.1f){
r2->setVelocity({0.0f, 0.0f, 0.0f});
r2->setAngularVelocity({0.0f, 0.0f, 0.0f});
}
}

void gpp_world::updateCollisions(float dt){
for(auto it=collisions.begin(); it!=collisions.end(); ++it){
if(it->second.counter==NUMFRAMES){
if(it->second.notify){
it->second.r1->beginContact(it->second.r2, &it->second);
it->second.r2->beginContact(it->second.r1, &it->second);
}
it->second.counter--;
}
else{
it->second.counter--;
}
if(it->second.counter<=0){
if(it->second.notify){
it->second.r1->endContact(it->second.r2, &it->second);
it->second.r2->endContact(it->second.r1, &it->second);
}
it=collisions.erase(it);
}
}
}

void gpp_world::onLoop() {
    hstate->setState(GW_RUNNING);
    bool done= false;
    while (!done) {
float ft=getTimeStep()*1000.0f;
int64 frameTime=(int64)ft;
        switch (hstate->getState()) {
            case GW_RUNNING: {
int64 start=get_timestamp_ms();
                internalUpdate(getTimeStep());
int64 end=get_timestamp_ms();
int64 diff=(end-start);
int64 diff2=frameTime-diff;
if(diff2>0){
wait_ms(diff2);
}
else{
_GINFO("Faltando tempo para processar! O atraso foi de {} ms", abs(diff2));
}
                break;
            }
            case GW_CLOSING: {
                done= true;
                break;
            }
        }
    }
}
}
