


#include<cassert>
#include<gpp/gpp.h>
#include<gpp_physics/gpp_physics.h>
using namespace gpp;
#include"rigidbody3d.h"

using namespace std;


#define NUMBODIES 4
#define NOCOLLISION 0
#define COLLISION 1
#define CONTACT 2
#define rho 1.23f
#define VELOCITYTOLERANCE 1.0f
#define COLLISIONTOLERANCE 0.1f
  RigidBody3d   Bodies[NUMBODIES];
  Collision   Collisions[NUMBODIES*8];
  int NumCollisions = 0;
bool DOCONTACT=true;
bool DOFRICTION=true;

void InitializeObjects(int configuration);
  void StepSimulation(float dtime);
  void CalcObjectForces(void);
int  CheckForCollisions(void);
bool CollisionRecordedAlready(int i, int j);
int   CheckGroundPlaneCollisions(pCollision CollisionData, int body1);
  void ResolveCollisions(void);
int   CheckGroundPlaneContacts(pCollision CollisionData, int body1);
void FlushCollisionData();
void printall(const string& msg);

int main()
{
setlocale(LC_ALL, "Portuguese");
InitializeObjects(0);
printall("Imprimindo objetos antes da simulação...");
for(int i=0; i<150; i++)
{
StepSimulation(0.1f);
}
printall("Imprimindo objetos depois da simulação.");
return 0;
}

