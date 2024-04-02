
#include"../math/math.h"
#include"../geometry/geometry.h"
#include"WorldProperties.h"
#include"BodyHandler.h"
#include"RigidBody.h"
#include"DefaultBodyHandler.h"

using namespace std;

namespace gpp{

 bool defaultpreStep(RigidBody* rb,WorldProperties* props){
rb->addForce(props->gravity*rb->mass);
return true;
}

 void defaultStep(RigidBody* rb,decimal dt){
if((rb->mass<=0.0f)||(rb->hbody==NULL))return;

//Movimento linear...

vector3d accel=rb->forces*rb->invMass;
rb->velocity+=(accel*dt);

rb->translate((rb->velocity*dt));

//Orientação e movimento angular...
/*
updateInertiaTensor();
angularVelocity+=(invInertia*torque)*dt;
float adamping=powf(angularDamping, dt);
angularVelocity*=adamping;

quaternion q(1.0f, (angularVelocity*dt)*0.5f);
orientation=(orientation+q*orientation);
orientation.normalize();
*/

//Limpar as forças...
rb->forces.zero();
rb->torque.zero();
}
}
