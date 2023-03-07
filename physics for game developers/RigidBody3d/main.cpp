


#include<gpp/gpp.h>
#include<gpp_physics/gpp_physics.h>
using namespace gpp;
#include"rigidbody3d.h"

using namespace std;

#define _TIMESTEP 0.1f
#define _GROUND_PLANE 0.0f
float total_time=0.0f;
rigidbody3dlist bodies;

void initialize(uint32 n);
void printAll(const string& msg="");
void updateSimulation(float dt);
int main()
{
setlocale(LC_ALL, "Portuguese");
initialize(1);
printAll("Imprimindo corpos antes da simulação...");
while(total_time<100)
{
updateSimulation(_TIMESTEP);
}
printAll("Imprimindo corpos depois da simulação...");
return 0;
}

void initialize(uint32 n)
{
for(uint32 i=0; i<n; i++)
{
shared_rigidbody3d sf=make_shared<rigidbody3d>();
sf->fMass=125.0f;
sf->vPosition.x=random_float(0, 100);
sf->vPosition.y=random_float(30, 50);
sf->vPosition.z=random_float(50, 100);
sf->linearDamping=0.45f;
sf->angularDamping=0.35f;
sf->fRestitution=0.35f;
sf->fRadius=5.0f;
bodies.push_back(sf);
}
}

void printAll(const string& msg)
{
if(msg.size()>0)
_FLOG("{}", msg);
for(auto& it : bodies)
{
_FLOG("{}", it->toString());
}
}

void updateSimulation(float dt)
{
total_time+=dt;
for(auto& it : bodies)
{
it->updateBodyEuler(dt);
if(it->vPosition.z<30)
{
_FLOG("{:.0f}:{:.0f}:{:.2f}", it->vPosition.x, it->vPosition.y, it->vPosition.z);
}
vector3d n;
vector3d vr;
float  vrn;
float  J;
vector3d Fi;
 // check for collisions with ground plane
 if(it->vPosition.z <= (_GROUND_PLANE+it->fRadius)) {
   n.x = 0;
n.y=0;
   n.z = 1;
   vr = it->vVelocity;
   vrn = vr * n;
   // check to see if the particle is moving toward the ground
   if(vrn < 0.0) {
 J = -(vr*n) * (it->fRestitution + 1) * it->fMass;
 Fi = n;
 Fi *= J/_TIMESTEP;
 it->vImpactForces += Fi;
  it->vPosition.z = _GROUND_PLANE + it->fRadius;
//  it->vPosition.x = ((_GROUND_PLANE + it->radius -    it->vPreviousPosition.y) /   (it->vPosition.y - it->vPreviousPosition.y) *   (it->vPosition.x - it->vPreviousPosition.x)) +   it->vPreviousPosition.x;
it->hasCollision=true;
  }
else
{
it->hasCollision=false;
}
  }
}
}
