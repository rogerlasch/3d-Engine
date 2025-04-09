

#ifndef GPP_WORLD_H
#define GPP_WORLD_H


#include"../common.h"
#include"../math/math.h"
#include"../geometry/geometry.h"
#include"../collision/collision.h"

#include"RigidBody.h"
#include"Constraint.h"
#include"RodConstraint.h"
#include"SpringConstraint.h"
#include"HingeJoint.h"
#include"SpatialOctree.h"

namespace gpp {

class gpp_world {
private:

vector3d gravity;
    std::vector<RigidBody*> rigidBodies; // Lista de corpos rígidos
    std::vector<Constraint*> constraints; // Lista de restrições
AABB bounds;
std::unique_ptr<CollisionTracker> tracker;
std::unique_ptr<SpatialOctree> hgeo;

public:
    gpp_world(const AABB& bounds=AABB({-1000, -1000, -1000}, {1000, 1000, 1000}), const vector3d& gravity={0,0, -9.81f});
    ~gpp_world();

const AABB& getBounds()const{return this->bounds;}
void setGravity(const vector3d& v){this->gravity=v;}
vector3d getGravity()const{return this->gravity;}

    // Gerenciamento de corpos rígidos
    void addRigidBody(RigidBody* body);
    void removeRigidBody(RigidBody* body);
    const std::vector<RigidBody*>& getRigidBodies() const;

    // Gerenciamento de restrições
    void addConstraint(Constraint* constraint);
    void removeConstraint(Constraint* constraint);
    const std::vector<Constraint*>& getConstraints() const;

void updateMotion(decimal deltaTime);
    void update(decimal deltaTime);
void printState() const ;
};
} // namespace gpp
#endif // GPP_WORLD_H
