
#ifndef CONSTRAINT_H
#define CONSTRAINT_H


namespace gpp {

class RigidBody;
class Constraint {
protected:
    RigidBody* bodyA; // Primeiro corpo rígido
    RigidBody* bodyB; // Segundo corpo rígido

public:
    Constraint(RigidBody* bodyA, RigidBody* bodyB);
    virtual ~Constraint();

    // Método para aplicar a restrição
    virtual void applyConstraint();

    // Método para verificar se a restrição é válida
    virtual bool isValid() const;
};
} // namespace gpp
#endif // CONSTRAINT_H
