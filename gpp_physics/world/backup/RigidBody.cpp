

#include"../math/math.h"
#include"../geometry/geometry.h"
#include"WorldInfo.h"
#include"RigidBody.h"
#include"../octree/octree.h"
#include"../collision/CollisionInfo.h"

using namespace std;

namespace gpp{

RigidBody::RigidBody(){
hbody=NULL;
cleanup();
}

RigidBody::~RigidBody(){
if(hbody){
delete hbody;
hbody=NULL;
}
cleanup();
}

string RigidBody::toString()const{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Exibindo objeto "<<id<<endl;
ss<<"Id: "<<id<<", Restitution: "<<restitution<<endl;
ss<<"Mass: "<<mass<<", InvMass:"<<invMass<<endl;
ss<<"staticFriction: "<<staticFriction<<", dynamicFriction"<<dynamicFriction<<endl;
ss<<"LinearDamping:"<<linearDamping<<", AngularDamping:"<<angularDamping<<endl;
ss<<"Velocity: "<<linearVelocity<<endl;
ss<<"Angular velocity: "<<angularVelocity<<endl;
ss<<"Forces: "<<forces<<endl;
ss<<"Torque: "<<torque<<endl;
ss<<hbody->toString()<<endl;
return ss.str();
}

void RigidBody::cleanup(){
id=0;
rbflags=0;
mass=0.0f;
restitution=0.0f;
staticFriction=0.0f;
dynamicFriction=0.0f;
userData=NULL;
hnode=NULL;
if(hbody!=NULL)delete hbody;
hbody=NULL;
}

 void RigidBody::translate(const vector3d& ts){
if(hbody!=NULL&&mass>0.0f){
 hbody->translate(ts);
if(hnode!=NULL){
hnode->getOctree()->transiction(this);
}
}
}

 void RigidBody::rotate(const quaternion& orientation){
if(hbody!=NULL){
 hbody->rotate(orientation);
if(hnode!=NULL){
hnode->getOctree()->transiction(this);
}
}
}

 void RigidBody::rotate(const vector3d& origin, const quaternion& orientation){
if(hbody!=NULL) {
hbody->rotate(origin, orientation);
if(hnode!=NULL){
hnode->getOctree()->transiction(this);
}
}
}

bool RigidBody::preStep(WorldInfo* props){
_GASSERT(props!=NULL);
addForce(props->gravity*mass);
return true;
}

void RigidBody::step(decimal dt){
    if (invMass == 0.0f) return; // N�o atualizar corpos est�ticos

    // Atualizar a posi��o com a velocidade linear
    linearVelocity += (forces * invMass) * dt;
this->translate( linearVelocity * dt);

    // Atualizar a orienta��o com a velocidade angular
    angularVelocity += (torque * invMass) * dt;
quaternion q =( quaternion(angularVelocity, dt) * getOrientation())+numeric_limits<decimal>::epsilon();
rotate(getPosition(), q);
//orientation.normalize();

    // Aplicar amortecimento
    linearVelocity *= 1.0f - linearDamping * dt;
    angularVelocity *= 1.0f - angularDamping * dt;

    // Limpar for�as e torques para a pr�xima itera��o
    forces.zero();
    torque.zero();
}

void RigidBody::solveCollision(RigidBody* r2, gpp::CollisionInfo* info) {
    if (invMass == 0.0f && r2->invMass == 0.0f) return; // Sem colis�o se ambos forem est�ticos

    // Calcular a velocidade relativa
    vector3d relativeVelocity = r2->linearVelocity - linearVelocity;

    // Calcular a velocidade na dire��o da normal da colis�o
    decimal velocityAlongNormal = vector3d::dot(relativeVelocity, info->normal);

    // Se os corpos est�o se afastando, n�o calcular o impulso
    if (velocityAlongNormal > 0) return;

    // Calcular impulso escalar
    decimal e = std::min(restitution, r2->restitution); // Coeficiente de restitui��o (elasticidade)
    decimal j = -(1 + e) * velocityAlongNormal;
    j /= invMass + r2->invMass;

    // Impulso final
    vector3d impulse = j * info->normal;

    // Aplicar impulso aos dois corpos
if(invMass>0.0f){
    linearVelocity -= impulse * invMass;
}

if(r2->invMass>0.0f){
    r2->linearVelocity += impulse * r2->invMass;
}

    // Atrito
    vector3d friction = computeFriction(info->normal, relativeVelocity, staticFriction, dynamicFriction);
if(invMass>0.0f){
    linearVelocity -= friction * invMass;
}
if(r2->invMass>0.0f){
    r2->linearVelocity += friction * r2->invMass;
}

    // 3. Corrigir a interpenetra��o dos corpos (Positional Correction)
    const decimal percent = 0.8f;  // Quanto da penetra��o ser� corrigido (80% neste caso)
    const decimal slop = 0.01f;    // Toler�ncia para pequenas penetra��es
    decimal correctionDepth = (((info->depth - slop)>0.0f) ? info->depth - slop : 0.0f) / (invMass + r2->invMass);
//    decimal correctionDepth = std::max((info->depth - slop), 0.0f) / (invMass + r2->invMass);

    // Vetor de corre��o para mover os corpos para fora da interpenetra��o
    vector3d correction = correctionDepth * info->normal * percent;

    // Aplicar a corre��o de posi��o proporcional � massa inversa
if(invMass>0.0f){
this->translate(-( correction * invMass));
}

if(r2->invMass>0.0f){
    r2->translate( correction * r2->invMass);
}
}

vector3d RigidBody::computeFriction(const vector3d& normal, const vector3d& relativeVel, decimal staticFriction, decimal dynamicFriction) {
    // Calcular a componente tangencial da velocidade relativa
    vector3d tangent = relativeVel - (vector3d::dot(relativeVel, normal) ^ normal);

    // Normalizar o vetor tangente
    if (tangent.lengthSquared() > 0.0f) {
        tangent = vector3d::normalize(tangent);
    } else {
        return vector3d(0, 0, 0); // Sem atrito se n�o houver componente tangencial
    }

    // Calcular a magnitude da for�a de atrito din�mica
    decimal frictionMagnitude = -dynamicFriction * vector3d::dot(relativeVel, tangent);

    // Calcular a for�a de atrito tangencial
    vector3d friction = frictionMagnitude * tangent;

    // Verificar se est� dentro do limite de atrito est�tico
    decimal maxFriction = staticFriction * vector3d::dot(relativeVel, normal);
    if (friction.lengthSquared() > maxFriction * maxFriction) {
        // Se o atrito din�mico for maior que o est�tico, limitar o atrito � for�a est�tica m�xima
        friction = vector3d::normalize(friction) * maxFriction;
    }

    return friction;
}

void RigidBody::onLoop(){
}

void RigidBody::beginContact(RigidBody* rb){
}

void RigidBody::updateContact(RigidBody* rb){
}

void RigidBody::endContact(RigidBody* rb){
}
}