void printall(const string& msg)
{
if(msg.size()>0)
{
_FLOG("{}", msg);
}
for(int i=0; i<4; i++)
{
_FLOG("{}", Bodies[i].toString());
}
}
void InitializeObjects(int configuration)
{
float  iRoll, iPitch, iYaw;
int i;
float  Ixx, Iyy, Izz;
float  s;
/////////////////////////////////////////////////////
// Initialize the cue ball:
// Set initial position
Bodies[0].vPosition.x = -BALLDIAMETER*50.0f;
Bodies[0].vPosition.y = 0.0f;
Bodies[0].vPosition.z = BALLDIAMETER/2.0f;
// Set initial velocity
s = 7.0;
Bodies[0].vVelocity.x = s;
Bodies[0].vVelocity.y = 0.0f;
Bodies[0].vVelocity.z = 0.0f;
Bodies[0].fSpeed = s;
   // Set initial angular velocity
   Bodies[0].vAngularVelocity.x = 0.0f; // rotate about long'l axis
   Bodies[0].vAngularVelocity.y = 0.0f; // rotate about transverse axis
   Bodies[0].vAngularVelocity.z = 0.0f; // rotate about vertical axis
   Bodies[0].vAngularAcceleration.x = 0.0f;
   Bodies[0].vAngularAcceleration.y = 0.0f;
   Bodies[0].vAngularAcceleration.z = 0.0f;
   Bodies[0].vAcceleration.x = 0.0f;
   Bodies[0].vAcceleration.y = 0.0f;
   Bodies[0].vAcceleration.z = 0.0f;
   // Set the initial forces and moments
   Bodies[0].vForces.x = 0.0f;
   Bodies[0].vForces.y = 0.0f;
   Bodies[0].vForces.z = 0.0f;
   Bodies[0].vMoments.x = 0.0f;
   Bodies[0].vMoments.y = 0.0f;
   Bodies[0].vMoments.z = 0.0f;
   // Zero the velocity in body space coordinates
   Bodies[0].vVelocityBody.x = 0.0f;
   Bodies[0].vVelocityBody.y = 0.0f;
   Bodies[0].vVelocityBody.z = 0.0f;
   // Set the initial orientation
   iRoll = 0.0f;
   iPitch = 0.0f;
   iYaw = 0.0f;
   Bodies[0].qOrientation = quaternion_from_euler_angles(iRoll, iPitch, iYaw);
   // Set the mass properties
   Bodies[0].fMass = BALLWEIGHT/(-GRAVITY);
   Ixx = 2.0f * Bodies[0].fMass / 5.0f * (BALLDIAMETER/2*BALLDIAMETER/2);
   Izz = Iyy = Ixx;
   Bodies[0].mInertia.e11   =   Ixx;
   Bodies[0].mInertia.e12   =   0;
   Bodies[0].mInertia.e13   =   0;
   Bodies[0].mInertia.e21   =   0;
   Bodies[0].mInertia.e22   =   Iyy;
   Bodies[0].mInertia.e23   =   0;
   Bodies[0].mInertia.e31   =   0;
   Bodies[0].mInertia.e32   =   0;
   Bodies[0].mInertia.e33   =   Izz;
   Bodies[0].mInertiaInverse = Bodies[0].mInertia.Inverse();
Bodies[0].fRadius = BALLDIAMETER/2;
/////////////////////////////////////////////////////
// Initialize the other balls
for(i=1; i<NUMBODIES; i++)
{
// Set initial position
if(i==1)
{
Bodies[i].vPosition.x = 0.0;
Bodies[i].vPosition.y = -(BALLDIAMETER/2.0f+0.25*BALLDIAMETER);
Bodies[i].vPosition.z = BALLDIAMETER/2.0f;
} else if(i==2) {
Bodies[i].vPosition.x = 0.0;
Bodies[i].vPosition.y = BALLDIAMETER/2.0f+0.25*BALLDIAMETER;
Bodies[i].vPosition.z = BALLDIAMETER/2.0f;
} else {
Bodies[i].vPosition.x = -BALLDIAMETER;
Bodies[i].vPosition.y = 0.0f;
Bodies[i].vPosition.z = BALLDIAMETER/2.0f;
}
// Set initial velocity
Bodies[i].vVelocity.x = 0.0f;
Bodies[i].vVelocity.y = 0.0f;
Bodies[i].vVelocity.z = 0.0f;
Bodies[i].fSpeed = 0.0f;
// Set initial angular velocity
Bodies[i].vAngularVelocity.x = 0.0f;
Bodies[i].vAngularVelocity.y = 0.0f;
Bodies[i].vAngularVelocity.z = 0.0f;
Bodies[i].vAngularAcceleration.x = 0.0f;
Bodies[i].vAngularAcceleration.y = 0.0f;
Bodies[i].vAngularAcceleration.z = 0.0f;
Bodies[i].vAcceleration.x = 0.0f;
Bodies[i].vAcceleration.y = 0.0f;
Bodies[i].vAcceleration.z = 0.0f;
// Set the initial forces and moments
Bodies[i].vForces.x = 0.0f;
Bodies[i].vForces.y = 0.0f;
Bodies[i].vForces.z = 0.0f;
Bodies[i].vMoments.x = 0.0f;
Bodies[i].vMoments.y = 0.0f;
Bodies[i].vMoments.z = 0.0f;
// Zero the velocity in body space coordinates
Bodies[i].vVelocityBody.x = 0.0f;
Bodies[i].vVelocityBody.y = 0.0f;
Bodies[i].vVelocityBody.z = 0.0f;
// Set the initial orientation
iRoll = 0.0f;
iPitch = 0.0f;
iYaw = 0.0f;
Bodies[i].qOrientation = quaternion_from_euler_angles(iRoll, iPitch, iYaw);
// Set the mass properties
Bodies[i].fMass = BALLWEIGHT/(-GRAVITY);
Ixx = 2.0f * Bodies[i].fMass / 5.0f * (BALLDIAMETER*BALLDIAMETER);
Izz = Iyy = Ixx;
Bodies[i].mInertia.e11   =   Ixx;
Bodies[i].mInertia.e12   =   0;
Bodies[i].mInertia.e13   =   0;
Bodies[i].mInertia.e21   =   0;
Bodies[i].mInertia.e22   =   Iyy;
Bodies[i].mInertia.e23   =   0;
Bodies[i].mInertia.e31   =   0;
Bodies[i].mInertia.e32   =   0;
Bodies[i].mInertia.e33   =   Izz;
Bodies[i].mInertiaInverse = Bodies[i].mInertia.Inverse();
Bodies[i].fRadius = BALLDIAMETER/2;
   }
  }

  void StepSimulation(float dtime)
  {
  vector3d Ae;
  int i;
  float  dt = dtime;
  int check = NOCOLLISION;
  int c = 0;
   // Calculate all of the forces and moments on the balls:
   CalcObjectForces();
// Integrate the equations of motion:
for(i=0; i<NUMBODIES; i++)
{
// Calculate the acceleration in earth space:
Ae = Bodies[i].vForces / Bodies[i].fMass;
Bodies[i].vAcceleration = Ae;
// Calculate the velocity in earth space:
Bodies[i].vVelocity += Ae * dt;
// Calculate the position in earth space:
Bodies[i].vPosition += Bodies[i].vVelocity * dt;
// Now handle the rotations:
float mag;
Bodies[i].vAngularAcceleration = Bodies[i].mInertiaInverse *
(Bodies[i].vMoments -
(Bodies[i].vAngularVelocity^
(Bodies[i].mInertia *
Bodies[i].vAngularVelocity)));
Bodies[i].vAngularVelocity += Bodies[i].vAngularAcceleration * dt;
// Calculate the new rotation quaternion:
Bodies[i].qOrientation += (Bodies[i].qOrientation *
   Bodies[i].vAngularVelocity) *
   (0.5f * dt);
// Now normalize the orientation quaternion:
mag = Bodies[i].qOrientation.length();
if (mag != 0)
Bodies[i].qOrientation /= mag;
// Calculate the velocity in body space:
Bodies[i].vVelocityBody = quaternion_vector_rotate(~Bodies[i].qOrientation,
Bodies[i].vVelocity);
// Get the angular velocity in global coords:
Bodies[i].vAngularVelocityGlobal = quaternion_vector_rotate(Bodies[i].qOrientation,
Bodies[i].vAngularVelocity);
// Get the angular acceleration in global coords:
Bodies[i].vAngularAccelerationGlobal = quaternion_vector_rotate(Bodies[i].qOrientation,
Bodies[i].vAngularAcceleration);
// Get the inverse intertia tensor in global coordinates
matrix3x3 R, RT;
R = quaternion_extract_matrix(Bodies[i].qOrientation);
RT = R.Transpose();
Bodies[i].mIeInverse = R * Bodies[i].mInertiaInverse * RT;
// Calculate the air speed:
Bodies[i].fSpeed = Bodies[i].vVelocity.length();
// Get the Euler angles for our information
vector3d u;
u = quaternion_extract_euler_angles(Bodies[i].qOrientation);
Bodies[i].vEulerAngles.x = u.x;// roll
Bodies[i].vEulerAngles.y = u.y;// pitch
Bodies[i].vEulerAngles.z = u.z;// yaw
   }
   // Handle Collisions :
   check = CheckForCollisions();
   if(check == COLLISION)
   ResolveCollisions();
  }

  void CalcObjectForces(void)
  {
  vector3d Fb, Mb;
  vector3d vDragvector3d;
  vector3d vAngularDragvector3d;
  int i, j;
  vector3d ContactForce;
  vector3d pt;
  int check = NOCOLLISION; 
  pCollision pCollisionData;
  vector3d FrictionForce;
  vector3d fDir;
  double speed;
  vector3d FRn, FRt;
   for(i=0; i<NUMBODIES; i++)
   {
   // Reset forces and moments:
   Bodies[i].vForces.x = 0.0f;
   Bodies[i].vForces.y = 0.0f;
   Bodies[i].vForces.z = 0.0f;
Bodies[i].vMoments.x = 0.0f;
Bodies[i].vMoments.y = 0.0f;
Bodies[i].vMoments.z = 0.0f;
Fb.x = 0.0f;   Mb.x = 0.0f;
Fb.y = 0.0f;   Mb.y = 0.0f;
Fb.z = 0.0f;   Mb.z = 0.0f;
// Do drag force:
vDragvector3d = -Bodies[i].vVelocityBody;
vDragvector3d.normalize();
speed = Bodies[i].vVelocityBody.length();
Fb += vDragvector3d * ((1.0f/2.0f)*speed * speed * rho *
LINEARDRAGCOEFFICIENT * pow(Bodies[i].fRadius,2) *
Bodies[i].fRadius*GPP_PI);
vAngularDragvector3d = -Bodies[i].vAngularVelocity;
vAngularDragvector3d.normalize();
Mb += vAngularDragvector3d * (Bodies[i].vAngularVelocity.length() *
Bodies[i].vAngularVelocity.length() * rho * ANGULARDRAGCOEFFICIENT
* 4 * pow(Bodies[i].fRadius,2)*GPP_PI);
// Convert forces from model space to earth space:
Bodies[i].vForces = quaternion_vector_rotate(Bodies[i].qOrientation, Fb);
// Apply gravity:
Bodies[i].vForces.z += GRAVITY * Bodies[i].fMass;
// Save the moments:
Bodies[i].vMoments += Mb;
// Handle contacts with ground plane:
Bodies[i].vAcceleration = Bodies[i].vForces / Bodies[i].fMass;
Bodies[i].vAngularAcceleration = Bodies[i].mInertiaInverse *
(Bodies[i].vMoments -
(Bodies[i].vAngularVelocity^
(Bodies[i].mInertia *
 Bodies[i].vAngularVelocity)));
// Resolve ground plane contacts:
FlushCollisionData();
pCollisionData = Collisions;
NumCollisions = 0;
if(DOCONTACT)
check = CheckGroundPlaneContacts(pCollisionData, i);
if((check == CONTACT) && DOCONTACT)
{ j = 0;
{
 assert(NumCollisions <= 1);
ContactForce = (Bodies[i].fMass * (-Bodies[i].vAcceleration *
Collisions[j].vCollisionNormal)) *
Collisions[j].vCollisionNormal;
if(DOFRICTION)
   {
   double vt = fabs(Collisions[j].vRelativeVelocity *
   Collisions[j].vCollisionTangent);
   if(vt > VELOCITYTOLERANCE)
   {
   // Kinetic:
   FrictionForce = (ContactForce.length() *
   FRICTIONCOEFFICIENTGROUND) *
   Collisions[j].vCollisionTangent;
   } else {
   // Static:
   FrictionForce = (ContactForce.length() *
   FRICTIONCOEFFICIENTGROUND * 2 *
   vt/VELOCITYTOLERANCE) *
   Collisions[j].vCollisionTangent;
   }
   } else
   FrictionForce.x = FrictionForce.y = FrictionForce.z = 0;
   // Do rolling resistance:
   if(Bodies[i].vAngularVelocity.length() > VELOCITYTOLERANCE)
   {
   FRn = ContactForce.length() *
 Collisions[j].vCollisionNormal;
   Collisions[j].vCollisionTangent.normalize();
   vector3d m = (Collisions[j].vCollisionTangent
   *(ROLLINGRESISTANCECOEFFICIENT *
   Bodies[i].fRadius))^FRn;
   double mag = m.length();
   vector3d a = Bodies[i].vAngularVelocity;
   a.normalize();
   Bodies[i].vMoments += -a * mag;
   }
   // accumlate contact and friction forces and moments
   Bodies[i].vForces += ContactForce;
   Bodies[i].vForces += FrictionForce;
   ContactForce = quaternion_vector_rotate(~Bodies[i].qOrientation, ContactForce);
   FrictionForce = quaternion_vector_rotate(~Bodies[i].qOrientation,
  FrictionForce);
   pt = Collisions[j].vCollisionPoint - Bodies[i].vPosition;
   pt = quaternion_vector_rotate(~Bodies[i].qOrientation, pt);
   Bodies[i].vMoments += pt^ContactForce;
   Bodies[i].vMoments += pt^FrictionForce;
  }
}
   }
  }

