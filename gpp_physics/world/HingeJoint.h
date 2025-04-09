
#ifndef HINGEJOINT_H
#define HINGEJOINT_H

namespace gpp {

class HingeJoint {
private:
    RigidBody* bodyA; // Primeiro corpo rígido (por exemplo, o batente)
    RigidBody* bodyB; // Segundo corpo rígido (por exemplo, a porta)
    vector3d anchorA; // Ponto de ancoragem no corpo A (em coordenadas locais)
    vector3d anchorB; // Ponto de ancoragem no corpo B (em coordenadas locais)
    vector3d axis;    // Eixo de rotação (em coordenadas globais)

public:
    HingeJoint(RigidBody* bodyA, RigidBody* bodyB, const vector3d& anchorA, const vector3d& anchorB, const vector3d& axis);
virtual ~HingeJoint()=default;

    void applyConstraint();
};
} // namespace gpp
#endif // HINGEJOINT_H
