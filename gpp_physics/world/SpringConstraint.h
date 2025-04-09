
#ifndef SPRINGCONSTRAINT_H
#define SPRINGCONSTRAINT_H

namespace gpp {

class SpringConstraint {
private:
    RigidBody* bodyA; // Primeiro corpo rígido
    RigidBody* bodyB; // Segundo corpo rígido
    vector3d anchorA; // Ponto de ancoragem no corpo A (em coordenadas locais)
    vector3d anchorB; // Ponto de ancoragem no corpo B (em coordenadas locais)
    decimal restLength; // Comprimento de repouso da mola
    decimal stiffness;  // Rigidez da mola (constante da mola)
    decimal damping;    // Amortecimento da mola

public:
    SpringConstraint(RigidBody* bodyA, RigidBody* bodyB, const vector3d& anchorA, const vector3d& anchorB, decimal restLength, decimal stiffness, decimal damping);
SpringConstraint()=default;

    void applyConstraint();
};

} // namespace gpp

#endif // SPRINGCONSTRAINT_H
