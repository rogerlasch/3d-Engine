


#ifndef RIGIDBODY2D_H
#define RIGIDBODY2D_H

#define _LINEARDRAGCOEFFICIENT 1.25f
#define _THRUSTFORCE 5.0f
#define _MAXTHRUST 30.0f
#define _MINTHRUST 0.0f
#define _DTHRUST 0.2f
#define _STEERINGFORCE 3.0f
#define _RHO 0.6f

vector3d VRotate2d(float angle, const vector3d& v);

class rigidbody2d
{
public:
float fMass;
float fInertia;
float fInertiaInverse;
float fSpeed;
float fOrientation;
float fWidth, fLength, fHeight;
float ProjectedArea;
float ThrustForce;
vector3d PThrust, SThrust;
vector3d vPosition;
vector3d vVelocity;
vector3d vVelocityBody;
vector3d vAngularVelocity;
vector3d vMoments;
vector3d vForces;
vector3d CD;
vector3d CT;
vector3d CPT;
vector3d CST;
rigidbody2d()
{
fMass=100;
 fInertia=500;
 fInertiaInverse=1/fInertia;
 fSpeed=0.0f;
 fOrientation=0.0f;
 fWidth=10;
fLength=20;
fHeight=5;
 ThrustForce=0.0f;
 PThrust={0,0,0};
SThrust={0,0,0};
 vPosition={0,0,0};
 vVelocity={0,0,0};
 vVelocityBody={0,0,0};
 vAngularVelocity={0,0,0};
 vMoments={0,0,0};
 vForces={0,0,0};
CD.x = -0.25*fLength;
CD.y = 0.0f;
CD.z = 0.0f;

CT.x = -0.5*fLength;
CT.y = 0.0f;
CT.z = 0.0f;

CPT.x = 0.5*fLength;
CPT.y = -0.5*fWidth;
CPT.z = 0.0f;

CST.x = 0.5*fLength;
CST.y = 0.5*fWidth;
CST.z = 0.0f;
ProjectedArea=(fLength+fWidth)/2*fHeight;
ThrustForce=_THRUSTFORCE;
}
void calcLoads()
{
vector3d Fb;//Guarda a soma das for�as...
vector3d Mb;//guarda os momentos...
vector3d Thrust;//thrust vector
//redefine as for�as...
vForces={0,0,0};
vMoments={0,0,0};
Fb={0,0,0};
Mb={0,0,0};
// Define o vetor de empuxo, que atua atrav�s do CG da embarca��o
Thrust.x=1.0f;
Thrust*=ThrustForce;
// Calcula for�as e momentos no espa�o do corpo:
vector3d vLocalVelocity;
float fLocalSpeed=0.0f;
float tmp=0.0f;
vector3d vDragVector;
vector3d vResultant;
vector3d vtmp;
// Calcula a for�a de arrasto aerodin�mico:
// Calcula a velocidade local:
// A velocidade local inclui a velocidade devido a
// movimento linear da embarca��o,
// mais a velocidade em cada elemento
// devido � rota��o da embarca��o.
vtmp=vAngularVelocity^CD;//Parte rotacional...
vLocalVelocity+=vVelocityBody+vtmp;
// Calcula a velocidade do ar local
fLocalSpeed=vLocalVelocity.length();
// Encontre a dire��o na qual o arrasto ir� atuar.
 // O arrasto sempre age em linha com o relativo
// velocidade mas na dire��o oposta
if(fLocalSpeed>0.0001f)
{
vLocalVelocity.normalize();
vDragVector=vector3d::inverse(vLocalVelocity);
// Determina a for�a resultante no elemento.
tmp=0.5f*_RHO*fLocalSpeed*fLocalSpeed*ProjectedArea;
vResultant=vDragVector*_LINEARDRAGCOEFFICIENT*tmp;
// Mant�m um total corrente dessas for�as resultantes
Fb+=vResultant;
// Calcula o momento sobre o CG
 // e mant�m um total desses momentos
vtmp=CD^vResultant;
Mb+=vtmp;

}
// Calcule as for�as propulsoras de proa de bombordo e estibordo:
// Mantenha um total atualizado dessas for�as resultantes
Fb+=PThrust;
// Calcula o momento sobre o CG da for�a deste elemento
// e mant�m um total cont�nuo desses momentos (momento total)
vtmp=CPT^PThrust;
Mb+=vtmp;
// Mant�m um total dessas for�as resultantes (for�a total)
Fb+=SThrust;
// Calcula o momento sobre o CG da for�a deste elemento
// e mant�m um total cont�nuo desses momentos (momento total)
vtmp+=CST^SThrust;
Mb+=vtmp;
// Agora adicione o impulso de propuls�o
Fb+=Thrust;// nenhum momento desde que a linha de a��o � atrav�s do CG
// Converte for�as do espa�o do modelo para o espa�o terrestre
vForces=VRotate2d(fOrientation, Fb);
vMoments+=Mb;

}
void updateBodyEuler(float dt)
{
vector3d a;
vector3d dv;
vector3d ds;
float aa=0.0f;
float dav=0.0f;
float dr=0.0f;
calcLoads();
a=vForces/fMass;
dv=a*dt;
vVelocity+=dv;
ds=vVelocity*dt;
vPosition+=ds;
fSpeed=vVelocity.length();
// Integra a equa��o angular do movimento:
aa=vMoments.z/fInertia;
dav=aa*dt;
vAngularVelocity.z+=dav;
dr=radians_to_degrees(vAngularVelocity.z*dt);
fOrientation+=dr;
// Diversos c�lculos:
vVelocityBody=VRotate2d(-fOrientation, vVelocity);
}
void setThrusters(bool p, bool s)
{
   PThrust.x   =   0;
   PThrust.y   =   0;
   SThrust.x   =   0;
   SThrust.y   =   0;
   if(p)
   PThrust.y = _STEERINGFORCE;
   if(s)
   SThrust.y = -_STEERINGFORCE;
}
void modulateThrust(bool up)
{
float  dT = up ? _DTHRUST:-_DTHRUST;
   ThrustForce += dT;
   if(ThrustForce > _MAXTHRUST) ThrustForce = _MAXTHRUST;
   if(ThrustForce < _MINTHRUST) ThrustForce = _MINTHRUST;
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
ss<<"fOrientation:"<< fOrientation<<std::endl;
ss<<"fWidth:"<< fWidth<<std::endl;
ss<<"fLength:"<< fLength<<std::endl;
ss<<"fHeight:"<< fHeight<<std::endl;
ss<<"ThrustForce:"<< ThrustForce<<std::endl;
ss<<"PThrust:"<< PThrust<<std::endl;
ss<<"SThrust:"<<SThrust<<std::endl;
ss<<"vPosition:"<< vPosition<<std::endl;
ss<<"vVelocity:"<< vVelocity<<std::endl;
ss<<"vVelocityBody:"<< vVelocityBody<<std::endl;
ss<<"vAngularVelocity:"<< vAngularVelocity<<std::endl;
ss<<"vMoments:"<< vMoments<<std::endl;
ss<<"vForces:"<< vForces<<std::endl;
ss<<"CD:"<< CD<<std::endl;
ss<<"CT:"<<CT<<std::endl;
ss<<"CPT:"<<CPT<<std::endl;
ss<<"CST:"<<CST<<std::endl;
ss<<"ProjectedArea:"<<ProjectedArea<<std::endl;
ss<<"ThrustForce:"<<ThrustForce<<std::endl;
return ss.str();
}
};
typedef std::shared_ptr<rigidbody2d> shared_rigidbody2d;
typedef std::vector<shared_rigidbody2d> rigidbody2dlist;

vector3d VRotate2d(float angle, const vector3d& v)
{
float x=0.0f, y=0.0f;
   x = v.x * cos(degrees_to_radians(-angle)) +
   v.y * sin(degrees_to_radians(-angle));
   y = -v.x * sin(degrees_to_radians(-angle)) +
   v.y * cos(degrees_to_radians(-angle));
return vector3d(x, y, 0.0f);
}
#endif
