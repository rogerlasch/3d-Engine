
#include <cmath>
#include "gpp_world.h"

using namespace std;

namespace gpp {

SpringConstraint::SpringConstraint(RigidBody* bodyA, RigidBody* bodyB, const vector3d& anchorA, const vector3d& anchorB, decimal restLength, decimal stiffness, decimal damping)
    : bodyA(bodyA), bodyB(bodyB), anchorA(anchorA), anchorB(anchorB), restLength(restLength), stiffness(stiffness), damping(damping) {}

void SpringConstraint::applyConstraint() {
    if (!bodyA || !bodyB) return;

    // Calcula os pontos de ancoragem no espaço global
    vector3d globalAnchorA = bodyA->getTransform()->toGlobal(anchorA);
    vector3d globalAnchorB = bodyB->getTransform()->toGlobal(anchorB);

    // Calcula a distância atual entre os pontos de ancoragem
    vector3d delta = globalAnchorB - globalAnchorA;
    decimal currentLength = delta.length();

    if (currentLength > 1e-9) {
        vector3d direction = delta / currentLength;

        // Força da mola (lei de Hooke)
        decimal displacement = currentLength - restLength;
        vector3d springForce = direction * (stiffness * displacement);

        // Força de amortecimento
        vector3d relativeVelocity = bodyB->getVelocityAtPoint(globalAnchorB) - bodyA->getVelocityAtPoint(globalAnchorA);
        vector3d dampingForce = direction * (damping * relativeVelocity.dot(direction));

        // Aplica as forças nos pontos de ancoragem
        if (!bodyA->hasFlag(BF_STATIC)) {
            bodyA->applyForceAtPoint(springForce + dampingForce, globalAnchorA);
        }
        if (!bodyB->hasFlag(BF_STATIC)) {
            bodyB->applyForceAtPoint(-springForce - dampingForce, globalAnchorB);
        }
    }
}
} // namespace gpp
