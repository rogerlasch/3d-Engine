
#define GPP_DOUBLE_PRECISION
#include<execution>
#include <iostream>
#include <gpp/gpp.h>
#include<gpp_physics/gpp_physics.h>
#include"g_start.h"
#include"BodyManager.h"

using namespace std;
using namespace gpp;

int main() {
try{
G_STARTER gst;

vector3d vmin={-10000,-10000,-10000};
vector3d vmax={10000,10000,10000};
gpp_world game(AABB(vmin, vmax));

RigidBody* s=createBodySphere({0,0,2.5f}, 2.5f, {0,0,0},50.0f);
s->setId(1);
s->registerCollisionCallback(COL_END, [&](RigidBody* r1, RigidBody* r2, CollisionInfo* info){
if(r2->getId()==2){
logger::info("vel={}", r1->getLinearVelocity().toString());
}
});

//RigidBody* s2=createBodyCapsule({50, 0, -25}, {50, 0, 25}, 25.0f, {0,0,0}, 0.0f);
//s2->rotate(quaternion::fromEuler(-90, 0, 0));
//s2->translate({0,0,-15});
//s2->setId(2);

vector3d alf=(vmax-vmin)*0.5f;
alf.z*=0.5f;
RigidBody* ground=createBodyBox({0,0,-alf.z}, alf, {0,0,0}, 0.0f);
ground->setId(3);

game.addRigidBody(ground);
//game.addRigidBody(s2);
game.addRigidBody(s);

/*
for(uint32 i=0; i<10000; i++){
vector3d pos=vector3d::random(vmin.x, vmax.x, vmin.y, vmax.y, 0, vmax.z);
RigidBody* rb=createBodySphere(pos, 2.5f);
game.addRigidBody(rb);
}
*/

s->applyForce({9.81f*s->getMass(), 0, 0});
for(uint32 i=0; i<12500; i++){
if(s->getTransform()->getPosition().x<5.0f){
s->applyForce({9.81f*s->getMass(), 0, 0});
}
game.update(0.2f);
vector3d pos=s->getTransform()->getPosition();
logger::info("{:.2f}, {:.2f}", pos.x, pos.z);
}

game.printState();
/*
Sphere3d* s=new Sphere3d(2.5f);
s->translate({0,0,2.5f});

Sphere3d* c=new Sphere3d(25.0f);
c->translate({50, 0, -15});

RigidBody r1(NULL, 0, 5.0f, s);
RigidBody r2(NULL, BF_STATIC, 0, c);

//logger::info("{}\n{}", s->toString(), c->toString());

for(uint32 i=0; i<250; i++){
r1.applyForce({20, 0,0});
r1.update(0.2f);

logger::info("{}", r1.getTransform()->getPosition().toString());
CollisionInfo info;
if(detectCollisionWith(&r1, &r2, &info)){
logger::info("col={}", info.toString());
info.r1->resolveCollision(info.r2, &info);
}
}

logger::info("Fim da simulação.");
logger::info("{}\n{}", r1.toString(), r2.toString());
*/
} catch(const exception& e){
logger::info("Exception:\n{}", e.what());
}
    return 0;
}
