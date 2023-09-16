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
winfo.flags=GWF_GRAVITY;
winfo.min={0,0,-10000.00f};
winfo.max={1000000, 1000000, 1000000};
winfo.gravity={0.0f, 0.0f, -9.81f};
world=make_shared<gpp_world>(&winfo);
RigidBody* ground=createBox(winfo.min, {winfo.max.x, winfo.max.y, fabs(winfo.min.z)});
ground->setName("Chão");
ground->mass=0.0f;
world->addBody(ground);
RigidBody* obs=createBox({10.0f, 10.0f, 0.f}, {50.0f, 50.0f, 10.0f});
obs->mass=0.0f;
obs->setName("Terreno elevado");
world->addBody(obs);

RigidBody* rb=createSphere({10.0f, 10.0f, 50.0f}, 2.5f);
rb->setName("Esfera dinâmica");
world->addBody(rb);
rb->mass=5.0f;
rb->restitution=0.25f;
_GINFO("{}", world->toString());
bool done=false;
for(uint32 i=0; i<2500; i++)
{
world->update(0.2f);
displayInfos(rb);
if(done==false){
if((rb->position.z<=12.60)&&(rb->velocity.length()<2.0f)){
done=true;
world->info->gravity.x=35.0f;
_GINFO("Vento ativado!");
}
}
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
stringstream ss;
if(dir.x<0) ss<<"o";
else if(dir.x>0) ss<<"l";
if(dir.y<0) ss<<"s";
else if(dir.y>0) ss<<"n";
if(dir.z<0) ss<<"b";
else if(dir.z>0) ss<<"c";
return ss.str();
}
