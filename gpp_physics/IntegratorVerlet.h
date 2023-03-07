

/**
*Classe que far� a integra��o num�rica na nossa simula��o.
*O m�todo dever� calcular as velocidades lineares e angulares, bem como atualizar a orienta��o aplicar torque, e etc...
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
