
#include<execution>
#include <iostream>
#include "gpp_world.h"

using namespace std;

namespace gpp{
gpp_world::gpp_world(const AABB& bounds, const vector3d& gravity) {
this->gravity=gravity;
this->bounds=bounds;
tracker=make_unique<CollisionTracker>();
hgeo=make_unique<SpatialOctree>(bounds, 7, 64);
}

gpp_world::~gpp_world() {
tracker.reset();
hgeo.reset();
rigidBodies.clear();
//    for (auto body : rigidBodies) delete body;
    for (auto constraint : constraints) delete constraint;
}

void gpp_world::addRigidBody(RigidBody* body) {
body->setWorld(this);
    rigidBodies.push_back(body);
hgeo->insert(body);
}

void gpp_world::removeRigidBody(RigidBody* body) {
    auto it = std::find(rigidBodies.begin(), rigidBodies.end(), body);
    if (it != rigidBodies.end()) {
(*it)->setWorld(NULL);
        rigidBodies.erase(it);
hgeo->remove(body);
//        delete body;
    }
}

const vector<RigidBody*>& gpp_world::getRigidBodies() const {
    return rigidBodies;
}

void gpp_world::addConstraint(Constraint* constraint) {
    constraints.push_back(constraint);
}

void gpp_world::removeConstraint(Constraint* constraint) {
    auto it = std::find(constraints.begin(), constraints.end(), constraint);
    if (it != constraints.end()) {
        constraints.erase(it);
        delete constraint;
    }
}

const vector<Constraint*>& gpp_world::getConstraints() const {
    return constraints;
}

void gpp_world::updateMotion(decimal deltaTime){
_PROFILER_SNAP();
tbb::concurrent_vector<RigidBody*> updateBodies;
tbb::parallel_for_each(rigidBodies.begin(), rigidBodies.end(), [&](RigidBody* rb){
rb->calculateForces(gravity);
rb->update(deltaTime);
if(!rb->hasFlag(BF_STATIC)){
AABB ab;
rb->getAABB(&ab);
vector3d displacement=bounds.getDisplacement(ab);

if(displacement.lengthSquared()>0.01f) {
rb->translate(displacement);
}

updateBodies.push_back(rb);
}
});

tbb::parallel_for_each(constraints.begin(), constraints.end(), [&](Constraint* constraint){        constraint->applyConstraint();});

for_each(updateBodies.begin(), updateBodies.end(), [&](RigidBody* rb){
hgeo->update(rb);
});
hgeo->removePendingObjects();

}

void gpp_world::update(decimal deltaTime) {
_PROFILER_SNAP();
tbb::concurrent_vector<pair<RigidBody*, RigidBody*>> pairs;
updateMotion(deltaTime);
hgeo->broadPhase(pairs);
tracker->narrowPhase(pairs);
tracker->resolveCollisions();
}

void gpp_world::printState() const {
    for (auto& it : rigidBodies){
logger::info("{}", it->toString());
    }
}
}
