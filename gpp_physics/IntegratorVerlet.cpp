


#include"gpp_physics.h"

using namespace std;

namespace gpp
{

void IntegratorVerlet::integrate(RigidBodyList& bodies, float delta_time)
{
for(auto& it : bodies)
{
integrate(it, delta_time);
}
}

void IntegratorVerlet::integrate(RigidBody* rb, float delta_time)
{
if((rb==NULL)||(rb->getMass()<=0)) return;
    //Atualizar velocidade linear e posição
vector3d displacement;
    rb->linearVelocity = rb->linearMomentum / rb->mass;
displacement= rb->linearVelocity * delta_time + 0.5f * rb->linearAcceleration * delta_time * delta_time;
    rb->position +=displacement;
    rb->linearMomentum += rb->force * delta_time;
    rb->linearVelocity = rb->linearMomentum / rb->mass;

    //Atualizar orientação
    rb->angularVelocity = rb->inverseInertiaTensor * rb->angularMomentum;
    quaternion deltaOrientation(0.0f, rb->angularVelocity * delta_time * 0.5f);
    deltaOrientation = deltaOrientation * rb->orientation;
    rb->orientation += deltaOrientation;
    rb->orientation.normalize();

    //Atualizar velocidade angular
    rb->angularMomentum += rb->torque * delta_time;
    rb->angularVelocity = rb->inverseInertiaTensor * rb->angularMomentum;
if(rb->getGeometricShape()!=NULL)
{
rb->getGeometricShape()->Translate(displacement);
rb->getGeometricShape()->Rotate(rb->orientation);
}
}
}
