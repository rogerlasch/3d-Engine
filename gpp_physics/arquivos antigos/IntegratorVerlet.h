

/**
*Classe que fará a integração numérica na nossa simulação.
*O método deverá calcular as velocidades lineares e angulares, bem como atualizar a orientação aplicar torque, e etc...
**/
#ifndef INTEGRATORVERLET_H
#define INTEGRATORVERLET_H

namespace gpp
{

class IntegratorVerlet
{
public:
void integrate(RigidBodyList& bodies, float delta_time);
void integrate(RigidBody* rb, float delta_time);
};
}
#endif
