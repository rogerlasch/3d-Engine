


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
bool isColliding(shared_rigidbody3d& r1, shared_rigidbody3d& r2, bcollisioninfo* info);
void calcVelocitys(shared_rigidbody3d& r1, shared_rigidbody3d& r2, bcollisioninfo* info);
void solveCollision(shared_rigidbody3d& r1, shared_rigidbody3d& r2, bcollisioninfo* info);
int main()
{
setlocale(LC_ALL, "Portuguese");
profiler_start();
log_create("main_log.txt", LG_ALL);
log_set_default("main_log.txt");
initialize(1);
printAll("Imprimindo corpos antes da simulação...");
while(total_time<100)
{
updateSimulation(_TIMESTEP);
if(bodies[0]->position.z<-1000)
{
break;
}
}
printAll("Imprimindo corpos depois da simulação...");
profiler_dump("profiler.txt");
return 0;
}

void initialize(uint32 n)
{
shared_rigidbody3d r1=make_shared<rigidbody3d>();
r1->position={50,50,1000};
r1->radius=10.0f;
r1->mass=250.0f;
r1->inverseMass=1/r1->mass;
r1->restitution=0.99f;
r1->calcInertia();
shared_rigidbody3d r2=make_shared<rigidbody3d>();
r2->position={50,50,-50.0f};
r2->radius=50.0f;
r2->calcInertia();
bodies.push_back(r1);
bodies.push_back(r2);
}

void printAll(const string& msg)
{
if(msg.size()>0)
_GINFO("{}", msg);
for(auto& it : bodies)
{
_GINFO("{}", it->toString());
}
}

void updateSimulation(float dt)
{
profiler_snap();
total_time+=dt;
for(auto& it : bodies)
{
if(it->mass>0.0f)
{
//if(it->position.z<15)
{
_GINFO("{:.2f}", it->position.z);
}
it->calcLoads();
it->updateBodyVerlet(dt);
}
if((bodies.size()>1)&&(it->mass>0.0f))
{
for(auto& it2 : bodies)
{
if(&it==&it2)
{
continue;
}
bcollisioninfo info;
if(isColliding(it, it2, &info))
{
shared_rigidbody3d r1, r2;
if(it->mass<=0.0f)
{
r1=it2;
r2=it;
}
else
{
r1=it;
r2=it2;
}
solveCollision(r1, r2, &info);
}
}
}
}
}

bool isColliding(shared_rigidbody3d& r1, shared_rigidbody3d& r2, bcollisioninfo* info)
{
vector3d vPoint=r1->position-r2->position;
float sqRadius=r1->radius+r2->radius;
sqRadius*=sqRadius;
if((vPoint*vPoint)>sqRadius) return false;
float distance=sqrt(vPoint*vPoint);
vector3d vNormal=vPoint/distance;
vector3d vCollisionPoint=r2->position+r2->radius*vNormal;
info->vNormal=vNormal;
info->vPoint=vCollisionPoint;
info->depth=(r1->radius+r2->radius)-distance;
return true;
}

void calcVelocitys(shared_rigidbody3d& r1, shared_rigidbody3d& r2, bcollisioninfo* info)
{
vector3d vr;
vector3d vt;
vector3d vel1;
vector3d vel2;
if(r1->mass>0)
{
vel1=r1->linearVelocity;
}
else
{
vel1={0,0,0};
}
if(r2->mass>0)
{
vel2=r2->linearVelocity;
}
else
{
vel2={0,0,0};
}
float d1=vel1*vel1;
float d2=vel2*vel2;
if((d1!=0)&&(d2!=0))
{
vr=vel1-vel2;
}
else
{
vr=((d1!=0.0f) ? vel1 : vel2);
}
info->vRelativeVelocity=vr;
info->vTangent=((info->vNormal^info->vRelativeVelocity)^info->vNormal).normalize();
}

void solveCollision(shared_rigidbody3d& r1, shared_rigidbody3d& r2, bcollisioninfo* info)
{
calcVelocitys(r1, r2, info);
float vrn=info->vRelativeVelocity*info->vNormal;
_GINFO("vv:{}", r1->linearVelocity.length());
    float restitution=r1->restitution;
    float j = (1 + restitution) * (info->vRelativeVelocity* info->vNormal) / (1 /
r1->mass);
    vector3d impulse = j * info->vNormal;
vector3d correction = info->vNormal * info->depth;
r1->position += correction;

// Projetar a velocidade do corpo na direção da normal da colisão
//vector3d velAlongNormal = info->vNormal * (r1->vVelocity * info->vNormal);
// Remover a projeção da velocidade
//r1->vVelocity += velAlongNormal;
r1->linearVelocity += impulse / r1->mass;
r1->linearMomentum = (r1->mass * r1->linearVelocity);
r1->linearAcceleration = (-9.81f*r1->mass)+(impulse*r1->mass);
}

