


#ifndef BODYMANAGER_H
#define BODYMANAGER_H

using namespace gpp;

std::vector<std::shared_ptr<RigidBody>> hbodies;

RigidBody* createBodySphere(const vector3d& pos, decimal radius, const vector3d& angles={0,0,0}, decimal mass=1.0f){

Sphere3d* s=new Sphere3d(radius, new Transform(pos, angles, {1,1,1}));
std::shared_ptr<RigidBody> rb=std::make_shared<RigidBody>(nullptr, BF_GRAVITY, mass, s);
hbodies.push_back(rb);
return rb.get();
}

RigidBody* createBodyBox(const vector3d& pos, const vector3d& alf, const vector3d& angles={0,0,0}, decimal mass=1.0f){

Box3d* s=new Box3d(alf, new Transform(pos, angles, {1,1,1}));
std::shared_ptr<RigidBody> rb=std::make_shared<RigidBody>(nullptr, 0, mass, s);
hbodies.push_back(rb);
return rb.get();
}

RigidBody* createBodyCapsule(const vector3d& p1, const vector3d& p2, decimal radius, const vector3d& angles={0,0,0}, decimal mass=1.0f){

Capsule3d* s=new Capsule3d(p1, p2, radius, new Transform((p2+p1)*0.5f, angles, {1,1,1}));
std::shared_ptr<RigidBody> rb=std::make_shared<RigidBody>(nullptr, 0, mass, s);
hbodies.push_back(rb);
return rb.get();
}
#endif
