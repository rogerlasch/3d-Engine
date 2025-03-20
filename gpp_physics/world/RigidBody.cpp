

#include"../common.h"
#include"../math/math.h"
#include"../geometry/geometry.h"
#include"../collision/collision.h"
#include"RigidBody.h"

using namespace std;

namespace gpp{
RigidBody::RigidBody(RigidBody* hparent, decimal mass, GeometricShape* shape) {
this->id=0;
this->mass=mass;
this->hbody=shape;
this->isStatic=false;
this->useGravity=true;
this->linearDamping=0.99f;
this->angularDamping=0.99f;
this->restitution=0.5f;
this->friction=0.5f;

    setMass(mass); // Atualiza massa e inércia
    linearVelocity = vector3d(0, 0, 0);
    angularVelocity = vector3d(0, 0, 0);
    force = vector3d(0, 0, 0);
    torque = vector3d(0, 0, 0);
pushBody(hparent);
}

RigidBody::~RigidBody(){
if(hbody){
delete hbody;
hbody=NULL;
}
else{
hbody=NULL;
}

if(hparent){
hparent->removeBody(this);
hparent=NULL;
}
removeAllBodies();
}

std::string RigidBody::toString() const {

stringstream ss;
ss<<fixed;
ss.precision(2);

ss<<"id="<<id<<endl;
ss<<"mass="<<mass<<", inverseMass="<<inverseMass<<endl;
ss<<"LinearVelocity="<<linearVelocity<<endl;
ss<<"angularVelocity="<<angularVelocity<<endl;
ss<<"restitution="<<restitution<<", friction="<<friction<<endl;
ss<<"linearDamping="<<linearDamping<<", angularDamping="<<angularDamping<<endl;
ss<<"isStatic="<<boolalpha<<isStatic<<", useGravity="<<boolalpha<<useGravity<<endl;
ss<<"hparent="<<boolalpha<<(hparent!=NULL)<<", childs.size="<<childs.size()<<endl;
ss<<hbody->toString()<<endl;
if(childs.size()>0){
ss<<"childs={"<<endl;
for(auto& it : childs){
ss<<it->toString()<<endl;
}
ss<<"}"<<endl;
}

return ss.str();
}

void RigidBody::setMass(decimal m) {
    mass = m;
    inverseMass = (m > 0) ? 1.0f / m : 0.0f;
isStatic=((m>0) ? false : true);
    updateInertia();
}

void RigidBody::setShape(GeometricShape* s){
if(this->hbody){
delete this->hbody;
this->hbody=NULL;
}

this->hbody=s;
}

void RigidBody::updateInertia() {
    if (hbody) {
        inertia = hbody->getInertiaTensor(mass);
        inverseInertia = inertia.inverse();
    } else {
        inertia = matrix3x3().identity();
        inverseInertia = inertia;
    }
}

void RigidBody::applyForce(const vector3d& f) {
    force += f;
}

void RigidBody::applyForceAtPoint(const vector3d& f, const vector3d& point) {
    force += f;
    torque += (point - hbody->getPosition()).cross(f);
}

void RigidBody::applyTorque(const vector3d& t) {
    torque += t;
}

void RigidBody::clearForces() {
    force = vector3d(0, 0, 0);
    torque = vector3d(0, 0, 0);
}

matrix4x4 RigidBody::getWorldTransform() const {
    return hbody->getTransform()->getWorldTransformMatrix();
}

vector3d RigidBody::getVelocityAtPoint(const vector3d& point) const {
    vector3d r = point - hbody->getPosition();
    return linearVelocity + angularVelocity.cross(r);
}

void RigidBody::update(decimal deltaTime) {
    if (isStatic) return;

    // Integração da velocidade linear
    linearVelocity += (force * inverseMass) * deltaTime;
hbody->translate(linearVelocity * deltaTime);

    // Integração da velocidade angular
    angularVelocity += (inverseInertia * torque) * deltaTime;
//    quaternion angularDelta(angularVelocity * deltaTime, 0.0);
vector3d rotationVector = angularVelocity * deltaTime;
decimal angle = rotationVector.length();
vector3d axis = rotationVector.normalize();

// Para rotações pequenas, w ˜ 1.0
quaternion angularDelta(axis * (angle * 0.5), 1.0);

hbody->rotate(angularDelta);

    // Aplicar damping
    linearVelocity *= linearDamping;
    angularVelocity *= angularDamping;

    // Limpar forças e torques
    clearForces();
}

void RigidBody::resolveCollision(RigidBody* other, CollisionInfo* info) {
    if (isStatic && other->isStatic) return; // Corpos estáticos não se movem

    // Vetor normal da colisão (aponta de A para B)
    vector3d normal = info->normal;

    // Ponto de colisão no espaço global
    vector3d collisionPoint = info->point;

    // Velocidades no ponto de colisão
    vector3d velocityA = getVelocityAtPoint(collisionPoint);
    vector3d velocityB = other->getVelocityAtPoint(collisionPoint);

    // Velocidade relativa
    vector3d relativeVelocity = velocityB - velocityA;

    // Componente da velocidade relativa na direção da normal
    decimal velocityAlongNormal = relativeVelocity.dot(normal);

    // Se os corpos já estão se afastando, não há necessidade de resolver a colisão
    if (velocityAlongNormal > 0) return;

    // Coeficiente de restituição (usamos o mínimo dos dois corpos)
    decimal e = std::min(restitution, other->restitution);

    // Calcula o impulso
    vector3d rA = collisionPoint - getTransform()->getPosition();
    vector3d rB = collisionPoint - other->getTransform()->getPosition();

    vector3d rACrossN = rA.cross(normal);
    vector3d rBCrossN = rB.cross(normal);

    decimal invMassA = inverseMass;
    decimal invMassB = other->inverseMass;

    decimal invInertiaA = (inverseInertia * rACrossN).cross(rA).dot(normal);
    decimal invInertiaB = (other->inverseInertia * rBCrossN).cross(rB).dot(normal);

    // Denominador do cálculo do impulso
    decimal denominator = invMassA + invMassB + invInertiaA + invInertiaB;

    // Impulso escalar
    decimal j = -(1 + e) * velocityAlongNormal / denominator;

    // Impulso vetorial
    vector3d impulse = normal * j;

    // Aplica o impulso aos corpos
    if (!isStatic) {
        linearVelocity -= impulse * invMassA;
        angularVelocity -= (inverseInertia * rA.cross(impulse));
    }

    if (!other->isStatic) {
        other->linearVelocity += impulse * invMassB;
        other->angularVelocity += (other->inverseInertia * rB.cross(impulse));
    }

/*
    // Aplicar atrito (após o impulso normal)
    vector3d tangent = relativeVelocity - normal * relativeVelocity.dot(normal);
    if (tangent.length() > 0) {
        tangent = tangent.normalize();

        // Calcula o impulso de atrito
        decimal jt = -relativeVelocity.dot(tangent);
        jt /= denominator;

        // Limita o impulso de atrito usando a lei de Coulomb (friction)
        decimal mu = std::min(friction, other->friction);
        vector3d frictionImpulse = tangent * std::min(jt, j * mu);

        // Aplica o impulso de atrito
        if (!isStatic) {
            linearVelocity -= frictionImpulse * invMassA;
            angularVelocity -= (inverseInertia * rA.cross(frictionImpulse));
        }

        if (!other->isStatic) {
            other->linearVelocity += frictionImpulse * invMassB;
            other->angularVelocity += (other->inverseInertia * rB.cross(frictionImpulse));
        }
    }
*/

    // Correção de penetração (opcional, para evitar sobreposição)
    const decimal penetrationSlop = 0.01f; // Tolerância de penetração
    const decimal percent = 0.99f; // Percentual de correção

    if (info->depth > 0) {
        vector3d correction = normal * (std::max(info->depth - penetrationSlop, 0.0) / (invMassA + invMassB)) * percent;

        if (!isStatic) {
            hbody->translate(-correction * invMassA);
        }

        if (!other->isStatic) {
            other->hbody->translate(correction * invMassB);
        }
    }
}

void RigidBody::rotate(decimal x, decimal y, decimal z){
hbody->rotateWithDecimal(x, y, z);
}

void RigidBody::rotate(const quaternion& q){
hbody->rotate(q);
}

void RigidBody::translate(const vector3d& translation){
hbody->translate(translation);
}

void RigidBody::pushBody(RigidBody* rb){

if(rb==NULL||childs.find(rb)!=childs.end()){
return;
}

if(rb->getParent()!=NULL){
rb->getParent()->removeBody(rb);
}

rb->setParent(this);
Transform* transform=hbody->getTransform();
transform->pushTransform(rb->getShape()->getTransform());
childs.insert(rb);
}

void RigidBody::removeBody(RigidBody* rb){
if(childs.find(rb)!=childs.end()){
childs.erase(rb);
rb->setParent(NULL);
Transform* transform=hbody->getTransform();
transform->removeTransform(rb->getShape()->getTransform());
}
}

void RigidBody::removeAllBodies(){

while(childs.size()>0){
removeBody(*childs.begin());
}

childs.clear();
}
}
