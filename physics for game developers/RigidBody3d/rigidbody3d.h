


#ifndef RIGIDBODY3D_H
#define RIGIDBODY3D_H

class rigidbody3d
{
public:
float fRadius;
float fMass;
float linearDamping;
float angularDamping;
float fSpeed;
float fRestitution;
matrix3x3 fInertia;
matrix3x3 fInertiaInverse;
vector3d vPosition;
vector3d vVelocity;
vector3d vVelocityBody;
vector3d vAngularVelocity;
vector3d vMoments;
vector3d vImpactForces;
vector3d vForces;
quaternion orientation;
bool hasCollision;
rigidbody3d()
{
fMass=100;
 fSpeed=0.0f;
linearDamping=0.15f;
angularDamping=0.15f;
fRestitution=0.35f;
 vPosition={0,0,0};
 vVelocity={0,0,0};
 vVelocityBody={0,0,0};
 vAngularVelocity={0,0,0};
 vMoments={0,0,0};
 vForces={0,0,0};
vImpactForces={0,0,0};
orientation=quaternion_from_euler_angles(0,0,0);
  float inertia = (2.0f/5.0f) * fMass * fRadius * fRadius;
fInertia.setDiagonal({inertia, inertia, inertia});
fInertiaInverse=fInertia.Inverse();
hasCollision=false;
}
void calcLoads()
{
vForces={0,0,0};
if(hasCollision)
{
vForces+=vImpactForces;
vImpactForces={0,0,0};
hasCollision=false;
}
else
{
vForces={0,0, -9.8f};
}
}
void updateBodyEuler(float dt)
{
calcLoads();
 // Take care of translation first:
 // (If this body were a particle, this is all you would need to do.)
  vector3d Ae;
  // calculate the acceleration of the airplane in earth space:
  Ae = vForces / fMass;
  // calculate the velocity of the airplane in earth space:
  vVelocity += Ae * dt;
  // calculate the position of the airplane in earth space:
  vPosition += vVelocity * dt;
 // Now handle the rotations:
  float mag=0.0f;
  // calculate the angular velocity of the airplane in body space:
  vAngularVelocity += fInertiaInverse *
   (vMoments -
   (vAngularVelocity^
   (fInertia *
 vAngularVelocity)))
* dt;
  // calculate the new rotation quaternion:
  orientation += (orientation *
vAngularVelocity) *
   (0.5f * dt);
  // now normalize the orientation quaternion:
  mag = orientation.length();
  if (mag != 0)
   orientation /= mag;
  // calculate the velocity in body space:
  // (we'll need this to calculate lift and drag forces)
  vVelocityBody = quaternion_vector_rotate(~orientation, vVelocity);
  // calculate the air speed:
   fSpeed = vVelocity.length();
}
std::string toString()const
{
std::stringstream ss;
ss<<std::fixed;
ss.precision(2);
ss<<"fMass:"<<fMass<<std::endl;
ss<<"fInertia:"<< fInertia<<std::endl;
ss<<"fInertiaInverse:"<< fInertiaInverse<<std::endl;
ss<<" fSpeed:"<<fSpeed<<std::endl;
ss<<"vPosition:"<< vPosition<<std::endl;
ss<<"vVelocity:"<< vVelocity<<std::endl;
ss<<"vVelocityBody:"<< vVelocityBody<<std::endl;
ss<<"vAngularVelocity:"<< vAngularVelocity<<std::endl;
ss<<"vMoments:"<< vMoments<<std::endl;
ss<<"vForces:"<< vForces<<std::endl;
return ss.str();
}
};
typedef std::shared_ptr<rigidbody3d> shared_rigidbody3d;
typedef std::vector<shared_rigidbody3d> rigidbody3dlist;
#endif
