

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
    float depth = ((data->depth==0) ? 1 : data->depth);;
    vector3d contact_point = data->point;

    // correção de penetração
    vector3d correction = normal * depth / (r1->mass + r2->mass);
    if (r1->mass > 0)
    {
        r1->position -= correction * r1->mass;
        r1->translate(-correction * r1->mass);
    }
    if (r2->mass > 0)
    {
        r2->position += correction * r2->mass;
        r2->translate(correction * r2->mass);
    }

    // cálculo da velocidade relativa
    vector3d relative_velocity = r2->linearVelocity - r1->linearVelocity;
    float velocity_along_normal = vector3d::dotProduct(relative_velocity, normal);

    // se a velocidade relativa já é menor que zero, então eles já estão se afastando
    if (velocity_along_normal < 0)
        return;

    // cálculo do coeficiente de restituição
    float restitution = std::min(r1->restitution, r2->restitution);

    // cálculo da força de impacto
    float impulse_scalar = (1 + restitution) * velocity_along_normal;
    impulse_scalar /= r1->mass + r2->mass;

    // aplicação da força de impacto
    vector3d impulse = normal * impulse_scalar;
    if (r1->mass > 0)
        r1->linearVelocity -= impulse * r1->mass;
    if (r2->mass > 0)
        r2->linearVelocity += impulse * r2->mass;
}
}
