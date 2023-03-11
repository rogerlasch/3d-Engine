


#ifndef RIGIDBODY3D_H
#define RIGIDBODY3D_H

  #define   BALLDIAMETER  0.05715f
  #define   BALLWEIGHT 1.612f
  #define   GRAVITY   -9.87f
  #define   LINEARDRAGCOEFFICIENT 0.5f
  #define   ANGULARDRAGCOEFFICIENT 0.05f
  #define   FRICTIONFACTOR 0.5f
  #define   COEFFICIENTOFRESTITUTION 0.8f
  #define   COEFFICIENTOFRESTITUTIONGROUND 0.1f
  #define   FRICTIONCOEFFICIENTBALLS 0.1f
  #define   FRICTIONCOEFFICIENTGROUND 0.1f
  #define   ROLLINGRESISTANCECOEFFICIENT 0.025f

typedef struct _RigidBody3d {
float fMass;   // Total mass (constant)
matrix3x3 mInertia;// Mass moment of inertia in body coordinates
matrix3x3 mInertiaInverse;// Inverse of mass moment of inertia matrix
vector3d vPosition;  // Position in earth coordinates
vector3d vVelocity;  // Velocity in earth coordinates
vector3d vVelocityBody; // Velocity in body coordinates
vector3d vAcceleration; // Acceleration of cg in earth space
vector3d vAngularAcceleration; //Angular acceleration in body coordinates
vector3d vAngularAccelerationGlobal; // Angular acceleration
   // in global coordinates
vector3d vAngularVelocity; // Angular velocity in body coordinates
vector3d vAngularVelocityGlobal; // Angular velocity in global coordinates
vector3d vEulerAngles;// Euler angles in body coordinates
float fSpeed;   // Speed (magnitude of the velocity)
quaternion qOrientation;  // Orientation in earth coordinates
vector3d vForces; // Total force on body
vector3d vMoments;// Total moment (torque) on body
matrix3x3  mIeInverse; // Inverse of moment of inertia in earth coordinates
float  fRadius;  // Ball radius
std::string toString()
{
std::stringstream ss;
ss<<"fMass:"<< fMass<<std::endl;   // Total mass (constant)
ss<<"mInertia:"<< mInertia<<std::endl;// Mass moment of inertia in body coordinates
ss<<"mInertiaInverse:"<< mInertiaInverse<<std::endl;// Inverse of mass moment of inertia matrix
ss<<"vPosition:"<< vPosition<<std::endl;  // Position in earth coordinates
ss<<"vVelocity:"<< vVelocity<<std::endl;  // Velocity in earth coordinates
ss<<"vVelocityBody:"<< vVelocityBody<<std::endl; // Velocity in body coordinates
ss<<"vAcceleration:"<< vAcceleration<<std::endl; // Acceleration of cg in earth space
ss<<"vAngularAcceleration:"<< vAngularAcceleration<<std::endl; //Angular acceleration in body coordinates
ss<<"vAngularAccelerationGlobal:"<< vAngularAccelerationGlobal<<std::endl; // Angular acceleration
   // in global coordinates
ss<<"vAngularVelocity:"<< vAngularVelocity<<std::endl; // Angular velocity in body coordinates
ss<<"vAngularVelocityGlobal:"<< vAngularVelocityGlobal<<std::endl; // Angular velocity in global coordinates
ss<<"vEulerAngles:"<< vEulerAngles<<std::endl;// Euler angles in body coordinates
ss<<"fSpeed:"<< fSpeed<<std::endl;   // Speed (magnitude of the velocity)
ss<<"qOrientation:"<< qOrientation<<std::endl;  // Orientation in earth coordinates
ss<<"vForces:"<< vForces<<std::endl; // Total force on body
ss<<"vMoments:"<< vMoments<<std::endl;// Total moment (torque) on body
ss<<"mIeInverse:"<<  mIeInverse<<std::endl; // Inverse of moment of inertia in earth coordinates
ss<<"fRadius:"<<  fRadius<<std::endl;  // Ball radius
return ss.str();
}
} RigidBody3d, *pRigidBody3d;

typedef struct _Collision {
int body1;
int body2;
vector3d vCollisionNormal;
vector3d vCollisionPoint;
vector3d vRelativeVelocity;
vector3d vRelativeAcceleration;
   vector3d vCollisionTangent;
  }Collision, *pCollision;

#endif
