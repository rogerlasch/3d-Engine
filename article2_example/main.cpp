#include <iostream>
#include<sstream>
#include<stack>
#include<unordered_set>
#include <deque>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>
#include"g_start.h"

using namespace gpp;
using namespace std;

vector<shared_ptr<iRigidBody>> hbodies;
shared_ptr<gpp_world> world;
RigidBody* createRandomBody();
RigidBody* createSphere(const vector3d& center, float radius);
RigidBody* createBox(const vector3d& min, const vector3d& mes);
void displayInfos(RigidBody* rb);
string getDirectionDescription(const vector3d& dir) ;

int main()
{
G_STARTER hstart;
WorldInfo winfo;
winfo.flags=GWF_GRAVITY|GWF_AIRDENCITY;;
winfo.min={0,0,-10000.00f};
winfo.max={1000000, 1000000, 1000000};
winfo.gravity={0, 0.0f, -9.81f};
winfo.airDencity=1.223f;
world=make_shared<gpp_world>(&winfo);
RigidBody* b1=createBox(winfo.min, {winfo.max.x, winfo.max.y,
fabs(winfo.min.z)+10});
b1->setName("Chão");
b1->mass=0.0f;
world->addBody(b1);

RigidBody* rb=createSphere({220.0f, 10.0f, 100.0f}, 25.0f);
rb->restitution=0.3f;
rb->airDragCoefficient=0.25f;
rb->mass=1000.0f;
rb->setName("Esfera dinâmica");
world->addBody(rb);
_GINFO("{}", world->toString());
bool done=false;
uint32 x=0;
vector3d force={0, 0, 1024*16};
float minDist=((sphere3d*)rb->getGeometricShape())->radius+0.2f;
_GINFO("Simulação iniciada!");
for(uint32 i=0; i<2500; i++)
{
if(done==false){
box3d* b=(box3d*)b1->getGeometricShape();
if((fabs(rb->position.z-b->max.z)<=minDist)&&(rb->velocity.length()<1.0f)){
_GINFO("Impulso was started");
done=true;
rb->applyForce(force);
x++;
}
}
else{
if(x<10){
x++;
force.z+=128.0f;
rb->applyForce(force);
if(x==10){
_GINFO("Último impulso aplicado");
_GINFO("impl:{:.2f}", force.z);
x=0;
done=false;
if(force.z<1024*25) force*=2;
}
}
}
world->update(0.02f);
displayInfos(rb);

}
_GINFO("Fim da simulação...\n{}", world->toString());
return 0;
}

RigidBody* createRandomBody()
{
static uint32 x=0;
x++;
shared_ptr<RigidBody> rb=make_shared<RigidBody>();
rb->setIndex({x, x+1});
rb->mass=random_float(1.0f, 330.0f);
rb->restitution=random_float(0.01f, 1.0f);
hbodies.push_back(rb);
return rb.get();
}

RigidBody* createSphere(const vector3d& center, float radius){
RigidBody* rb=createRandomBody();
sphere3d* sp=new sphere3d();
sp->center=center;
sp->radius=radius;
rb->setGeometricShape(sp);
return rb;
}

RigidBody* createBox(const vector3d& min, const vector3d& mes){
RigidBody* rb=createRandomBody();
box3d* b=new box3d(min, mes);
rb->setGeometricShape(b);
return rb;
}

void displayInfos(RigidBody* rb) {
vector3d vdir=rb->position-rb->getGeometricShape()->getLastPosition();
vdir.normalize();
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Tempo: "<<world->info->currenttime<<"S, Direção: "<<getDirectionDescription(vdir)<<endl;
ss<<"Posição atual: "<<rb->position<<", Velocidade: "<<rb->velocity.length()<<"M/S"<<endl;
_GINFO("{}", ss.str());
}

string getDirectionDescription(const vector3d& dir) {
vector3d vdir;
for(uint32 i=0; i<3; i++){
float eps=dir[i];
if((eps>-0.0001)&&(eps<0.0001)) vdir[i]=0.0f;
else vdir[i]=eps;
}
stringstream ss;
if(vdir.x<0) ss<<"o";
else if(vdir.x>0) ss<<"l";
if(vdir.y<0) ss<<"s";
else if(vdir.y>0) ss<<"n";
if(vdir.z<0) ss<<"b";
else if(vdir.z>0) ss<<"c";
return ss.str();
}
