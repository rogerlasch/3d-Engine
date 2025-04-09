
#include <cmath>
#include "gpp_world.h"

using namespace std;

namespace gpp {

RodConstraint::RodConstraint(RigidBody* bodyA, RigidBody* bodyB, const vector3d& anchorA, const vector3d& anchorB, decimal length)
    : bodyA(bodyA), bodyB(bodyB), anchorA(anchorA), anchorB(anchorB), length(length) {}

void RodConstraint::applyConstraint() {
    if (!bodyA || !bodyB) return;

    // Calcula os pontos de ancoragem no espaço global
    vector3d globalAnchorA = bodyA->getTransform()->toGlobal(anchorA);
    vector3d globalAnchorB = bodyB->getTransform()->toGlobal(anchorB);

    // Calcula a distância atual entre os pontos de ancoragem
    vector3d delta = globalAnchorB - globalAnchorA;
    decimal currentLength = delta.length();

    // Se a distância estiver errada, aplica uma correção
    if (std::abs(currentLength - length) > 1e-9) {
        vector3d correction = delta * (currentLength - length) / currentLength;
        decimal invMassSum = bodyA->getInverseMass() + bodyB->getInverseMass();
        if (!bodyA->hasFlag(BF_STATIC)) {
            bodyA->getTransform()->translate(correction * (bodyA->getInverseMass() / invMassSum));
        }

        if (!bodyB->hasFlag(BF_STATIC)) {
            bodyB->getTransform()->translate(-correction * (bodyB->getInverseMass() / invMassSum));
        }
    }
}
} // namespace gpp
