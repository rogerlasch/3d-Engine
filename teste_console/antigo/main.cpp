#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>

using namespace gpp;
using namespace std;

vector3d calculateFriction(const vector3d& velocity, float frictionCoefficient);
int main()
{
sphere3d s({50, 50, 500}, 25);
box3d b({-10,-10,-10}, {1000000,1000000,10});
RigidBody rb, ground;
initRigidBody(&rb, &s, 15);
initRigidBody(&ground, &b, 0.0f);
vector3d gravity={0.0f,0,-9.8f};
rb.setName("rb");
ground.setName("Chão");
ground.setRestitution(0.0f);
rb.setRestitution(0.20f);
rb.setLinearDamping(0.30f);
rb.setPosition(s.GetCenter());
ground.setPosition(b.GetCenter());
int x=0;
IntegratorVerlet vt;
CollisionSolver cs;
bool showb=false;
CollisionData info;
while(x<2500)
{
x++;
vector3d forse=gravity+calculateFriction(rb.getLinearVelocity(), 0.35);
rb.setForce(forse);
if(showb)
{
_FLOG("{}", rb.getPosition().z);
}
vt.integrate(&rb, 0.2f);
if(collision3d::sphereBox(&s, &b, &info))
{
showb=true;
cs.solve(&rb, &ground, &info);
}
if(rb.getPosition().z<-10) break;
}
_FLOG("RB final:\n{}", rb.toString());
    return 0;
}

vector3d calculateFriction(const vector3d& velocity, float frictionCoefficient)
{
    vector3d friction = -frictionCoefficient * vector3d::normalize( velocity);
    if (velocity.length() < friction.length())
        friction = vector3d::inverse(velocity);
    return friction;
}