int  CheckForCollisions(void)
{
  int  status = NOCOLLISION;
  int  i, j;
  vector3d   d;
  pCollision   pCollisionData;
  int  check = NOCOLLISION;
  float r;
  float s;
  vector3d   tmp;
  FlushCollisionData();
  pCollisionData = Collisions;
  NumCollisions = 0;
  // check object collisions with each other
  for(i=0; i<NUMBODIES; i++)
  {
  for(j=0; j<NUMBODIES; j++)
  if((j!=i) && !CollisionRecordedAlready(i, j))
  {
  // do a bounding sphere check
  d = Bodies[i].vPosition - Bodies[j].vPosition;
  r = Bodies[i].fRadius + Bodies[j].fRadius;
  s = d.length() - r;
   if(s < COLLISIONTOLERANCE)
   {// possible collision
   vector3d   pt1, pt2, vel1, vel2, n, Vr;
   float Vrn;
   pt1 = (Bodies[i].vPosition + Bodies[j].vPosition)/2;
   tmp = pt2 = pt1;
   pt1 = pt1-Bodies[i].vPosition;
   pt2 = pt2-Bodies[j].vPosition;
   vel1 = Bodies[i].vVelocity +
  (Bodies[i].vAngularVelocityGlobal^pt1);
   vel2 = Bodies[j].vVelocity +
  (Bodies[j].vAngularVelocityGlobal^pt2);
   n = d;
   n.normalize();
   Vr = (vel1 - vel2);
   Vrn = Vr * n;
   if(Vrn < -VELOCITYTOLERANCE)
   {
   // Have a collision so fill the data structure
   assert(NumCollisions < (NUMBODIES*8));
   if(NumCollisions < (NUMBODIES*8))
   {
   pCollisionData->body1 = i;
   pCollisionData->body2 = j;
   pCollisionData->vCollisionNormal = n;
   pCollisionData->vCollisionPoint = tmp;
   pCollisionData->vRelativeVelocity = Vr;
   pCollisionData->vCollisionTangent = (n^Vr)^n;
   pCollisionData->vCollisionTangent.normalize();
   pCollisionData++;
   NumCollisions++;
   status = COLLISION;
   }  }
   }
  }
   }
   for(i=0; i<NUMBODIES; i++)
   {
   check = NOCOLLISION;
  assert(NumCollisions < (NUMBODIES*8));
  check = CheckGroundPlaneCollisions(pCollisionData, i);
  if(check == COLLISION)
  {
status = COLLISION;
pCollisionData++;
NumCollisions++;
   }
  }
  return status;
}

