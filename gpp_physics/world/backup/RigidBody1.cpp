

#include"../common.h"
#include"../math/math.h"
#include"../geometry/geometry.h"
#include"../collision/collision.h"
#include"RigidBody.h"

using namespace std;

namespace gpp{
RigidBody::RigidBody(RigidBody* hparent, uint32 bFlags, decimal mass, GeometricShape* shape) {
this->hworld=NULL;
this->hparent=hparent;
this->id=0;
this->bFlags=bFlags;
this->mass=mass;
this->hbody=shape;
this->linearDamping=0.99f;
this->angularDamping=0.99f;
this->restitution=0.5f;
this->friction=0.5f;

    setMass(mass); // Atualiza massa e inércia
    linearVelocity = vector3d(0, 0, 0);
    angularVelocity = vector3d(0, 0, 0);
    force = vector3d(0, 0, 0);
    torque = vector3d(0, 0, 0);

if(hparent){
hparent->pushBody(this);
}
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

AABB ab;
getAABB(&ab);

ss<<"RigidBody: "<<"id="<<id<<", ParentId="<<((hparent==NULL) ? 0 : hparent->getId())<<endl;
ss<<hbody->getShortDescription()<<endl;
ss<<"mass="<<mass<<", inverseMass="<<inverseMass<<endl;
ss<<"Position="<<hbody->getPosition()<<", Scale="<<hbody->getTransform()->getScale()<<endl;
ss<<"Orientation Euler="<<quaternion_extract_euler_angles(hbody->getTransform()->getOrientation())<<", Orientation="<<hbody->getTransform()->getOrientation()<<endl;
ss<<"LinearVelocity="<<linearVelocity<<endl;
ss<<"angularVelocity="<<angularVelocity<<endl;
ss<<"restitution="<<restitution<<", friction="<<friction<<endl;
ss<<"linearDamping="<<linearDamping<<", angularDamping="<<angularDamping<<endl;
ss<<"hparent="<<boolalpha<<(hparent!=NULL)<<", childs.size="<<childs.size()<<endl;
ss<<"aabb="<<ab.getShortDescription()<<endl;
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
    updateInertia();
enableFlag(BF_STATIC, mass<0.00001f);
}

void RigidBody::setShape(GeometricShape* s){
if(this->hbody){
delete this->hbody;
this->hbody=NULL;
}

this->hbody=s;
}

bool RigidBody::rayCast(RayInfo* info){
return hbody->rayCast(info);
}

bool RigidBody::rayCast(const vector3d& origin, const vector3d& dir, std::vector<RayInfo>& infos){

RayInfo info;

info.origin=origin;
info.dir=dir;

if(this->rayCast(&info)){
infos.push_back(info);
}

for(auto& it : childs){
it->rayCast(origin, dir, infos);
}

return infos.size()>0;
}

void RigidBody::calculateForces(const vector3d& gravity){

if(hasFlag(BF_STATIC)){
return;
}

vector3d accForces;

if(!hasFlag(BF_GRAVITY)){
accForces+=gravity*mass;
}

if(collisionObjects.size()>0){
//Força normal, provavelmente...
}

//Outras forças, se necessário...

if(!hasFlag(BF_SLEEPING)){
if(linearVelocity.length()<0.5f){
setLinearVelocity({0,0,0});
enableFlag(BF_SLEEPING, true);
}
}

if(accForces.length()>0.0f){
applyForce(accForces);
}
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

void RigidBody::enableFlag(uint32 hFlag, bool b){

if(b){
if((bFlags&hFlag)==0){
bFlags|=hFlag;
}
}
else{
if((bFlags&hFlag)==hFlag){
bFlags^=hFlag;
}
}
}

bool RigidBody::hasFlag(uint32 hFlags)const{
return (bFlags&hFlags)>0;
}

void RigidBody::applyForce(const vector3d& f) {

if(!hasFlag(BF_STATIC)){
    force += f;
enableFlag(BF_SLEEPING, false);
}
}

void RigidBody::applyForceAtPoint(const vector3d& f, const vector3d& point) {

if(!hasFlag(BF_STATIC)){
    force += f;
    torque += (point - hbody->getPosition()).cross(f);
enableFlag(BF_SLEEPING, false);
}
}

void RigidBody::applyTorque(const vector3d& t) {

if(!hasFlag(BF_STATIC)){
    torque += t;
}

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

if(!hasFlag(BF_STATIC|BF_SLEEPING)){
    // Integração da velocidade linear
    linearVelocity += (force * inverseMass) * deltaTime;
this->translate(linearVelocity * deltaTime);

    // Integração da velocidade angular
    angularVelocity += (inverseInertia * torque) * deltaTime;
//    quaternion angularDelta(angularVelocity * deltaTime, 0.0);
vector3d rotationVector = angularVelocity * deltaTime;
decimal angle = rotationVector.length();
vector3d axis = rotationVector.normalize();

// Para rotações pequenas, w ˜ 1.0
quaternion angularDelta(axis * (angle * 0.5), 1.0);

this->rotate(angularDelta);
updateInertia();

    // Aplicar damping
    linearVelocity *= linearDamping;
    angularVelocity *= angularDamping;

}

    // Limpar forças e torques
    clearForces();

if(childs.size()>0){
for(auto& it : childs){
it->update(deltaTime);
}
}
}

void RigidBody::resolveCollision(RigidBody* other, CollisionInfo* info) {

uint32 dontFlags=BF_STATIC|BF_SENSOR|BF_REGION;
if((hasFlag(dontFlags))&&(other->hasFlag(dontFlags))){
return;
}

dontFlags^=BF_STATIC;
if((hasFlag(dontFlags))||(other->hasFlag(dontFlags))){
return;
}

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
    if (!hasFlag(BF_STATIC)) {
        linearVelocity -= impulse * invMassA;
        angularVelocity -= (inverseInertia * rA.cross(impulse));
    }

    if (!other->hasFlag(BF_STATIC)) {
        other->linearVelocity += impulse * invMassB;
        other->angularVelocity += (other->inverseInertia * rB.cross(impulse));
    }

/**
*Resolver mais tarde. Está causando instabilidade numérica.

    // Aplicar atrito (após o impulso normal)
    vector3d tangent = relativeVelocity - normal * relativeVelocity.dot(normal);
    if (tangent.length() > 1e-9) {
        tangent.normalize();

        // Calcula o impulso de atrito
        decimal jt = -relativeVelocity.dot(tangent);
        jt /= denominator;
        // Limita o impulso de atrito usando a lei de Coulomb (friction)
        decimal mu = std::min(friction, other->friction);
        vector3d frictionImpulse = tangent * std::min(jt, j * mu)+numeric_limits<decimal>::epsilon();
        // Aplica o impulso de atrito
        if (!hasFlag(BF_STATIC) {
            linearVelocity -= frictionImpulse * invMassA;
            angularVelocity -= (inverseInertia * rA.cross(frictionImpulse));
        }

        if (!other->hasFlag(BF_STATIC) {
            other->linearVelocity += frictionImpulse * invMassB;
            other->angularVelocity += (other->inverseInertia * rB.cross(frictionImpulse));
        }
    }
*/

    // Correção de penetração (opcional, para evitar sobreposição)
    const decimal penetrationSlop = 0.01f; // Tolerância de penetração
    const decimal percent = 0.95f; // Percentual de correção

    if (info->depth > 0) {
        vector3d correction = normal * (std::max(info->depth - penetrationSlop, 0.0) / (invMassA + invMassB)) * percent;

        if (!hasFlag(BF_STATIC)) {
            this->translate(-correction * invMassA);
        }

        if (!other->hasFlag(BF_STATIC)) {
            other->translate(correction * invMassB);
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

void RigidBody::getAABB(AABB* ab)const{
hbody->getAABB(ab);

if(childs.size()>0){
AABB ab2;
for(auto& it : childs){
it->getAABB(&ab2);
ab->addAABB(ab2);
}
}
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

void RigidBody::registerCollisionCallback(uint32 type, COLLISIONNOTIFICATIONCALLBACK hcall){
CollisionNotifications[type]=hcall;
}

void RigidBody::removeCollisionCallback(uint32 type){
auto it=CollisionNotifications.find(type);
if(it!=CollisionNotifications.end()){
CollisionNotifications.erase(type);
}
}

void RigidBody::executeCollisionCallback(uint32 type, RigidBody* rb, CollisionInfo* info){

auto it=CollisionNotifications.find(type);

if(it!=CollisionNotifications.end()){
it->second(this, rb, info);
}
}
}
