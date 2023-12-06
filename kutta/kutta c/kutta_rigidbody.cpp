#include <iostream>
#include "vec.h"

using namespace std;

vector3d fCalculateAirDrag(const vector3d& velocity, float projectedArea, float airDencity, float airDragCoefficient) ;
class RigidBody{
public:
float mass;
vector3d pos;
vector3d vel;
vector3d forces;
RigidBody(float mass=1.0f, const vector3d& pos={0,0,0}, const vector3d vel={0,0,0}){
this->mass=mass;
this->pos=pos;
this->vel=vel;
forces={0,0,0};
}
void calcLoads(){
forces={0,0,0};
//forces={0,0, -9.81f*mass};
forces+=fCalculateAirDrag(vel, 100.0f, 1.223f, 0.35f);
}
void applyForce(const vector3d f){
this->forces+=f;
}
};

struct derivative{
vector3d dx={0,0,0};  // dx/dt = velocidade
        vector3d dv={0,0,0};  // dv/dt = aceleração
};

void evaluate(RigidBody* rb, float t, float dt, derivative* d, derivative* output){
output->dx=rb->vel+(d->dv*dt);
output->dv=rb->forces/rb->mass;
}

void rk4Update(RigidBody* rb, float t, float dt){
    derivative k0, a, b, c, d;
evaluate(rb, t, 0.0f, &k0, &a);
evaluate(rb, t, dt * 0.5f, &a, &b);
evaluate(rb, t, dt * 0.5f, &b, &c);
evaluate(rb, t, dt, &c, &d);
    vector3d dxdt = (1.0f / 6.0f) * (a.dx + 2.0f * (b.dx + c.dx) + d.dx);
    vector3d dvdt = (1.0f / 6.0f) * (a.dv + 2.0f * (b.dv + c.dv) + d.dv);
rb->pos = rb->pos + dxdt * dt;
rb->vel = rb->vel + dvdt * dt;
}

vector3d fCalculateAirDrag(const vector3d& velocity, float projectedArea, float airDencity, float airDragCoefficient) {
float vel=velocity.length();
if (vel < 1.0f) {
    return vector3d(0.0f, 0.0f, 0.0f);
}
vector3d vdir;
vdir -= velocity;
vdir.normalize();
//float fd=0.5f*airDencity*vel*vel*projectedArea*airDragCoefficient;
float fd=airDragCoefficient*0.5f*airDencity*vel*vel*projectedArea;
return vdir*fd;
}

int main(){
setlocale(LC_ALL, "Portuguese");
cout<<fixed;
cout.precision(2);
float step=0.01f;
RigidBody rb(5.0f, {50,50,50}, {0,0,0});
float f=2500000.0f;
for(int i=0; i<2500; i++){
if((i%10)==0){
        cout << "Iteração " << i << ": Posição = (" << rb.pos.x << ", " << rb.pos.y << ", " << rb.pos.z << "), Velocidade = (" << rb.vel.x << ", " << rb.vel.y << ", " << rb.vel.z << ")" << endl;
}
rb.calcLoads();
if((i>=15)&&(i<=35)){
vector3d dir={1,0,0};
dir=dir*(f*step);
rb.applyForce(dir);
cout<<"Impulso de "<<dir.x<<" newtons aplicado!"<<endl;
}
rk4Update(&rb, (i*step), step);
}

return 0;
}