bool CollisionRecordedAlready(int i, int j)
{
int k;
int b1, b2;
  for(k=0; k<NumCollisions; k++)
  {
  b1 = Collisions[k].body1;
  b2 = Collisions[k].body2;
  if( ((b1 == i) && (b2 == j)) ||
  ((b1 == j) && (b2 == i)) )
  return true;
  }
  return false;
}

int   CheckGroundPlaneCollisions(pCollision CollisionData, int body1)
{
  vector3d   tmp;
  vector3d   vel1;
  vector3d   pt1;
  vector3d   Vr;
  float Vrn;
  vector3d   n;
  int  status = NOCOLLISION;
  if(Bodies[body1].vPosition.z <= (Bodies[body1].fRadius))
  {
  pt1 = Bodies[body1].vPosition;
pt1.z = COLLISIONTOLERANCE;
tmp = pt1;
pt1 = pt1-Bodies[body1].vPosition;
vel1 = Bodies[body1].vVelocity/*Body*/ +
   (Bodies[body1].vAngularVelocityGlobal^pt1);
n.x = 0;
n.y = 0;
n.z = 1;
Vr = vel1;
Vrn = Vr * n;
if(Vrn < -VELOCITYTOLERANCE)
{
// Have a collision so fill the data structure
assert(NumCollisions < (NUMBODIES*8));
if(NumCollisions < (NUMBODIES*8))
{
CollisionData->body1 = body1;
CollisionData->body2 = -1;
CollisionData->vCollisionNormal = n;
CollisionData->vCollisionPoint = tmp;
CollisionData->vRelativeVelocity = Vr;
   CollisionData->vCollisionTangent = (n^Vr)^n;
   CollisionData->vCollisionTangent.inverse();
   CollisionData->vCollisionTangent.normalize();
   status = COLLISION;
  }
}
   }
   return status;
  }

  void ResolveCollisions(void)
  {
  int   i;
  double j;
  vector3d pt1, pt2, vB1V, vB2V, vB1AV, vB2AV;
  float fCr = COEFFICIENTOFRESTITUTION;
  int   b1, b2;
  float Vrt;
  float muB = FRICTIONCOEFFICIENTBALLS;
  float muG = FRICTIONCOEFFICIENTGROUND;
  bool  dofriction = DOFRICTION;
for(i=0; i<NumCollisions; i++)
{
b1 = Collisions[i].body1;
b2 = Collisions[i].body2;
if( (b1 != -1) && (b1 != b2) )
{
if(b2 != -1) // not ground plane
{
pt1 = Collisions[i].vCollisionPoint - Bodies[b1].vPosition;
pt2 = Collisions[i].vCollisionPoint - Bodies[b2].vPosition;
// Calculate impulse:
j = (-(1+fCr) * (Collisions[i].vRelativeVelocity *
Collisions[i].vCollisionNormal)) /
((1/Bodies[b1].fMass + 1/Bodies[b2].fMass) +
(Collisions[i].vCollisionNormal * ( ( (pt1 ^
 Collisions[i].vCollisionNormal) *
 Bodies[b1].mIeInverse )^pt1) ) +
(Collisions[i].vCollisionNormal * ( ( (pt2 ^
 Collisions[i].vCollisionNormal) *
 Bodies[b2].mIeInverse )^pt2) ) );
Vrt = Collisions[i].vRelativeVelocity *
  Collisions[i].vCollisionTangent;
if(fabs(Vrt) > 0.0 && dofriction) {
Bodies[b1].vVelocity +=
((j * Collisions[i].vCollisionNormal) +
((muB * j) * Collisions[i].vCollisionTangent)) /
Bodies[b1].fMass;
Bodies[b1].vAngularVelocityGlobal +=
(pt1 ^ ((j * Collisions[i].vCollisionNormal) +
((muB * j) * Collisions[i].vCollisionTangent))) *
Bodies[b1].mIeInverse;
Bodies[b1].vAngularVelocity =
quaternion_vector_rotate(~Bodies[b1].qOrientation,
Bodies[b1].vAngularVelocityGlobal);
Bodies[b2].vVelocity +=
((-j * Collisions[i].vCollisionNormal) + ((muB *
j) * Collisions[i].vCollisionTangent)) /
Bodies[b2].fMass;
Bodies[b2].vAngularVelocityGlobal +=
(pt2 ^ ((-j * Collisions[i].vCollisionNormal) +
((muB * j) * Collisions[i].vCollisionTangent)))
* Bodies[b2].mIeInverse;
Bodies[b2].vAngularVelocity =
quaternion_vector_rotate(~Bodies[b2].qOrientation,
Bodies[b2].vAngularVelocityGlobal);
   } else {
   // Apply impulse:
   Bodies[b1].vVelocity +=
   (j * Collisions[i].vCollisionNormal) /
   Bodies[b1].fMass;
   Bodies[b1].vAngularVelocityGlobal +=
   (pt1 ^ (j * Collisions[i].vCollisionNormal)) *
   Bodies[b1].mIeInverse;
   Bodies[b1].vAngularVelocity =
   quaternion_vector_rotate(~Bodies[b1].qOrientation,
   Bodies[b1].vAngularVelocityGlobal);
   Bodies[b2].vVelocity -=
   (j * Collisions[i].vCollisionNormal) /
   Bodies[b2].fMass;
   Bodies[b2].vAngularVelocityGlobal -=
   (pt2 ^ (j * Collisions[i].vCollisionNormal)) *
   Bodies[b2].mIeInverse;
   Bodies[b2].vAngularVelocity =
   quaternion_vector_rotate(~Bodies[b2].qOrientation,
   Bodies[b2].vAngularVelocityGlobal);
   }
  } else { // Ground plane:
  fCr = COEFFICIENTOFRESTITUTIONGROUND;
  pt1 = Collisions[i].vCollisionPoint - Bodies[b1].vPosition;
   // Calculate impulse:
   j = (-(1+fCr) * (Collisions[i].vRelativeVelocity *
   Collisions[i].vCollisionNormal)) /
   ( (1/Bodies[b1].fMass) +
   (Collisions[i].vCollisionNormal *
   ( ( (pt1 ^ Collisions[i].vCollisionNormal) *
   Bodies[b1].mIeInverse )^pt1)));
   Vrt = Collisions[i].vRelativeVelocity *
Collisions[i].vCollisionTangent;
   if(fabs(Vrt) > 0.0 && dofriction) {
   Bodies[b1].vVelocity +=
   ( (j * Collisions[i].vCollisionNormal) + ((muG *
   j) * Collisions[i].vCollisionTangent) ) /
   Bodies[b1].fMass;
   Bodies[b1].vAngularVelocityGlobal +=
   (pt1 ^ ((j * Collisions[i].vCollisionNormal) +
   ((muG * j) * Collisions[i].vCollisionTangent))) *
   Bodies[b1].mIeInverse;
   Bodies[b1].vAngularVelocity =
   quaternion_vector_rotate(~Bodies[b1].qOrientation,
   Bodies[b1].vAngularVelocityGlobal);
   } else {
// Apply impulse:
Bodies[b1].vVelocity +=
(j * Collisions[i].vCollisionNormal) /
Bodies[b1].fMass;
Bodies[b1].vAngularVelocityGlobal +=
(pt1 ^ (j * Collisions[i].vCollisionNormal)) *
Bodies[b1].mIeInverse;
Bodies[b1].vAngularVelocity =
quaternion_vector_rotate(~Bodies[b1].qOrientation,
Bodies[b1].vAngularVelocityGlobal);
}
   }
  }
  }
}

