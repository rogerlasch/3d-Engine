
#define GPP_DOUBLE_PRECISION
#include <iostream>
#include <gpp/gpp.h>
#include<gpp_physics/gpp_physics.h>
#include"g_start.h"
#include"teste.h"

using namespace gpp;
using namespace std;


using namespace gpp;

void testApplyForceAtPoint() {
    // Cria um cubo (Box3d) com dimensões 2x2x2
    Box3d* cubeShape = new Box3d(vector3d(1.0f, 1.0f, 1.0f));

    // Cria um corpo rígido para o cubo
    RigidBody cube(NULL, 1.0f, cubeShape); // Massa = 1.0

    // Define um ponto de aplicação da força (um canto do cubo)
    vector3d forcePoint = vector3d(1.0f, 1.0f, 1.0f); // Canto superior direito

    // Aplica uma força para cima no canto do cubo
    vector3d force = vector3d(0, 10.0f, 0); // Força de 10 unidades para cima
    cube.applyForceAtPoint(force, forcePoint);

    // Simula a física por alguns passos
    for (int i = 0; i < 10; ++i) {
        cube.update(0.1f); // Atualiza a física com deltaTime = 0.1 segundos
        cout << "Posição: " << cube.getTransform()->getPosition() << endl;
        cout << "Orientação: " << quaternion_extract_euler_angles(cube.getTransform()->getOrientation()).toString() << endl;
    }
cout<<cube.toString();
}

void testCollision() {
    // Cria duas esferas
    Sphere3d* sphereA = new Sphere3d(1.5f);
    Sphere3d* sphereB = new Sphere3d(1.5f);
sphereB->translate({2.5,0,0});
    // Cria dois corpos rígidos
    RigidBody bodyA(NULL, 1.5f, sphereA);
    RigidBody bodyB(NULL, 1.5f, sphereB);

    // Define velocidades iniciais
    bodyA.setLinearVelocity(vector3d(1, 0, 0));
    bodyB.setLinearVelocity(vector3d(-1, 0, 0));

    // Simula uma colisão
    CollisionInfo info;
    info.normal = vector3d(1, 0, 0);
    info.point = vector3d(1.5, 0, 0);
    info.depth = 0.5f;

    // Resolve a colisão
    bodyA.resolveCollision(&bodyB, &info);

    // Exibe os resultados
    cout << "Body A Velocity: " << bodyA.getLinearVelocity() << endl;
    cout << "Body B Velocity: " << bodyB.getLinearVelocity() << endl;
logger::info("bodyA=\n{}", bodyA.toString());
logger::info("bodyB=\n{}", bodyB.toString());

}

int main() {
try{
G_STARTER gst;


Sphere3d* sphere=new Sphere3d(2.5f);
RigidBody rb(NULL, 5.0f, sphere);
rb.setFriction(0.5f);
sphere->translate({0,0, 12.5f});

Box3d* box=new Box3d({500000, 500000, 10});
RigidBody ground(NULL, 0.0f, box);
ground.setFriction(0.05f);

vector3d gravity={0,0, -9.81f};

int x=0;

decimal dt=1/5.0f;
while(x<5000){
x++;
rb.applyForce(gravity*rb.getMass());
if(rb.getTransform()->getPosition().x<500){
rb.applyForce({300, 0, 0});
}
rb.update(dt);
//logger::info("{:.2f}", rb.getTransform()->getPosition().z);

CollisionInfo info;
if(checkBoxSphereCollision(box, sphere, &info)){
//logger::info("{}", info.toString());
ground.resolveCollision(&rb, &info);
}
logger::info("p={:.2f}, {:.2f}", rb.getTransform()->getPosition().x, rb.getTransform()->getPosition().z);
logger::info("v={}", rb.getLinearVelocity().toString());
}

logger::info("Fim da simulação.\n{}", rb.toString());

} catch(const exception& e){
logger::info("Exception:\n{}", e.what());
}
    return 0;
}
