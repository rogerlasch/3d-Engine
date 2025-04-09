

#include"../geometry/geometry.h"
#include"collision.h"
#include"../world/RigidBody.h"

using namespace std;

namespace gpp{

//Protótipos para testes de Obbs.
bool testAxis(const vector3d& axis, const vector3d& delta, const vector3d axesA[3], const vector3d& extentsA,               const vector3d axesB[3], const vector3d& extentsB);
decimal computeProjection(const vector3d& axis, const vector3d axes[3], const vector3d& extents);
decimal computePenetrationDepth(const vector3d& delta, const vector3d axesA[3], const vector3d& extentsA,                                 const vector3d axesB[3], const vector3d& extentsB);

bool checkSphereSphereCollision(Sphere3d* sphereA, Sphere3d* sphereB, CollisionInfo* info) {
    // Obtém os centros das esferas
    vector3d centerA = sphereA->getTransform()->getPosition();
    vector3d centerB = sphereB->getTransform()->getPosition();

    // Calcula a distância entre os centros
    vector3d delta = centerB - centerA;
    decimal distance = delta.length();

if(distance<=0.000001){

if(info!=NULL){
info->normal={0,0,1};
info->point=sphereA->getTransform()->getPosition()+(info->normal*sphereA->getRadius());
info->depth=sphereA->getRadius()+sphereB->getRadius();
}

return true;
}

    // Soma dos raios das esferas
    decimal radiusSum = sphereA->getRadius() + sphereB->getRadius();

    // Verifica se há colisão
    if (distance > radiusSum) {
        return false; // Nenhuma colisão
    }

    // Preenche as informações da colisão
if(info!=NULL){
    info->point = centerA + delta.normalize() * sphereA->getRadius();
    info->normal = delta.normalize();
    info->depth = radiusSum - distance;
}
    return true;
}

bool checkCapsuleCapsuleCollision(Capsule3d* capsuleA, Capsule3d* capsuleB, CollisionInfo* info) {
    if (!capsuleA || !capsuleB || !info) {
        return false; // Verificação de ponteiros nulos
    }

    // Obtém os pontos iniciais e finais das cápsulas
vector3d startA, endA;
vector3d startB, endB;
capsuleA->getSegment(startA, endA);
capsuleB->getSegment(startB, endB);

    // Calcula os pontos mais próximos entre os segmentos das cápsulas
    decimal paramA, paramB;
    vector3d closestPointA, closestPointB;
    decimal distanceSq = ClosestPtSegmentSegment(startA, endA, startB, endB, paramA, paramB, closestPointA, closestPointB);

    // Soma dos raios das cápsulas
    decimal radiusSum = capsuleA->getRadius() + capsuleB->getRadius();
    decimal radiusSumSq = radiusSum * radiusSum;

    // Verifica se há colisão
    if (distanceSq > radiusSumSq) {
        return false; // Nenhuma colisão
    }

    // Preenche as informações da colisão
if(info!=NULL){
    info->normal = (closestPointB - closestPointA).normalize();
    info->point = closestPointA+(info->normal*capsuleA->getRadius());
    info->depth = radiusSum - std::sqrt(distanceSq);
}

    return true;
}

bool checkSphereCapsuleCollision(Sphere3d* sphere, Capsule3d* capsule, CollisionInfo* info) {
    if (!sphere || !capsule || !info) {
        return false; // Verificação de ponteiros nulos
    }

    // Obtém o centro da esfera e os pontos inicial e final da cápsula
    vector3d sphereCenter = sphere->getTransform()->getPosition();
vector3d capsuleStart, capsuleEnd;
capsule->getSegment(capsuleStart, capsuleEnd);

    // Calcula o ponto mais próximo na cápsula ao centro da esfera
    decimal param;
    vector3d closestPointOnCapsule;
    decimal distance = sqrt(ClosestPtPointSegment(sphereCenter, capsuleStart, capsuleEnd, param, closestPointOnCapsule));

    // Soma dos raios da esfera e da cápsula
    decimal radiusSum = sphere->getRadius() + capsule->getRadius();

if(distance<=0.000001){

if(info!=NULL){
info->normal={0,0,1};
info->point=sphere->getTransform()->getPosition()+(info->normal*sphere->getRadius());
info->depth=sphere->getRadius()+capsule->getRadius();
}

return true;
}

    // Verifica se há colisão
    if (distance > radiusSum) {
        return false; // Nenhuma colisão
    }

    // Preenche as informações da colisão
if(info!=NULL){
vector3d normal=vector3d::normalize(closestPointOnCapsule-sphere->getPosition());
    info->point = sphere->getPosition()+ normal * sphere->getRadius();
    info->normal = normal;
    info->depth = radiusSum - distance;
}
    return true;

}

bool checkBoxSphereCollision(Box3d* box, Sphere3d* sphere, CollisionInfo* info) {
    if (!box || !sphere || !info) {
        return false; // Verificação de ponteiros nulos
    }

    // Obtém o centro da esfera no espaço local da caixa
    vector3d sphereCenter = box->getTransform()->toLocal(sphere->getTransform()->getPosition());

    // Obtém as dimensões da caixa (metade das extensões)
    vector3d boxExtents = box->getExtents();

    // Calcula o ponto mais próximo na caixa ao centro da esfera
    vector3d closestPoint;
    closestPoint.x = gpp_clamp(sphereCenter.x, -boxExtents.x, boxExtents.x);
    closestPoint.y = gpp_clamp(sphereCenter.y, -boxExtents.y, boxExtents.y);
    closestPoint.z = gpp_clamp(sphereCenter.z, -boxExtents.z, boxExtents.z);

    // Calcula a distância entre o centro da esfera e o ponto mais próximo na caixa
    vector3d delta = sphereCenter - closestPoint;
    decimal distanceSq = delta.lengthSquared();

    // Verifica se há colisão
    decimal radius = sphere->getRadius();
    decimal radiusSq = radius * radius;

    if (distanceSq > radiusSq) {
        return false; // Nenhuma colisão
    }

if(info!=NULL){
//Tratar o caso de quando o centro da esfera estiver na caixa.
if(distanceSq<=0.00001){

info->normal=vector3d::normalize(box->getTransform()->toLocal(sphere->getTransform()->getDirectionMovement().inverse()));
RayInfo rinfo;
rinfo.origin=sphere->getTransform()->getPosition();
rinfo.dir=info->normal;
box->rayCast(&rinfo);
info->point=rinfo.outPoint;
info->depth=rinfo.outDist+radius;
}
else{
info->normal=(sphereCenter-closestPoint).normalize();
info->point=box->getTransform()->toGlobal(closestPoint);
info->depth=radius-sqrt(distanceSq);
}
}

    return true;
}

bool checkBoxCapsuleCollision(Box3d* box, Capsule3d* capsule, CollisionInfo* info) {
    if (!box || !capsule || !info) {
        return false; // Verificação de ponteiros nulos
    }

    // Obtém os pontos inicial e final da cápsula no espaço local da caixa
vector3d capsuleStart, capsuleEnd;
capsule->getSegment(capsuleStart, capsuleEnd);

capsuleStart = box->getTransform()->toLocal(capsuleStart);
capsuleEnd = box->getTransform()->toLocal(capsuleEnd);

    // Obtém as dimensões da caixa (metade das extensões)
    vector3d boxExtents = box->getExtents();

    // Calcula o ponto mais próximo na cápsula ao centro da caixa
    decimal param;
    vector3d closestPointOnCapsule;
    decimal distanceSq = ClosestPtPointSegment(vector3d(0.0f, 0.0f, 0.0f), capsuleStart, capsuleEnd, param, closestPointOnCapsule);

    // Limita o ponto mais próximo às dimensões da caixa
    vector3d closestPointOnBox;
    closestPointOnBox.x = gpp_clamp(closestPointOnCapsule.x, -boxExtents.x, boxExtents.x);
    closestPointOnBox.y = gpp_clamp(closestPointOnCapsule.y, -boxExtents.y, boxExtents.y);
    closestPointOnBox.z = gpp_clamp(closestPointOnCapsule.z, -boxExtents.z, boxExtents.z);

    // Calcula a distância entre o ponto mais próximo na cápsula e o ponto mais próximo na caixa
    vector3d delta = closestPointOnCapsule - closestPointOnBox;
    distanceSq = delta.lengthSquared();

    // Verifica se há colisão
    decimal radius = capsule->getRadius();
    decimal radiusSq = radius * radius;
    if (distanceSq > radiusSq) {
        return false; // Nenhuma colisão
    }

if(info!=NULL){
//Tratar o caso de quando o centro da esfera estiver na caixa.
if(distanceSq<=0.00001){

info->normal=vector3d::normalize(box->getTransform()->toLocal(capsule->getTransform()->getDirectionMovement().inverse()));
RayInfo rinfo;
rinfo.origin=box->getTransform()->toGlobal(closestPointOnCapsule );
rinfo.dir=info->normal;
box->rayCast(&rinfo);
info->point=rinfo.outPoint;
info->depth=rinfo.outDist+radius;
}
else{
info->normal=delta.normalize();
info->point=box->getTransform()->toGlobal(closestPointOnBox);
info->depth=radius-sqrt(distanceSq);
}
}

    return true;
}

bool checkBoxBoxCollision(Box3d* boxA, Box3d* boxB, CollisionInfo* info) {
    if (!boxA || !boxB || !info) {
        return false; // Verificação de ponteiros nulos
    }

    // Obtém as transformações das caixas
Transform* transformA = boxA->getTransform();
Transform* transformB = boxB->getTransform();

    // Obtém os eixos locais das caixas
    matrix3x3 rotationA = transformA->getOrientation().toMatrix3x3();
    matrix3x3 rotationB = transformB->getOrientation().toMatrix3x3();

    vector3d axesA[3] = {
        rotationA * vector3d(1.0f, 0.0f, 0.0f), // Eixo X da caixa A
        rotationA * vector3d(0.0f, 1.0f, 0.0f), // Eixo Y da caixa A
        rotationA * vector3d(0.0f, 0.0f, 1.0f)  // Eixo Z da caixa A
    };

    vector3d axesB[3] = {
        rotationB * vector3d(1.0f, 0.0f, 0.0f), // Eixo X da caixa B
        rotationB * vector3d(0.0f, 1.0f, 0.0f), // Eixo Y da caixa B
        rotationB * vector3d(0.0f, 0.0f, 1.0f)  // Eixo Z da caixa B
    };

    // Obtém os centros das caixas
    vector3d centerA = transformA->getPosition();
    vector3d centerB = transformB->getPosition();

    // Vetor entre os centros das caixas
    vector3d delta = centerB - centerA;

    // Obtém as extensões das caixas
    vector3d extentsA = boxA->getExtents();
    vector3d extentsB = boxB->getExtents();

    // Verifica os 15 eixos de separação
    for (int i = 0; i < 3; i++) {
        // Eixos da caixa A
        if (!testAxis(axesA[i], delta, axesA, extentsA, axesB, extentsB)) {
            return false; // Eixo de separação encontrado
        }

        // Eixos da caixa B
        if (!testAxis(axesB[i], delta, axesA, extentsA, axesB, extentsB)) {
            return false; // Eixo de separação encontrado
        }
    }

    // Eixos resultantes do produto vetorial entre os eixos das caixas
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vector3d axis = vector3d::cross(axesA[i], axesB[j]);
            if (axis.lengthSquared() < GPP_EPSILON) {
                continue; // Eixo degenerado (paralelo)
            }
            axis.normalize();

            if (!testAxis(axis, delta, axesA, extentsA, axesB, extentsB)) {
                return false; // Eixo de separação encontrado
            }
        }
    }

    // Se nenhum eixo de separação foi encontrado, as caixas estão colidindo