int   CheckGroundPlaneContacts(pCollision CollisionData, int body1)
{
  vector3d   v1[8];
  vector3d   tmp;
  vector3d   u, v;
  vector3d   f[4];
  vector3d   vel1;
  vector3d   pt1;
  vector3d   Vr;
  float Vrn;
  vector3d   n;
  int  status = NOCOLLISION;
  vector3d   Ar;
  float Arn;
  if(Bodies[body1].vPosition.z <= (Bodies[body1].fRadius + COLLISIONTOLERANCE))
  {
  pt1 = Bodies[body1].vPosition;
  pt1.z = COLLISIONTOLERANCE;
  tmp = pt1;
  pt1 = pt1-Bodies[body1].vPosition;
  vel1 = Bodies[body1].vVelocity/*Body*/ +
(Bodies[body1].vAngularVelocityGlobal^pt1);
   n.x = 0;
   n.y = 0;
n.z = 1;
Vr = vel1;
Vrn = Vr * n;
if(fabs(Vrn) <= VELOCITYTOLERANCE) // at rest
{
// Check the relative acceleration:
Ar = Bodies[body1].vAcceleration +
(Bodies[body1].vAngularVelocityGlobal ^
(Bodies[body1].vAngularVelocityGlobal^pt1)) +
(Bodies[body1].vAngularAccelerationGlobal ^ pt1);
  Arn = Ar * n;
  if(Arn <= 0.0f)
  {
  // We have a contact so fill the data structure
  assert(NumCollisions < (NUMBODIES*8));
  if(NumCollisions < (NUMBODIES*8))
  {
  CollisionData->body1 = body1;
  CollisionData->body2 = -1;
  CollisionData->vCollisionNormal = n;
  CollisionData->vCollisionPoint = tmp;
  CollisionData->vRelativeVelocity = Vr;
  CollisionData->vRelativeAcceleration = Ar;
   CollisionData->vCollisionTangent = (n^Vr)^n;
   CollisionData->vCollisionTangent.inverse();
   CollisionData->vCollisionTangent.normalize();
   CollisionData++;
   NumCollisions++;
   status = CONTACT;
   }
  }
}
   }
   return status;
  }

void FlushCollisionData()
{
for(int i=0; i<32; i++)
{
memset(&Collisions[i], 0, sizeof(Collision));
}
}
