#include <iostream>
#include<sstream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include"g_start.h"
#include"testes.h"

using namespace gpp;
using namespace std;

void collisioncallback(shared_collisionrow& row);
int main()
{
G_STARTER hstart;
WorldCallbacks hcalls;
hcalls.createDefault();
hcalls.hcall=collisioncallback;
WorldInfo winfo;
winfo.flags=0;
winfo.flags=GWF_AIRDENCITY;
winfo.airDencity=1.223f;
winfo.min={0,0,-10000.00f};
winfo.max={1000000, 1000000, 1000000};
winfo.gravity={0.0f, 0.0f, -9.81f};
game=make_shared<gpp_world>(&winfo, &hcalls);
RigidBody* ground=createBox(winfo.min, {winfo.max.x, winfo.max.y, fabs(winfo.min.z)});
ground->setName("Chão");
ground->mass=0.0f;
game->addBody(ground);

RigidBody* r1=createSphere({0,0,5.0f}, 2.5f);
r1->setName("r1");
r1->mass=35.0f;
r1->restitution=0.1f;
r1->airDragCoefficient=0.35f;
game->addBody(r1);

/*
RigidBody* r2=createBox({50,45,0}, {10,10,10});
r2->setName("r2");
r2->mass=50.0f;
r2->restitution=0.5f;
r2->airDragCoefficient=0.02f;
game->addBody(r2);
*/

_GINFO("{}", game->toString());
float f=32.0f;
for(uint32 i=0; i<2500; i++)
{
if((i>=15)&&(i<=33)){
vector3d dir={1,0,0};
f*=2.0f;
dir=dir*(f*0.20f);
r1->applyForce(dir);
_GINFO("Impulso de {:.2f} Newtons aplicado!", dir.x);
}
game->update(0.2f);
displayInfos(r1);
}
_GINFO("Fim da simulação...\n{}", game->toString());
/*
float radius=2.5f;
vector3d velocity={30,0,0};
float projectedArea=GPP_PI*(radius*radius);
float airDencity=1.2231f;
float airDragCoefficient=0.30f;
vector3d vdrag=fCalculateAirDrag(velocity, projectedArea, airDencity, airDragCoefficient);
cout<<vdrag<<endl;
*/
return 0;
}

void collisioncallback(shared_collisionrow& row){
for(uint32 i=0; i<row->size(); i++){
auto it=row->at(i);
if(row->at(i)->r2->getGeometricShape()->getGeometricType()==GTYPE_CAPSULE){
_GINFO("Normal: {:.2f}:{:.2f}:{:.2f}", it->r1->velocity.x, it->r1->velocity.y, it->r1->velocity.z);
}
}
}
