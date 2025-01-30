

#ifndef RIGIDBODY_H_H
#define RIGIDBODY_H_H
#include<algorithm>

class RigidBody {
public:
    double mass;
    double invMass;          // Inverso da massa
    double restitution;      // Coeficiente de restitui��o (elasticidade)
    double linearDamping;    // Amortecimento linear
    double angularDamping;   // Amortecimento angular
    double staticFriction;   // Atrito est�tico
    double dynamicFriction;  // Atrito din�mico

    bool isStatic=true;;          // Definir se o corpo � est�tico

    vector3d position;
    vector3d linearVelocity;
    vector3d angularVelocity;
    vector3d force;
    vector3d torque;
    quaternion orientation;
gpp::GeometricShape* hbody;
    // M�todos
void integrate(double dt) {
    if (invMass == 0.0f) return; // N�o atualizar corpos est�ticos

    // Atualizar a posi��o com a velocidade linear
    linearVelocity += (force * invMass) * dt;
    position += linearVelocity * dt;
hbody->translate( linearVelocity * dt);

    // Atualizar a orienta��o com a velocidade angular
    angularVelocity += (torque * invMass) * dt;
    orientation += quaternion(angularVelocity, dt) * orientation;

    // Aplicar amortecimento
    linearVelocity *= 1.0f - linearDamping * dt;
    angularVelocity *= 1.0f - angularDamping * dt;

    // Limpar for�as e torques para a pr�xima itera��o
    force = vector3d(0, 0, 0);
    torque = vector3d(0, 0, 0);
}

void solveCollision(RigidBody* other, gpp::CollisionInfo* info) {
    if (invMass == 0.0f && other->invMass == 0.0f) return; // Sem colis�o se ambos forem est�ticos

    // Calcular a velocidade relativa
    vector3d relativeVelocity = other->linearVelocity - linearVelocity;

    // Calcular a velocidade na dire��o da normal da colis�o
    double velocityAlongNormal = vector3d::dot(relativeVelocity, info->normal);

    // Se os corpos est�o se afastando, n�o calcular o impulso
    if (velocityAlongNormal > 0) return;

    // Calcular impulso escalar
    double e = std::min(restitution, other->restitution); // Coeficiente de restitui��o (elasticidade)
    double j = -(1 + e) * velocityAlongNormal;
    j /= invMass + other->invMass;

    // Impulso final
    vector3d impulse = j * info->normal;

    // Aplicar impulso aos dois corpos
    linearVelocity -= impulse * invMass;
    other->linearVelocity += impulse * other->invMass;

    // Atrito
    vector3d friction = computeFriction(info->normal, relativeVelocity, staticFriction, dynamicFriction);
    linearVelocity -= friction * invMass;
    other->linearVelocity += friction * other->invMass;

    // 3. Corrigir a interpenetra��o dos corpos (Positional Correction)
    const double percent = 0.8f;  // Quanto da penetra��o ser� corrigido (80% neste caso)
    const double slop = 0.01f;    // Toler�ncia para pequenas penetra��es
    double correctionDepth = (((info->depth - slop)>0.0f) ? info->depth - slop : 0.0f) / (invMass + other->invMass);
//    double correctionDepth = std::max((info->depth - slop), 0.0f) / (invMass + other->invMass);

    // Vetor de corre��o para mover os corpos para fora da interpenetra��o
    vector3d correction = correctionDepth * info->normal * percent;

    // Aplicar a corre��o de posi��o proporcional � massa inversa
    position -= correction * invMass;
hbody->translate(-(correction * invMass));
    other->position += correction * other->invMass;
other->hbody->translate(correction * other->invMass);
}

vector3d computeImpulse(RigidBody* r1, RigidBody* r2, const CollisionInfo& info) {
    // Calcular a velocidade relativa
    vector3d relativeVelocity = r2->linearVelocity - r1->linearVelocity;

    // Calcular a velocidade ao longo da normal de colis�o
    double velocityAlongNormal = vector3d::dot(relativeVelocity, info.normal);

    // Se a velocidade ao longo da normal for maior que 0, os corpos est�o se afastando, n�o aplicar impulso
    if (velocityAlongNormal > 0) return vector3d(0, 0, 0);

    // Calcular o coeficiente de restitui��o
    double e = std::min(r1->restitution, r2->restitution);

    // Calcular o impulso escalar
    double j = -(1 + e) * velocityAlongNormal;
    j /= r1->invMass + r2->invMass;

    // Retornar o vetor de impulso
    return j * info.normal;
}

vector3d computeFriction(const vector3d& normal, const vector3d& relativeVel, double staticFriction, double dynamicFriction) {
    // Calcular a componente tangencial da velocidade relativa
    vector3d tangent = relativeVel - (vector3d::dot(relativeVel, normal) ^ normal);

    // Normalizar o vetor tangente
    if (tangent.lengthSquared() > 0.0f) {
        tangent = vector3d::normalize(tangent);
    } else {
        return vector3d(0, 0, 0); // Sem atrito se n�o houver componente tangencial
    }

    // Calcular a magnitude da for�a de atrito din�mica
    double frictionMagnitude = -dynamicFriction * vector3d::dot(relativeVel, tangent);

    // Calcular a for�a de atrito tangencial
    vector3d friction = frictionMagnitude * tangent;

    // Verificar se est� dentro do limite de atrito est�tico
    double maxFriction = staticFriction * vector3d::dot(relativeVel, normal);
    if (friction.lengthSquared() > maxFriction * maxFriction) {
        // Se o atrito din�mico for maior que o est�tico, limitar o atrito � for�a est�tica m�xima
        friction = vector3d::normalize(friction) * maxFriction;
    }

    return friction;
}
};
#endif
