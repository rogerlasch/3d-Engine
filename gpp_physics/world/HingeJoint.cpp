
#include <cmath>
#include"gpp_world.h"

using namespace std;

namespace gpp {

HingeJoint::HingeJoint(RigidBody* bodyA, RigidBody* bodyB, const vector3d& anchorA, const vector3d& anchorB, const vector3d& axis)
    : bodyA(bodyA), bodyB(bodyB), anchorA(anchorA), anchorB(anchorB){
this->axis=axis;
this->axis.normalize();
}

void HingeJoint::applyConstraint() {
    if (!bodyA || !bodyB) return;

    // Calcula os pontos de ancoragem no espa�o global
    vector3d globalAnchorA = bodyA->getTransform()->toGlobal(anchorA);
    vector3d globalAnchorB = bodyB->getTransform()->toGlobal(anchorB);

    // Calcula a dist�ncia entre os pontos de ancoragem
    vector3d delta = globalAnchorB - globalAnchorA;

    // Restringe a transla��o ao longo do eixo de rota��o
    vector3d correction = delta - axis * delta.dot(axis);
    decimal invMassSum = bodyA->getInverseMass() + bodyB->getInverseMass();

    if (!bodyA->hasFlag(BF_STATIC)) {
        bodyA->getTransform()->translate(correction * (bodyA->getInverseMass() / invMassSum));
    }

    if (!bodyB->hasFlag(BF_STATIC)) {
        bodyB->getTransform()->translate(-correction * (bodyB->getInverseMass() / invMassSum));
    }

    // Restringe a rota��o em outros eixos
    vector3d rotationAxisA = bodyA->getTransform()->toLocal(axis);
    vector3d rotationAxisB = bodyB->getTransform()->toLocal(axis);

    vector3d angularVelocityA = bodyA->getAngularVelocity();
    vector3d angularVelocityB = bodyB->getAngularVelocity();

    // Remove a componente da velocidade angular que n�o est� alinhada com o eixo de rota��o
    bodyA->setAngularVelocity(rotationAxisA * angularVelocityA.dot(rotationAxisA));
    bodyB->setAngularVelocity(rotationAxisB * angularVelocityB.dot(rotationAxisB));
}
} // namespace gpp
