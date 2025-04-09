
#include "Constraint.h"

using namespace std;

namespace gpp {

Constraint::Constraint(RigidBody* bodyA, RigidBody* bodyB)
    : bodyA(bodyA), bodyB(bodyB) {}

Constraint::~Constraint(){
bodyA=nullptr;
bodyB=nullptr;
}

void Constraint::applyConstraint(){
}

bool Constraint::isValid() const {
    return bodyA != nullptr && bodyB != nullptr;
}
} // namespace gpp
