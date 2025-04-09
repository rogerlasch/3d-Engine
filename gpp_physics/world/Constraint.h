
#ifndef CONSTRAINT_H
#define CONSTRAINT_H


namespace gpp {

class RigidBody;
class Constraint {
protected:
    RigidBody* bodyA; // Primeiro corpo r�gido
    RigidBody* bodyB; // Segundo corpo r�gido

public:
    Constraint(RigidBody* bodyA, RigidBody* bodyB);
    virtual ~Constraint();

    // M�todo para aplicar a restri��o
    virtual void applyConstraint();

    // M�todo para verificar se a restri��o � v�lida
    virtual bool isValid() const;
};
} // namespace gpp
#endif // CONSTRAINT_H
