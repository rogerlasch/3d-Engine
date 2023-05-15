

#include"gpp_physics.h"
#include"CollisionSolver.h"

using namespace std;

namespace gpp
{

void CollisionSolver::solve(CollisionList& collisions)
{
for(auto it=collisions.begin(); it!=collisions.end(); ++it)
{
solvePair(it->b1, it->b2, &(*it));
}
}
void CollisionSolver::solvePair(RigidBody* r1, RigidBody* r2, CollisionInfo*
info)
{
    // Verifica se ambos corpos são estáticos
//Se os 2 corpos tiverem maça = 0, retorne porque não á nada a fazer.
    if (r1->mass <= 0 && r2->mass <= 0)
        return;
//Calcula a velocidade relativa entre os corpos...
vector3d v1=r1->linearVelocity+r1->angularVelocity^info->point;
vector3d v2=r2->linearVelocity+r2->angularVelocity^info->point;
vector3d relativeVelocity=v1-v2;
//Calcula a normal da velocidade relativa...
vector3d rvn=relativeVelocity*info->normal;
//Não sei bem o que as duas próximas linhas fazem...
vector3d pt1=info->point-r1->position;
vector3d pt2=info->point-r2->position;
//Calcula a força de impulso a ser aplicado
float j = (-(1+r1->restitution) * (relativeVelocity *
info->normal)) /
((1/r1->mass + 1/r2->mass) +
(info->normal * ( ( (pt1 ^
 info->normal) *
 r1->inverseInertiaTensor )^pt1) ) +
(info->normal * ( ( (pt2 ^
 info->normal) *
r2->inverseInertiaTensor )^pt2) ) );
//Calcula a translação necessária para resolver a penetração, caso info->depth seja maior que 0
vector3d backStep=((info->depth>0) ? (info->depth*info->normal) : vector3d());
//Aplica o impulso em r1
if(r1->mass>0)
{
vector3d impulse = j * info->normal;
r1->linearVelocity += (1 / r1->mass) * impulse;
r1->angularVelocity += r1->inverseInertiaTensor * (pt1 ^ impulse);
r1->translate(backStep);
}
//Aplica o impulso em r2
if(r2->mass>0)
{
vector3d impulse = -j * info->normal;
r2->linearVelocity += (1 / r2->mass) * impulse;
r2->angularVelocity += r2->inverseInertiaTensor * (pt2 ^ impulse);
r2->translate(vector3d::inverse(backStep));
}
}
}
