

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
this->staticFriction=0.5f;
this->dynamicFriction=0.4f;

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
ss<<"restitution="<<restitution<<", staticFriction="<<staticFriction<<", dynamicFriction="<<dynamicFriction<<endl;
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

void RigidBody::calculateForces(const vector3d& gravity) {
    if(hasFlag(BF_STATIC)) return;

    vector3d accForces;

    if(hasFlag(BF_GRAVITY)) {
//        accForces += gravity * mass;
    }

    // Verificação de repouso mais robusta
    if(!hasFlag(BF_SLEEPING)) {
    }

    if(accForces.lengthSquared() >= 1.0f) {
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

if (angularVelocity.lengthSquared() > 1e-12f) {
    vector3d axis = angularVelocity.normalize();
    decimal angle = angularVelocity.length() * deltaTime;
    quaternion angularDelta(axis * std::sin(angle/2), std::cos(angle/2));
    this->rotate(angularDelta);
updateInertia();
}

    // Aplicar damping
    linearVelocity *= linearDamping;
    angularVelocity *= angularDamping;
    // Limpar forças e torques
    clearForces();

}


if(childs.size()>0){
for(auto& it : childs){
it->update(deltaTime);
}
}
}

void RigidBody::resolveCollision(RigidBody* other, CollisionInfo* info) {
    uint32 dontFlags = BF_STATIC|BF_SENSOR|BF_REGION;
    if((hasFlag(dontFlags))&&(other->hasFlag(dontFlags))){
        return;
    }
    dontFlags ^= BF_STATIC;
    if(hasFlag(dontFlags)){
        return;
    }
    // 1. Preparação dos dados da colisão
    vector3d normal = info->normal;
    vector3d collisionPoint = info->point;
    // 2. Velocidades no ponto de contato
    vector3d velocityA = getVelocityAtPoint(collisionPoint);
    vector3d velocityB = other->getVelocityAtPoint(collisionPoint);
    vector3d relativeVelocity = velocityB - velocityA;
    // 3. Componente normal da velocidade relativa
    decimal velocityAlongNormal = relativeVelocity.dot(normal);
    // 4. Se os objetos estão se separando, não resolva
    if (velocityAlongNormal > 0) return;
    // 5. Coeficiente de restituição combinado
    decimal e = std::min(restitution, other->restitution);
    // 6. Vetores do centro de massa ao ponto de contato
    vector3d rA = collisionPoint - getTransform()->getPosition();
    vector3d rB = collisionPoint - other->getTransform()->getPosition();
    // 7. Termos de inércia
    vector3d rACrossN = rA.cross(normal);
    vector3d rBCrossN = rB.cross(normal);
    // 8. Denominador para cálculo do impulso
    decimal denominator = inverseMass + other->inverseMass;
    denominator += (inverseInertia * rACrossN).cross(rA).dot(normal);
    denominator += (other->inverseInertia * rBCrossN).cross(rB).dot(normal);
    // 9. Impulso normal
    decimal j = -(1 + e) * velocityAlongNormal;
    j /= denominator;
    vector3d impulse = normal * j;
    // 10. Aplicação do impulso normal
    if (!hasFlag(BF_STATIC)) {
        linearVelocity -= impulse * inverseMass;
        angularVelocity -= inverseInertia * rA.cross(impulse);
    }
    if (!other->hasFlag(BF_STATIC)) {
        other->linearVelocity += impulse * other->inverseMass;
        other->angularVelocity += other->inverseInertia * rB.cross(impulse);
    }
    // --------------------------------------------------
    // PARTE DE ATRITO - IMPLEMENTAÇÃO COMPLETA
    // --------------------------------------------------
    // 11. Recalcular velocidades após impulso normal
    velocityA = getVelocityAtPoint(collisionPoint);
    velocityB = other->getVelocityAtPoint(collisionPoint);
    relativeVelocity = velocityB - velocityA;
    // 12. Componente tangencial da velocidade
    vector3d tangent = relativeVelocity - normal * relativeVelocity.dot(normal);
    // 13. Se houver movimento tangencial significativo
    if (tangent.lengthSquared() > 1e-8f) {
        tangent.normalize();
        // 14. Coeficientes de atrito combinados
        decimal staticMu = std::sqrt(
            staticFriction * staticFriction +
            other->staticFriction * other->staticFriction
        );
        decimal dynamicMu = std::sqrt(
            dynamicFriction * dynamicFriction +
            other->dynamicFriction * other->dynamicFriction
        );
        // 15. Magnitude da velocidade tangencial
        decimal tangentSpeed = relativeVelocity.dot(tangent);
        // 16. Impulso tangencial necessário para parar o movimento
        decimal jt = -tangentSpeed*2.0f;
        jt /= denominator;
        // 17. Lei de Coulomb - escolhe entre atrito estático e dinâmico
        vector3d frictionImpulse;
        if (std::abs(jt) < j * staticMu) {
            // Atrito estático - impede o início do deslizamento
            frictionImpulse = tangent * jt;
        } else {
            // Atrito dinâmico - reduz a velocidade de deslizamento
            frictionImpulse = tangent * (-j * dynamicMu);
        }
        // 18. Aplicação do atrito
        if (!hasFlag(BF_STATIC)) {
            linearVelocity -= frictionImpulse * inverseMass;
            angularVelocity -= inverseInertia * rA.cross(frictionImpulse);
        }
        if (!other->hasFlag(BF_STATIC)) {
            other->linearVelocity += frictionImpulse * other->inverseMass;
            other->angularVelocity += other->inverseInertia * rB.cross(frictionImpulse);
        }
    }
    // 19. Correção de penetração (opcional)
    const decimal penetrationSlop = 0.01f;
const decimal percent=0.85;
    if (info->depth > penetrationSlop) {
        vector3d correction = normal * (info->depth - penetrationSlop) / (inverseMass + other->inverseMass) * percent;
        if (!hasFlag(BF_STATIC)) {
            translate(-correction * inverseMass);
        }
        if (!other->hasFlag(BF_STATIC)) {
            other->translate(correction * other->inverseMass);
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
