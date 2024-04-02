
#define _GPP_USE_DEBUG
#include <iostream>
#include<array>
#include<chrono>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include "g_start.h"
#include <windows.h>

using namespace gpp;
using namespace std;

vector<RigidBody*> hbodies;

WorldProperties props;
void fillWorld(gpp_world* game, int32 n);
void run_to(gpp_world* game, int64 ms);
int main() {
    G_STARTER gst;

props.toDefault();
props.center={0,0,0};
props.radius=1024.0f*1024.0f;
gpp_world game;
_GINFO("Criando mundo: {}", game.create(props));

RigidBody* rb2=createBodyBox<RigidBody>(1, {0,0, -50}, {1024*1024,1024*1024,50});
rb2->setMass(0.0f);
hbodies.push_back(rb2);
game.addBody(rb2);
fillWorld(&game, 50);

run_to(&game, 60000);
profiler_dump("main_profiler.txt", PFD_NANOSECONDS);
_GINFO("Fim da simulação");

_GINFO("{}", game.toString());
for(auto& it : hbodies){
_GINFO("{}", it->toString());
delete it;
}
return 0;
}

void fillWorld(gpp_world* game, int32 n){
vector3d m1={0,0,0};
vector3d m2={2500,2500,2500};
decimal r1=1.5f, r2=55.0f;
decimal minMass=1.0f, maxMass=2000.0f;

for(int32 i=0; i<n; i++){
vector3d position;
decimal mass, radius;
for(uint32 i1=0; i1<3; i1++){
position[i1]=random_float(m1[i1], m2[i1]);
}
radius=random_float(r1, r2);
mass=random_float(minMass, maxMass);
RigidBody* rb=createBodySphere<RigidBody>(i+2, position, radius);
rb->setMass(mass);
hbodies.push_back(rb);
game->addBody(rb);
}
}

void run_to(gpp_world* game, int64 ms){
using mClock=chrono::high_resolution_clock;

int64 frameTime=1000000000/35;
props.timeStep=1.0f/(35.0f);
mClock::time_point tps;
int64 tstart=get_timestamp_ms();
while((get_timestamp_ms()-tstart)<ms){
tps=mClock::now();
game->update(1/35.0f);
mClock::time_point tend=mClock::now();
int64 x=chrono::duration_cast<chrono::nanoseconds>(tend-tps).count();
x=frameTime-x;
_GINFO("sobrou {}ms", x/1000000);
if(x>0){
this_thread::sleep_for(chrono::microseconds(x/1000));
}
}
}
