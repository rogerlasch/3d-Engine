

#include"common.h"
#include"math/math.h"

namespace gpp{

//Calcula o arrasto do ar...
//Fd = 1/2 * p * v^2 * A * Cd
//onde: p = é a densidade do ar
//A é área projetada da partícula
//Cd é coeficiente de resitência da esfera
vector3d fCalculateAirDrag(const vector3d& velocity, float projectedArea, float airDencity, float airDragCoefficient) {
vector3d vdir;
vdir -= velocity;
vdir.normalize();
//float fd=0.5f*airDencity*vel*vel*projectedArea*airDragCoefficient;
float fd=0.5f*airDragCoefficient*airDencity*vector3d::dotProduct(velocity, velocity)*projectedArea;
return vdir*fd;
}

vector3d fCalculateTangent(const vector3d& collisionNormal) {
    // Realize uma rotação de 90 graus no plano da colisão.
    // Uma forma simples de fazer isso é trocar as coordenadas x e z e inverter a coordenada z.
    vector3d collisionTangent;
    collisionTangent.x = collisionNormal.z;
    collisionTangent.y = collisionNormal.y;
    collisionTangent.z = -collisionNormal.x;
    // Certifique-se de normalizar o vetor resultante
    collisionTangent.normalize();
    return collisionTangent;
}

// Função para calcular a velocidade tangencial
vector3d fCalculateTangentialVelocity(const vector3d& velocity, const vector3d& collisionTangent) {
    // Projete a velocidade atual no vetor tangencial usando a operação de produto escalar (dot product)
    float tangentialSpeed = vector3d::dotProduct(velocity, collisionTangent);
    // Agora, construa o vetor de velocidade tangencial multiplicando a velocidade escalar pela direção tangencial
    vector3d tangentialVelocity = collisionTangent * tangentialSpeed;
    return tangentialVelocity;
}

vector3d fCalculateSpringForce(const vector3d& p1, const vector3d& v1, const vector3d& p2, const vector3d& v2, float restLength, float stiffness, float damping){
vector3d final;
vector3d dist=p1-p2;
float d=dist.length();
float deformAmount=d-restLength;
float restorativeForce=stiffness*deformAmount;
final=((dist/d)*restorativeForce);
final+=(v1-v2)*damping;
return final;
}
}
