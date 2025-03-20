
#include <algorithm>
#include"../types.h"
#include"math.h"
#include "Transform.h"

using namespace std;

namespace gpp {

// Construtor padr�o
Transform::Transform() {
lastPosition={0,0,0};
position={0,0,0};
scale={1, 1, 1};
orientation=quaternion::fromEuler(0,0,0);
hparent=NULL;
setNeedUpdate(true);
updateLocalMatrix();
}

// Construtor com escala, �ngulos e transla��o
Transform::Transform(const vector3d& scale, const vector3d& angles, const vector3d& translation) {
lastPosition=translation;
this->position=translation;
this->scale=scale;
this->orientation=quaternion::fromEuler(angles.x, angles.y, angles.z);
this->hparent=NULL;
setNeedUpdate(true);
updateLocalMatrix();
}

// Construtor com pai, escala, �ngulos e transla��o
Transform::Transform(Transform* hparent, const vector3d& scale, const vector3d& angles, const vector3d& translation) {
this->lastPosition=translation;
this->position=translation;
this->scale=scale;
this->orientation=quaternion::fromEuler(angles.x, angles.y, angles.z);
this->hparent=NULL;
this->hparent=hparent;

    if (hparent) {
        hparent->pushTransform(this);
    }

setNeedUpdate(true);
updateLocalMatrix();
}

// Construtor de c�pia
Transform::Transform(const Transform& tr) {
*this=tr;
}

// Operador de atribui��o
Transform& Transform::operator=(const Transform& tr) {
    if (this != &tr) {
this->lastPosition=tr.lastPosition;
        this->position = tr.position;
        this->scale = tr.scale;
        this->orientation = tr.orientation;
        this->hparent = tr.hparent;
        this->childs = tr.childs;
setNeedUpdate(true);
    }
    return *this;
}

Transform::~Transform(){
if(hparent){
hparent->removeTransform(this);
}

for(auto& it : childs){
it->setParent(NULL);
}

childs.clear();
}

// Atualiza a matriz de transforma��o local
void Transform::updateLocalMatrix()const {

if(!getNeedUpdate()){
return;
}

    // Cria as matrizes individuais
    matrix4x4 translationMatrix = matrix4x4::getTranslation(position);
    matrix4x4 rotationMatrix = orientation.toMatrix4x4();
    matrix4x4 scaleMatrix = matrix4x4::getScale(scale);

    // Combina as transforma��es: T * R * S
    localMatrix = translationMatrix * rotationMatrix * scaleMatrix;
setNeedUpdate(false);
}

// Define a posi��o e notifica da necessidade de atualiza��o...
void Transform::setPosition(const vector3d& pos) {
setLastPosition(this->position);
    this->position = pos;
setNeedUpdate(true);
}

// Define a escala e notifica da necessidade de atualiza��o...
void Transform::setScale(const vector3d& scl) {
    this->scale = scl;
setNeedUpdate(true);
}

// Define a orienta��o e notifica da necessidade de atualiza��o...
void Transform::setOrientation(const quaternion& orient) {
    this->orientation = orient;
setNeedUpdate(true);
}

// Remove um filho da lista de filhos
void Transform::removeTransform(Transform* child) {
    if (childs.find(child)!=childs.end()){
        childs.erase(child);
child->setParent(NULL);
    }
}

// Adiciona um filho � lista de filhos
void Transform::pushTransform(Transform* child) {
if(child->getParent()!=NULL){
if(child->getParent()!=this){
child->getParent()->removeTransform(child);
child->setParent(this);
}
}
else{
child->setParent(this);
}

if(childs.find(child)==childs.end()){
    childs.insert(child);
child->setNeedUpdate(true);
}
}

// Obt�m a matriz de transforma��o global
matrix4x4 Transform::getWorldTransformMatrix() const {

if(getNeedUpdate()){
updateLocalMatrix();
}

    if (hparent) {
        return hparent->getWorldTransformMatrix() * localMatrix;
    }
    return localMatrix;
}

matrix4x4 Transform::getLocalMatrix()const{

if(getNeedUpdate()){
updateLocalMatrix();
}

return this->localMatrix;
}

// Transforma um ponto do espa�o local para o espa�o global
vector3d Transform::toGlobal(const vector3d& v)const  {

if(getNeedUpdate()){
updateLocalMatrix();
}

    matrix4x4 worldMatrix = getWorldTransformMatrix();
    return worldMatrix * v;
}

vector3d Transform::toLocal(const vector3d& v)const  {

if(getNeedUpdate()){
updateLocalMatrix();
}

    matrix4x4 worldMatrix = getWorldTransformMatrix().inverse();
    return worldMatrix * v;
}

void Transform::forceUpdateLocalMatrix(bool propagate){

setNeedUpdate(true);
updateLocalMatrix();

if(propagate){
for(auto& it : childs){
it->forceUpdateLocalMatrix(true);
}
}
}

vector3d Transform::getDirectionMovement()const{
vector3d dir=(position-lastPosition);

//Se for um vetor nulo, retorna o vetor que representa o movimento pra frente rotacionado pela orienta��o.
if(dir.lengthSquared()<0.00001){
return quaternion_vector_rotate(orientation, {0, 1, 0});
}

dir.normalize();
return dir;
}

void Transform::rotate(decimal x, decimal y, decimal z){
quaternion q=quaternion::fromEuler(x, y, z);
this->rotate(q);
}

    void Transform::rotate(const quaternion& rotation){
orientation=rotation*orientation;
orientation.normalize();
setNeedUpdate(true);
}

    void Transform::translate(const vector3d& translation){
lastPosition=position;
position+=translation;
setNeedUpdate(true);
}

}
