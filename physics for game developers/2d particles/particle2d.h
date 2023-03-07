


#ifndef PARTICLE2D_H
#define PARTICLE2D_H

#define CNT_GRAVITY -9.8f
#define _AIRDENSITY 1.23f
#define _WINDSPEED 10.0f
#define _DRAGCOEFFICIENT 0.6f
#define _GROUND_PLANE 0.0f

class particle2d
{
public:
float fMass;
float fSpeed;
float fRadius;
float fRestitution;
vector3d vPosition;
vector3d vPreviousPosition;
vector3d vVelocity;
vector3d vForces;
vector3d vImpactForces;
vector3d vGravity;
bool bCollision;
particle2d(float mass=1.0f, float radius=1.0f)
{
fMass=mass;
fSpeed=0.0f;
fRadius=radius;
fRestitution=0.30f;
vPosition={0,0,0};
vPreviousPosition={0,0,0};
vVelocity={0,0,0};
vForces={0,0,0};
vImpactForces={0,0,0};
vGravity={0,(fMass*CNT_GRAVITY),0};
bCollision=false;
}
void calcLoads()
{
//Limpar as coisas...
vForces={0,0,0};
if(bCollision)
{
vForces+=vImpactForces;
}
else
{
//Gravidade...
vForces+=vGravity;
//Arrasto do ar...
vector3d vDrag;
float fDrag=0.0f;
vDrag-=vVelocity;
vDrag.normalize();
fDrag=0.5*_AIRDENSITY*fSpeed*fSpeed*(GPP_PI*fRadius*fRadius)*_DRAGCOEFFICIENT;
vDrag*=fDrag;
vForces+=vDrag;
//Vento:
vector3d vWind;
vWind.x=0.5*_AIRDENSITY*_WINDSPEED*_WINDSPEED*(GPP_PI*fRadius*fRadius)*_DRAGCOEFFICIENT;
vForces+=vWind;
}
}
void updateBodyEuler(float dt)
{
vector3d a;//Aceleração...
vector3d dv;
vector3d ds;
a=vForces/fMass;
dv=a*dt;
vVelocity+=dv;
ds=vVelocity*dt;
vPreviousPosition=vPosition;
vPosition+=ds;
fSpeed=vVelocity.length();
}
std::string toString()const
{
std::stringstream ss;
ss<<std::fixed;
ss.precision(2);
ss<<"fMass:"<<fMass<<std::endl;
ss<<"fSpeed:"<<fSpeed<<std::endl;
ss<<"fRadius:"<<fRadius<<std::endl;
ss<<"fRestitution:"<<fRestitution<<std::endl;
ss<<"vPosition:"<<vPosition<<std::endl;
ss<<"vVelocity:"<<vVelocity<<std::endl;
ss<<"vForces:"<<vForces<<std::endl;
ss<<"vGravity:"<<vGravity<<std::endl;
return ss.str();
}
};
typedef std::shared_ptr<particle2d> shared_particle2d;
typedef std::vector<shared_particle2d> particle2dlist;
#endif
