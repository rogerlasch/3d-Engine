#include <iostream>
#include<sstream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include"g_start.h"
#include"testes.h"
#include"mRigidBody.h"

using namespace gpp;
using namespace std;

#define _AIRDENCITY 1.223f
#define _GRAVITY -9.81f
#define _STEP 0.2f

class particle{
public:
float mass;
float airDragCoefficient;
float fspeed;
float radius;
vector3d pos;
vector3d vel;
vector3d acceleration;
vector3d forces;
particle(){
mass=1.0f;
airDragCoefficient=0.84f;
radius=2.5f;
pos={0,0,0};
vel={0,0,0};
forces={0,0,0};
}
string toString(){
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Mass: "<<mass<<endl;
ss<<"airDragCoefficient: "<<airDragCoefficient<<endl;
ss<<"Radius: "<<radius<<endl;
ss<<"fspeed: "<<fspeed<<endl;
ss<<"Pos: "<<pos<<endl;
ss<<"Aceleração: "<<acceleration<<endl;
ss<<"Vel: "<<vel<<endl;
ss<<"Forces: "<<forces<<endl;
return ss.str();
}
void stop(){
forces={0,0,0};
vel={0,0,0};
acceleration={0,0,0};
}
void calcLoads(){
vector3d v;
v+={0,0, mass*_GRAVITY};
applyForce(v);
    // Arrasto do ar
    vector3d velocityDir =vector3d::normalize(vel);
velocityDir .inverse();
    float area = GPP_PI * radius * radius;  // Área de seção transversal para uma esfera
    float dragMagnitude = 0.5 * _AIRDENCITY * area * vector3d::dotProduct(vel, vel) * airDragCoefficient;
    vector3d dragForce = dragMagnitude * velocityDir;
    applyForce(dragForce);

}
void applyForce(const vector3d& v){
forces+=v;
}
void updateEulerSimpletic(float dt){
acceleration=forces/mass;
vel=vel+acceleration*dt;
pos+=vel*dt;
forces={0,0,0};
fspeed=vel.length();
}
};

int main(){
G_STARTER gst;
particle pst;
pst.mass=500.0f;
pst.pos={0,0,100};
uint32 impulseCount=0;
bool aplying=false;
vector3d force={0,0,1024};
//pst.applyForce(vector3d(1024, 0, 0)*2048);
for(uint32 i=0; i<2500; i++){
pst.calcLoads();
pst.updateEulerSimpletic(_STEP);
_GINFO("{:.2f}:{:.2f}, v={:.2f}, t={:.2f}", pst.pos.x, pst.pos.z, pst.fspeed, (i*_STEP));
if(!aplying){
if(pst.pos.z<=pst.radius){
_GINFO("Aplicando série de impulsos...");
aplying=true;
pst.stop();
impulseCount=0;
impulseCount+=1;
force*=2;
pst.applyForce(force);
}
}
else{
pst.applyForce(force);
impulseCount++;
if(impulseCount>10){
_GINFO("Interrompendo impulsos...");
aplying=false;
impulseCount=0;
}
}
}
return 0;
}
