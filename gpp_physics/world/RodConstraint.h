
#ifndef RODCONSTRAINT_H
#define RODCONSTRAINT_H

namespace gpp {

class RodConstraint {
private:
    RigidBody* bodyA; // Primeiro corpo rígido
    RigidBody* bodyB; // Segundo corpo rígido
    vector3d anchorA; // Ponto de ancoragem no corpo A (em coordenadas locais)
    vector3d anchorB; // Ponto de ancoragem no corpo B (em coordenadas locais)
    decimal length;   // Comprimento fixo da aste

public:
    RodConstraint(RigidBody* bodyA, RigidBody* bodyB, const vector3d& anchorA, const vector3d& anchorB, decimal length);
    void applyConstraint();
};
} // namespace gpp
#endif // RODCONSTRAINT_H
