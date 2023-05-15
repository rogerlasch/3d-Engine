


#ifndef RIGIDBODY3D_H
#define RIGIDBODY3D_H

#define AIRDENCITY 1.23f
#define LINEARDRAGCOEFFICIENT 0.35f
#define GRAVITY -9.81f*10

class rigidbody3d
{
public:
float radius;
    float mass; // Massa do corpo
    float inverseMass;
    float linearDamping;
    float angularDamping;
    float restitution;
    vector3d position; // Posição do corpo
    vector3d linearMomentum;
    vector3d angularMomentum;
    vector3d linearVelocity;
    vector3d angularVelocity;
    vector3d linearAcceleration;
    vector3d angularAcceleration;
    vector3d force;
    vector3d torque;
    matrix3x3 inertiaTensor;
    matrix3x3 inverseInertiaTensor;
    quaternion orientation;
rigidbody3d()
{
 radius=0.5f;
     mass=0.0f; // Massa do corpo
     inverseMass=0.0f;
     linearDamping=0.30f;
     angularDamping=0.30f;
     restitution=0.5f;
     position={0,0,0}; // Posição do corpo
     linearMomentum={0,0,0};
     angularMomentum={0,0,0};
     linearVelocity={0,0,0};
     angularVelocity={0,0,0};
     linearAcceleration={0,0,0};
     angularAcceleration={0,0,0};
     force={0,0,0};
     torque={0,0,0};
orientation=quaternion_from_euler_angles(0,0,0);
}
void calcInertia()
{
  float i = (2.0f/5.0f) * mass* radius* radius;
inertiaTensor.setDiagonal(vector3d(i,i,i));
inverseInertiaTensor=inertiaTensor.Inverse();
}
std::string toString()
{
std::stringstream ss;
#define _ff(x) ss<<#x<<":"<<x<<std::endl;
_ff(radius);
_ff(mass);
_ff(inverseMass);
_ff(linearDamping);
_ff(angularDamping);
_ff(restitution);
_ff(position);
_ff(linearMomentum);
_ff(angularMomentum);
_ff(linearVelocity);
_ff(angularVelocity);
_ff(linearAcceleration);
_ff(angularAcceleration);
_ff(force);
_ff(torque);
_ff(inertiaTensor);
_ff(inverseInertiaTensor);
_ff(orientation);
return ss.str();
}
void updateBodyVerlet(float dt)
{
        // Atualiza posição e velocidade linear
        vector3d displacement = linearVelocity * dt + 0.5f * linearAcceleration * dt * dt;
        position += displacement;
        linearVelocity += linearAcceleration * dt;
        linearVelocity *= pow(linearDamping, dt);

        // Atualiza orientação e velocidade angular
        quaternion orientation1 = orientation + quaternion(0.0f, angularVelocity * 0.5f * dt) * orientation;
        orientation1.normalize();
        angularVelocity += angularAcceleration * dt;
        angularVelocity *= pow(angularDamping, dt);

        // Atualiza momento linear e angular
        linearMomentum += force * dt;
        angularMomentum += torque * dt;

        // Atualiza matrizes de inércia e vetores de aceleração
        matrix3x3 worldInertiaTensor = quaternion_extract_matrix(orientation1) * inverseInertiaTensor *
quaternion_extract_matrix(orientation1).Transpose();
        linearAcceleration = force * inverseMass;
        angularAcceleration = worldInertiaTensor * torque;

        // Atualiza orientação, normaliza e atualiza a matriz de rotação
        orientation = orientation + quaternion(0.0f, angularVelocity * 0.5f * dt) * orientation;
        orientation.normalize();

        // Atualiza matriz de inércia invertida
        inverseInertiaTensor = worldInertiaTensor.Inverse();

        // Atualiza força e torque
        force = vector3d(0.0f, 0.0f, 0.0f);
        torque = vector3d(0.0f, 0.0f, 0.0f);
}

void calcLoads()
{
vector3d vGravity=vector3d(0,0, GRAVITY*mass);
force+=vGravity;
float fArea=((4*GPP_PI)*(radius*radius))/2.0f;
vector3d vRelativeVelocity=linearVelocity;
vector3d vDragForce=-0.5f*LINEARDRAGCOEFFICIENT*AIRDENCITY*fArea*vRelativeVelocity*vRelativeVelocity.length();
force+=vDragForce;
}
};
typedef std::shared_ptr<rigidbody3d> shared_rigidbody3d;
typedef std::vector<shared_rigidbody3d> rigidbody3dlist;

struct bcollisioninfo
{
vector3d vNormal={0,0,0};
vector3d vPoint={0,0,0};
vector3d vRelativeVelocity={0,0,0};
vector3d vRelativeAcceleration={0,0,0};
vector3d vTangent={0,0,0};
float depth=0.0f;
std::string toString()
{
std::stringstream ss;
ss<<"vNormal: "<< vNormal<<std::endl;
ss<<"vPoint: "<< vPoint<<std::endl;
ss<<"vRelativeVelocity: "<< vRelativeVelocity<<std::endl;
ss<<"vRelativeAcceleration: "<< vRelativeAcceleration<<std::endl;
ss<<"vTangent: "<< vTangent<<std::endl;
ss<<"depth: "<<depth<<std::endl;
return ss.str();
}
};
#endif
