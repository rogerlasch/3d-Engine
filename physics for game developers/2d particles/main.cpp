


#include<gpp/gpp.h>
#include<gpp_physics/gpp_physics.h>
using namespace gpp;
#include"particle2d.h"

using namespace std;

#define _FRAMECOUNT 10
#define _TIMESTEP 0.1f
float total_time=0.0f;
particle2dlist particles;
particle2dlist obstacles;

void initialize(uint32 n);
void initobstacles();
void printAll(const string& msg="");
void updateSimulation(float dt);
bool checkForCollisions(shared_particle2d& sf);
int main()
{
setlocale(LC_ALL, "Portuguese");
initialize(1);
initobstacles();
printAll("Imprimindo partículas antes da simulação...");
while(total_time<75)
{
updateSimulation(_TIMESTEP);
}
printAll("Imprimindo partículas depois da simulação...");
return 0;
}

void initialize(uint32 n)
{
/*
for(uint32 i=0; i<n; i++)
{
shared_particle2d sf=make_shared<particle2d>();
sf->fMass=125.0f;
sf->vPosition.x=random_float(0, 100);
sf->vPosition.y=random_float(30, 50);
sf->fRestitution=0.9f;
particles.push_back(sf);
}
*/
shared_particle2d sf=make_shared<particle2d>();
sf->fMass=125.0f;
sf->fRestitution=0.75f;
sf->vPosition={55.5, 100.0f, 0};
sf->fRadius=1;
particles.push_back(sf);
}

void initobstacles()
{
shared_particle2d d1=make_shared<particle2d>(0.0f, 0.0f);
d1->vPosition={50, 5, 0};
d1->fRadius=5.0f;
shared_particle2d d2=make_shared<particle2d>(0.0f, 0.0f);
d2->vPosition={60, 5, 0};
d2->fRadius=5.0f;
d1->fMass=300.0f;
d2->fMass=25.0f;
obstacles.push_back(d1);
obstacles.push_back(d2);
}

void printAll(const string& msg)
{
if(msg.size()>0)
_FLOG("{}", msg);
for(auto& it : particles)
{
_FLOG("{}", it->toString());
}
}

void updateSimulation(float dt)
{
total_time+=dt;
for(auto& it : particles)
{
it->bCollision=checkForCollisions(it);
it->calcLoads();
it->updateBodyEuler(dt);
if(it->vPosition.y<30)
{
_FLOG("{:.0f}:{}", it->vPosition.x, it->vPosition.y);
}
}
}

bool checkForCollisions(shared_particle2d& sf)
  {
vector3d n;
vector3d vr;
float  vrn;
float  J;
vector3d Fi;
bool   hasCollision = false;
 // Reset aggregate impact force
 sf->vImpactForces.x = 0;
 sf->vImpactForces.y = 0;
   // Check for collisions with obstacles
   float r;
   vector3d d;
   float s;
   for(int i=0; i<obstacles.size(); i++)
   {
 r = sf->fRadius + obstacles[i]->fRadius;
 d = sf->vPosition - obstacles[i]->vPosition;
 s = d.length() - r;
if(s <= 0.0)
{
  d.normalize();
  n = d;
  vr = sf->vVelocity - obstacles[i]->vVelocity;
  vrn = vr*n;
if(vrn < 0.0)
{
  J = -(vr*n) * (sf->fRestitution + 1) /
 (1/sf->fMass + 1/obstacles[i]->fMass)+0.001f;
  Fi = n;
  Fi *= J/_TIMESTEP;
  sf->vImpactForces += Fi;
sf->vPosition -= n*s;
hasCollision = true;
}
}
}
 // check for collisions with ground plane
 if(sf->vPosition.y <= (_GROUND_PLANE+sf->fRadius)) {
   n.x = 0;
   n.y = 1;
   vr = sf->vVelocity;
   vrn = vr * n;
   // check to see if the particle is moving toward the ground
   if(vrn < 0.0) {
 J = -(vr*n) * (sf->fRestitution + 1) * sf->fMass;
 Fi = n;
 Fi *= J/_TIMESTEP;
 sf->vImpactForces += Fi;
  sf->vPosition.y = _GROUND_PLANE + sf->fRadius;
  sf->vPosition.x = ((_GROUND_PLANE + sf->fRadius -
   sf->vPreviousPosition.y) /
  (sf->vPosition.y - sf->vPreviousPosition.y) *
  (sf->vPosition.x - sf->vPreviousPosition.x)) +
  sf->vPreviousPosition.x;
  hasCollision = true;
  }
  }
return hasCollision;
}
