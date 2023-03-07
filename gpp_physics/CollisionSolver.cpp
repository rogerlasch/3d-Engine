

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
    if (r1->mass <= 0 && r2->mass <= 0)
<<<<<<< HEAD
        return;
vector3d v1=r1->linearVelocity+r1->angularVelocity^info->point;
vector3d v2=r2->linearVelocity+r2->angularVelocity^info->point;
vector3d relativeVelocity=v1-v2;
vector3d rvn=relativeVelocity*info->normal;
vector3d pt1=info->point-r1->position;
vector3d pt2=info->point-r2->position;
float j = (-(1+r1->restitution) * (relativeVelocity *
info->normal)) /
((1/r1->mass + 1/r2->mass) +
(info->normal * ( ( (pt1 ^
 info->normal) *
 r1->inverseInertiaTensor )^pt1) ) +
(info->normal * ( ( (pt2 ^
 info->normal) *
r2->inverseInertiaTensor )^pt2) ) );

vector3d backStep=((info->depth>0) ? (info->depth*info->normal) : vector3d());

if(r1->mass>0)
{
}
if(r2->mass>0)
{
}
}
}