if(info!=NULL){
    info->point = centerA + delta * 0.5f; // Ponto médio entre os centros
    info->normal = delta.normalize(); // Normal apontando de A para B
    info->depth = computePenetrationDepth(delta, axesA, extentsA, axesB, extentsB);
}

    return true;
}

bool testAxis(const vector3d& axis, const vector3d& delta, const vector3d axesA[3], const vector3d& extentsA,               const vector3d axesB[3], const vector3d& extentsB) {
    // Projeta as extensões das caixas no eixo
    decimal projectionA = computeProjection(axis, axesA, extentsA);
    decimal projectionB = computeProjection(axis, axesB, extentsB);

    // Projeta o vetor delta no eixo
    decimal deltaProjection = std::abs(vector3d::dot(delta, axis));

    // Verifica se há sobreposição
    return (deltaProjection <= projectionA + projectionB);
}

decimal computeProjection(const vector3d& axis, const vector3d axes[3], const vector3d& extents) {
    decimal projection = 0.0f;
    for (int i = 0; i < 3; i++) {
        projection += std::abs(vector3d::dot(axis, axes[i])) * extents[i];
    }
    return projection;
}

decimal computePenetrationDepth(const vector3d& delta, const vector3d axesA[3], const vector3d& extentsA,                                 const vector3d axesB[3], const vector3d& extentsB) {
    decimal minDepth = std::numeric_limits<decimal>::max();
    vector3d minAxis;

    // Verifica os eixos das caixas
    for (int i = 0; i < 3; i++) {
        decimal depth = computeProjection(axesA[i], axesA, extentsA) + computeProjection(axesA[i], axesB, extentsB) - std::abs(vector3d::dot(delta, axesA[i]));
        if (depth < minDepth) {
            minDepth = depth;
            minAxis = axesA[i];
        }

        depth = computeProjection(axesB[i], axesA, extentsA) + computeProjection(axesB[i], axesB, extentsB) - std::abs(vector3d::dot(delta, axesB[i]));
        if (depth < minDepth) {
            minDepth = depth;
            minAxis = axesB[i];
        }
    }

    // Verifica os eixos resultantes do produto vetorial
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            vector3d axis = vector3d::cross(axesA[i], axesB[j]);
            if (axis.lengthSquared() < GPP_EPSILON) {
                continue; // Eixo degenerado (paralelo)
            }
            axis.normalize();

            decimal depth = computeProjection(axis, axesA, extentsA) + computeProjection(axis, axesB, extentsB) - std::abs(vector3d::dot(delta, axis));
            if (depth < minDepth) {
                minDepth = depth;
                minAxis = axis;
            }
        }
    }

    return minDepth;
}

