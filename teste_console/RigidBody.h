

#ifndef RIGIDBODY_H_H
#define RIGIDBODY_H_H
#include<algorithm>

class RigidBody {
public:
    double mass;
    double invMass;          // Inverso da massa
    double restitution;      // Coeficiente de restituição (elasticidade)
    double linearDamping;    // Amortecimento linear
    double angularDamping;   // Amortecimento angular
    double staticFriction;   // Atrito estático
    double dynamicFriction;  // Atrito dinâmico

    bool isStatic=true;;          // Definir se o corpo é estático

    vector3d position;
    vector3d linearVelocity;
    vector3d angularVelocity;
    vector3d force;
    vector3d torque;
    quaternion orientation;
gpp::GeometricShape* hbody;
    // Métodos
void integrate(double dt) {
    if (invMass == 0.0f) return; // Não atualizar corpos estáticos

    // Atualizar a posição com a velocidade linear
    linearVelocity += (force * invMass) * dt;
    position += linearVelocity * dt;
hbody->translate( linearVelocity * dt);

    // Atualizar a orientação com a velocidade angular
    angularVelocity += (torque * invMass) * dt;
    orientation += quaternion(angularVelocity, dt) * orientation;

    // Aplicar amortecimento
    linearVelocity *= 1.0f - linearDamping * dt;
    angularVelocity *= 1.0f - angularDamping * dt;

    // Limpar forças e torques para a próxima iteração
    force = vector3d(0, 0, 0);
    torque = vector3d(0, 0, 0);
}

void solveCollision(RigidBody* other, gpp::CollisionInfo* info) {
    if (invMass == 0.0f && other->invMass == 0.0f) return; // Sem colisão se ambos forem estáticos

    // Calcular a velocidade relativa
    vector3d relativeVelocity = other->linearVelocity - linearVelocity;

    // Calcular a velocidade na direção da normal da colisão
    double velocityAlongNormal = vector3d::dot(relativeVelocity, info->normal);

    // Se os corpos estão se afastando, não calcular o impulso
    if (velocityAlongNormal > 0) return;

    // Calcular impulso escalar
    double e = std::min(restitution, other->restitution); // Coeficiente de restituição (elasticidade)
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

    // 3. Corrigir a interpenetração dos corpos (Positional Correction)
    const double percent = 0.8f;  // Quanto da penetração será corrigido (80% neste caso)
    const double slop = 0.01f;    // Tolerância para pequenas penetrações
    double correctionDepth = (((info->depth - slop)>0.0f) ? info->depth - slop : 0.0f) / (invMass + other->invMass);
//    double correctionDepth = std::max((info->depth - slop), 0.0f) / (invMass + other->invMass);

    // Vetor de correção para mover os corpos para fora da interpenetração
    vector3d correction = correctionDepth * info->normal * percent;

    // Aplicar a correção de posição proporcional à massa inversa
    position -= correction * invMass;
hbody->translate(-(correction * invMass));
    other->position += correction * other->invMass;
other->hbody->translate(correction * other->invMass);
}

vector3d computeImpulse(RigidBody* r1, RigidBody* r2, const CollisionInfo& info) {
    // Calcular a velocidade relativa
    vector3d relativeVelocity = r2->linearVelocity - r1->linearVelocity;

    // Calcular a velocidade ao longo da normal de colisão
    double velocityAlongNormal = vector3d::dot(relativeVelocity, info.normal);

    // Se a velocidade ao longo da normal for maior que 0, os corpos estão se afastando, não aplicar impulso
    if (velocityAlongNormal > 0) return vector3d(0, 0, 0);

    // Calcular o coeficiente de restituição
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
        return vector3d(0, 0, 0); // Sem atrito se não houver componente tangencial
    }

    // Calcular a magnitude da força de atrito dinâmica
    double frictionMagnitude = -dynamicFriction * vector3d::dot(relativeVel, tangent);

    // Calcular a força de atrito tangencial
    vector3d friction = frictionMagnitude * tangent;

    // Verificar se está dentro do limite de atrito estático
    double maxFriction = staticFriction * vector3d::dot(relativeVel, normal);
    if (friction.lengthSquared() > maxFriction * maxFriction) {
        // Se o atrito dinâmico for maior que o estático, limitar o atrito à força estática máxima
        friction = vector3d::normalize(friction) * maxFriction;
    }

    return friction;
}
};
#endif
