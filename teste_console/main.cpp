#define _GPP_USE_DEBUG
#include<filesystem>
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
void run_test(const string& filename, int x, int bd);
void fillWorld(gpp_world* game, int32 n);
void run_to(gpp_world* game, int64 ms);

int main() {

int32 bd=40;
while(bd<=40){
filesystem::create_directory(safe_format("data/{} objetos", bd));
string filename="data/{} objetos/0{}.csv";
for(int i=0; i<5; i++){
run_test(safe_format(filename, bd, i+1), (i+1), bd);
wait_ms(5000);
}
if(bd==1)bd=5;
else bd+=5;
}
return 0;
}

void run_test(const string& filename, int x, int bd){
cout<<"Rodando teste "<<x<<", com "<<bd<<" objetos."<<endl;
    G_STARTER gst;
props.toDefault();
props.center={0,0,0};
props.radius=1024.0f*1024.0f;
gpp_world game;
game.create(props);
RigidBody* rb2=createBodyBox<RigidBody>(1, {0,0, -50}, {1024*1024,1024*1024,50});
rb2->setMass(0.0f);
hbodies.push_back(rb2);
game.addBody(rb2);
fillWorld(&game, bd);
run_to(&game, 60000);
profiler_dump(filename, PFD_NANOSECONDS);
for(auto& it : hbodies){
delete it;
}
hbodies.clear();
cout<<"Etapa "<<x<<" finalizada"<<endl;
}

void fillWorld(gpp_world* game, int32 n){
try{
float mass=250.0f;
float restitution=0.5f;
float radius=2.5f;
vector3d m1={0,0,3.0f};
vector3d m2={2000,2000,2000};

_GINFO("Criando {} objetos com as seguintes propriedades:", n);
_GINFO("Mass:{}", mass);
_GINFO("Radius:{}", radius);
_GINFO("Restitution:{}", restitution);

for(uint32 i=0; i<n; i++){
vector3d pos;
pos.x=random_float(m1.x, m2.x);
pos.y=random_float(m1.y, m2.y);
pos.z=random_float(m1.z, m2.z);
RigidBody* rb=createBodySphere<RigidBody>((i+2), pos, radius, quaternion_from_euler_angles(0,0,0));
rb->setMass(mass);
rb->setRestitution(restitution);
hbodies.push_back(rb);
game->addBody(rb);
_GINFO("Objeto {} criado", rb->id);
}
} catch(const exception& e){
_GINFO("{}", e.what());
}
}

void run_to(gpp_world* game, int64 ms){
using mClock=chrono::high_resolution_clock;

int64 frameTime=1000000000/35;
props.timeStep=1.0f/(35.0f);
mClock::time_point tps;
int64 tstart=get_timestamp_ms();
_GINFO("Iniciando simulação");
while((get_timestamp_ms()-tstart)<ms){
tps=mClock::now();
game->update(1/35.0f);
mClock::time_point tend=mClock::now();
int64 x=chrono::duration_cast<chrono::nanoseconds>(tend-tps).count();
x=frameTime-x;
//_GINFO("sobrou {}ms", x/1000000);
if(x>0){
this_thread::sleep_for(chrono::microseconds(x/1000));
}
}
}
