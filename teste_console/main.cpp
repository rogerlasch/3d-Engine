#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>

using namespace gpp;
using namespace std;

shared_ptr<gpp_world> game;
void addBody(const vector3d& position, float radius, float mass, float linear_damping, float restitution);
void initWorld();
int main()
{
/*
game=make_shared<gpp_world>();
game->setGravity({0,0,-9.8f});
_FLOG("Iniciando o mundo...");
initWorld();
addBody({50,50, 15.0f}, 5.0f, 25.0f, 0.12f, 0.55f);
_FLOG("Iniciado!");
for(uint32 i=0; i<500; i++)
{
game->update(0.1f);
}
_FLOG("Fim da simulação.\n{}", game->toString());
*/
box3d b1({50,50,50}, {10,10,10});
box3d b2({0,0,-10}, {100,100,10});
CollisionInfo info;
while(!collision3d::boxBox(&b1, &b2, &info))
{
b1.min.z--;
}
_FLOG("Colisão detectada!\nDepth:{}\nPoint:{}\nNormal:{}\n", info.depth, info.point.toString(), info.normal.toString());
_FLOG("Position::{}", b1.min.toString());
vector3d n={0,0,1};
n=0.1*n;
_FLOG("{}", n.toString());
    return 0;
}

void addBody(const vector3d& position, float radius, float mass, float linear_damping, float restitution)
{
static uint32 x=0;
x++;
RigidBody* rb=new RigidBody();
sphere3d* sp=new sphere3d(position, radius);
initRigidBody(rb, sp, mass);
rb->setLinearDamping(linear_damping);
rb->setRestitution(restitution);
rb->setIndex({x, 1});
rb->setPosition(position);
game->addBody(rb);
//game->setBodyDebug({x, 1});
}

void initWorld()
{
game->setMin({0,0,0});
game->setMax({1000, 1000, 1000});
vector<box3d*> boxs={
new box3d({0,0,-10}, {1000, 1000, 10}),
new box3d({0,0,0}, {0,1000, 1000}),
new box3d({0,1000,0}, {1000,000, 1000}),
new box3d({1000,1000,0}, {0,0,0}),
new box3d({0,0,0}, {1000, 0, 0}),
};
for(uint32 i=0; i<boxs.size(); i++)
{
RigidBody* rb=new RigidBody();
initRigidBody(rb, boxs[i], 0.0f);
rb->setIndex({i+200, 1});
rb->setPosition(boxs[i]->GetCenter());
rb->setRestitution(0.0f);
game->addBody(rb);
}
}