bool detectCollisionWith(RigidBody* r1, RigidBody* r2, CollisionInfo* info) {
    GeometricShape* s1 = r1->getShape();
    GeometricShape* s2 = r2->getShape();

if(info!=NULL){
info->r1=r1;
info->r2=r2;
}

bool hasCollision=false, inverseBodies=false;

    // Detecta colisões com base nos tipos das formas geométricas
    switch (s1->getType() | s2->getType()) {
        case GTYPE_SPHERE: // Esfera x Esfera
                hasCollision=checkSphereSphereCollision(
                    static_cast<Sphere3d*>(s1),
                    static_cast<Sphere3d*>(s2),
                    info
                );
            break;

        case GTYPE_BOX: // Caixa x Caixa
                hasCollision=checkBoxBoxCollision(
                    static_cast<Box3d*>(s1),
                    static_cast<Box3d*>(s2),
                    info
                );
            break;

        case GTYPE_CAPSULE: // Cápsula x Cápsula
                hasCollision=checkCapsuleCapsuleCollision(
                    static_cast<Capsule3d*>(s1),
                    static_cast<Capsule3d*>(s2),
                    info
                );
            break;

        case GTYPE_SPHERE | GTYPE_BOX: // Esfera x Caixa

inverseBodies=s1->getType()!=GTYPE_BOX;
if(inverseBodies) std::swap(s1, s2);

                hasCollision=checkBoxSphereCollision(
                    static_cast<Box3d*>(s1),
static_cast<Sphere3d*>(s2),
                    info
                );
            break;

        case GTYPE_SPHERE | GTYPE_CAPSULE: // Esfera x Cápsula

inverseBodies=s1->getType()!=GTYPE_SPHERE;
if(inverseBodies) std::swap(s1, s2);

                hasCollision=checkSphereCapsuleCollision(
                    static_cast<Sphere3d*>(s1),
                    static_cast<Capsule3d*>(s2),
                    info
                );
            break;

        case GTYPE_BOX | GTYPE_CAPSULE: // Caixa x Cápsula
inverseBodies=s1->getType()!=GTYPE_BOX;
if(inverseBodies) std::swap(s1, s2);

                hasCollision=checkBoxCapsuleCollision(
                    static_cast<Box3d*>(s1),
                    static_cast<Capsule3d*>(s2),
                    info
                );
            break;

        default:
            // Caso não haja colisão ou o tipo de colisão não seja suportado
return false;
    }

if(inverseBodies){
if(info!=NULL){
std::swap(info->r1, info->r2);
}
}

return hasCollision;
}
}
