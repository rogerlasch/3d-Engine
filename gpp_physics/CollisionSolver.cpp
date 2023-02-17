

#include"gpp_physics.h"
#include"CollisionSolver.h"

using namespace std;

namespace gpp
{
void CollisionSolver::solve(RigidBody* r1, RigidBody* r2, CollisionData* data)
{
    if (r1->mass <= 0 && r2->mass <= 0)
        return; // ambos são estáticos, não há nada a fazer

    vector3d normal = data->normal;
    float depth = ((data->depth<=0.1f) ? 0.0f : data->depth);
    vector3d contactPoint = data->point;

    // Calcular a velocidade relativa entre os corpos rígidos
    vector3d relativeVelocity = r2->linearVelocity + vector3d::crossProduct(r2->angularVelocity, contactPoint - r2->position) -
                                r1->linearVelocity - vector3d::crossProduct(r1->angularVelocity, contactPoint - r1->position);

    // Calcular a direção e magnitude do impulso
    float impulseMagnitude = (-(1 + r1->restitution) * vector3d::dotProduct(relativeVelocity, normal)) /
                            (r1->inverseMass + r2->inverseMass +
                             vector3d::dotProduct(normal, vector3d::crossProduct(r1->inverseInertiaTensor *
                                                 vector3d::crossProduct(contactPoint - r1->position, normal),
                                                 contactPoint - r1->position) +
                                                 vector3d::crossProduct(r2->inverseInertiaTensor *
                                                 vector3d::crossProduct(contactPoint - r2->position, normal),
                                                 contactPoint - r2->position)));
    vector3d impulse = impulseMagnitude * normal;
    // Calcular o momento linear
    vector3d momentum = vector3d::crossProduct(contactPoint - r1->position, impulse);

// Calcular o torque
vector3d torque1 = vector3d::crossProduct(contactPoint - r1->position, normal * impulseMagnitude);
vector3d torque2 = vector3d::crossProduct(contactPoint - r2->position, normal * impulseMagnitude);

    // Atualizar o momento linear dos corpos rígidos
    r1->linearMomentum -= impulse;
    r1->angularMomentum -= momentum;
r1->angularMomentum -= torque1;
    r2->linearMomentum += impulse;
    r2->angularMomentum += momentum;
r2->angularMomentum += torque2;
    // correção de penetração
if(depth>0)
{
_FLOG("Depth: {}", depth);
    vector3d correction = normal * depth / (r1->inverseMass + r2->inverseMass);
    if (r1->mass > 0)
    {
_FLOG("Correction: {}", correction.toString());
        r1->position -= correction * r1->inverseMass;
        r1->translate(-(correction * r1->inverseMass));
    }
    if (r2->mass > 0)
    {
        r2->position += correction * r2->inverseMass;
        r2->translate(correction * r2->inverseMass);
}
}
}
}
