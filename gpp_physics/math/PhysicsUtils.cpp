
#include "vector3d.h"
#include "quaternion.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

namespace gpp {

// Funções de física básicas

/**
 * @brief Calcula a força gravitacional entre dois corpos.
 *
 * Usa a fórmula F = G * (m1 * m2) / d^2 para calcular a força gravitacional entre dois corpos.
 *
 * @param position1 Posição do primeiro corpo (em metros).
 * @param position2 Posição do segundo corpo (em metros).
 * @param mass1 Massa do primeiro corpo (em kg). Deve ser um valor positivo.
 * @param mass2 Massa do segundo corpo (em kg). Deve ser um valor positivo.
 * @param gravitationalConstant Constante gravitacional, valor padrão é 9.81 m/s².
 *
 * @return A força gravitacional entre os dois corpos como um vetor.
 */

vector3d calculateGravitationalForce(const vector3d& position1, const vector3d& position2, decimal mass1, decimal mass2, decimal gravitationalConstant = 9.81f) {
    vector3d distance = position2 - position1;
    decimal dSquared = vector3d::dot(distance, distance);
    decimal forceMagnitude = gravitationalConstant * ((mass1 * mass2) / dSquared);
    return distance.normalize() * forceMagnitude;
}

/**
 * @brief Calcula a força de atrito.
 *
 * Usa a fórmula F_atrito = coeficiente * F_normal para calcular a força de atrito.
 *
 * @param normalForce Força normal aplicada ao objeto (em Newtons). Deve ser positiva.
 * @param coefficientOfFriction Coeficiente de atrito, variando entre 0 e 1. 0 significa superfície perfeitamente lisa, 1 significa atrito máximo.
 *
 * @return A força de atrito aplicada ao objeto.
 */
decimal calculateFrictionForce(decimal normalForce, decimal coefficientOfFriction) {
    return normalForce * coefficientOfFriction;
}

/**
 * @brief Calcula a força resultante em um corpo.
 *
 * Soma duas forças vetoriais aplicadas a um corpo.
 *
 * @param force1 Primeira força aplicada ao corpo.
 * @param force2 Segunda força aplicada ao corpo.
 *
 * @return A força resultante no corpo como um vetor.
 */
vector3d calculateNetForce(const vector3d& force1, const vector3d& force2) {
    return force1 + force2;
}

/**
 * @brief Calcula o impulso (momentum) de um corpo.
 *
 * Usa a fórmula P = m * v para calcular o impulso de um corpo, onde P é o impulso, m é a massa, e v é a velocidade.
 *
 * @param velocity Velocidade do corpo (em metros por segundo).
 * @param mass Massa do corpo (em kg). Deve ser um valor positivo.
 *
 * @return O impulso do corpo como um vetor.
 */
vector3d calculateImpulse(const vector3d& velocity, decimal mass) {
    return velocity * mass;
}

/**
 * @brief Verifica se dois corpos colidiram e retorna o vetor normal da colisão.
 *
 * Usa a distância entre os centros dos corpos e seus raios para determinar se há uma colisão.
 *
 * @param position1 Posição do primeiro corpo (em metros).
 * @param radius1 Raio do primeiro corpo (em metros). Deve ser um valor positivo.
 * @param position2 Posição do segundo corpo (em metros).
 * @param radius2 Raio do segundo corpo (em metros). Deve ser um valor positivo.
 * @param collisionNormal Referência onde o vetor normal da superfície de colisão será armazenado.
 *                        Esse vetor estará normalizado, apontando do primeiro corpo para o segundo.
 *                        Se não houver colisão, o vetor retornado será o vetor nulo.
 *
 * @return Verdadeiro se os corpos colidiram, falso caso contrário.
 */
bool checkCollision(const vector3d& position1, decimal radius1, const vector3d& position2, decimal radius2, vector3d& collisionNormal) {
    vector3d displacement = position2 - position1;
    decimal distanceSquared = displacement.magnitudeSquared();
    decimal radiusSum = radius1 + radius2;

    if (distanceSquared <= (radiusSum * radiusSum)) {
        collisionNormal = displacement.normalized(); // Normal da colisão, apontando de position1 para position2
        return true;
    } else {
        collisionNormal = vector3d(0, 0, 0); // Nenhuma colisão, normal é o vetor nulo
        return false;
    }
}

/**
 * @brief Resolve a colisão entre dois corpos ajustando suas velocidades.
 *
 * Utiliza a fórmula de conservação de momento para ajustar as velocidades dos corpos após uma colisão.
 *
 * @param velocity1 Velocidade do primeiro corpo (em metros por segundo).
 * @param velocity2 Velocidade do segundo corpo (em metros por segundo).
 * @param mass1 Massa do primeiro corpo (em kg). Deve ser positiva.
 * @param mass2 Massa do segundo corpo (em kg). Deve ser positiva.
 * @param normal Vetor normal à superfície de colisão.
 */
void resolveCollision(vector3d& velocity1, vector3d& velocity2, decimal mass1, decimal mass2, const vector3d& normal) {
    vector3d relativeVelocity = velocity2 - velocity1;
    decimal velocityAlongNormal = relativeVelocity.dot(normal);
    if (velocityAlongNormal > 0) return; // Não resolve se já estão se afastando

    decimal restitution = 0.5f; // Coeficiente de restituição (pode ser ajustado)
    decimal impulse = -(1 + restitution) * velocityAlongNormal / (1/mass1 + 1/mass2);

    vector3d impulseVector = normal * impulse;
    velocity1 -= impulseVector / mass1;
    velocity2 += impulseVector / mass2;
}

/**
 * @brief Calcula a energia cinética de um corpo.
 *
 * Usa a fórmula E_k = 1/2 * m * v^2 para calcular a energia cinética.
 *
 * @param mass Massa do corpo (em kg). Deve ser positiva.
 * @param velocity Velocidade do corpo (em metros por segundo).
 *
 * @return A energia cinética do corpo (em joules).
 */
decimal calculateKineticEnergy(decimal mass, const vector3d& velocity) {
    return 0.5f * mass * vector3d::dot(velocity, velocity);
}

/**
 * @brief Calcula o momento de inércia de uma esfera.
 *
 * Usa a fórmula I = 2/5 * m * r^2 para calcular o momento de inércia de uma esfera.
 *
 * @param mass Massa da esfera (em kg). Deve ser positiva.
 * @param radius Raio da esfera (em metros). Deve ser positivo.
 *
 * @return O momento de inércia da esfera.
 */
decimal calculateMomentOfInertiaSphere(decimal mass, decimal radius) {
    return (2.0f / 5.0f) * mass * (radius * radius);
}

/**
 * @brief Verifica a estabilidade de um corpo com base em sua posição vertical e orientação.
 *
 * Essa função pode ser usada para verificar se um corpo está "em pé" ou equilibrado. 
 * A estabilidade depende tanto da posição do corpo (se está acima de um certo limite, como o solo)
 * quanto da orientação do corpo (se sua rotação está dentro de um limiar aceitável).
 *
 * @param position A posição do corpo no espaço (em metros). A altura (componente y) deve ser positiva.
 * @param orientation A orientação do corpo representada como um quatérnion.
 * @param stabilityThreshold O limiar de estabilidade da orientação (entre 0 e 1). Quanto menor o valor,
 *        mais estável é o corpo. Exemplo: 0.1 indica que a rotação está próxima da posição de equilíbrio.
 * 
 * @return Verdadeiro se o corpo for estável (orientação dentro do limiar e acima do solo), falso caso contrário.
 */
bool checkStability(const vector3d& position, const quaternion& orientation, decimal stabilityThreshold) {
    // Critério 1: O corpo deve estar acima do solo (posição.z >= 0).
    if (position.z < 0) {
        return false;
    }

    // Critério 2: O comprimento do quatérnion deve estar abaixo do limiar de estabilidade.
    // Isso verifica se a rotação do corpo está "próxima" da orientação considerada estável.
    if (orientation.length() > stabilityThreshold) {
        return false;
    }

    // Se ambos os critérios forem atendidos, o corpo é considerado estável.
    return true;
}

// Funções utilitárias de física

/**
 * @brief Calcula o ângulo necessário para alinhar dois vetores.
 *
 * Usa o produto escalar para calcular o ângulo entre dois vetores.
 *
 * @param vector1 O primeiro vetor.
 * @param vector2 O segundo vetor.
 *
 * @return O ângulo em radianos entre os dois vetores.
 */
decimal calculateAngleBetweenVectors(const vector3d& vector1, const vector3d& vector2) {
    decimal dotProduct = vector3d::dot(vector1, vector2);
    return std::acos(dotProduct / (vector1.length() * vector2.length()));
}

/**
 * @brief Calcula a aceleração linear de um corpo com base na força aplicada.
 *
 * A aceleração é calculada pela fórmula: F = ma, onde:
 * F é a força aplicada, m é a massa do corpo e a é a aceleração.
 *
 * @param force A força aplicada ao corpo (em Newtons).
 * @param mass A massa do corpo (em kg). Deve ser um valor positivo.
 * @return A aceleração do corpo (em metros por segundo ao quadrado).
 */
vector3d calculateLinearAcceleration(const vector3d& force, decimal mass) {
    if (mass <= 0) {
        throw std::invalid_argument("Mass must be positive");
    }
    return force / mass;
}

/**
 * @brief Atualiza a velocidade linear de um corpo com base na aceleração.
 *
 * Usa a equação: v = v0 + a * ?t, onde:
 * v0 é a velocidade inicial, a é a aceleração, e ?t é o intervalo de tempo.
 *
 * @param velocity A velocidade atual do corpo (em metros por segundo).
 * @param acceleration A aceleração do corpo (em metros por segundo ao quadrado).
 * @param deltaTime O intervalo de tempo (em segundos).
 * @return A nova velocidade do corpo.
 */
vector3d updateLinearVelocity(const vector3d& velocity, const vector3d& acceleration, decimal deltaTime) {
    return velocity + (acceleration * deltaTime);
}

/**
 * @brief Atualiza a posição de um corpo com base na velocidade linear.
 *
 * Usa a equação: p = p0 + v * ?t, onde:
 * p0 é a posição inicial, v é a velocidade, e ?t é o intervalo de tempo.
 *
 * @param position A posição atual do corpo (em metros).
 * @param velocity A velocidade do corpo (em metros por segundo).
 * @param deltaTime O intervalo de tempo (em segundos).
 * @return A nova posição do corpo.
 */
vector3d integratePosition(const vector3d& position, const vector3d& velocity, decimal deltaTime) {
    return position + (velocity * deltaTime);
}

/**
 * @brief Calcula a aceleração angular de um corpo com base no torque e no momento de inércia.
 *
 * A aceleração angular é calculada pela fórmula: t = Ia, onde:
 * t é o torque, I é o momento de inércia, e a é a aceleração angular.
 *
 * @param torque O torque aplicado ao corpo (em Newton-metros).
 * @param inertia O momento de inércia do corpo (em kg·m²). Deve ser um valor positivo.
 * @return A aceleração angular do corpo (em radianos por segundo ao quadrado).
 */
vector3d calculateAngularAcceleration(const vector3d& torque, decimal inertia) {
    if (inertia <= 0) {
        throw std::invalid_argument("Inertia must be positive");
    }
    return torque / inertia;
}

/**
 * @brief Atualiza a velocidade angular de um corpo com base na aceleração angular.
 *
 * Usa a equação: ? = ?0 + a * ?t, onde:
 * ?0 é a velocidade angular inicial, a é a aceleração angular, e ?t é o intervalo de tempo.
 *
 * @param angularVelocity A velocidade angular atual do corpo (em radianos por segundo).
 * @param angularAcceleration A aceleração angular do corpo (em radianos por segundo ao quadrado).
 * @param deltaTime O intervalo de tempo (em segundos).
 * @return A nova velocidade angular do corpo.
 */
vector3d updateAngularVelocity(const vector3d& angularVelocity, const vector3d& angularAcceleration, decimal deltaTime) {
    return angularVelocity + (angularAcceleration * deltaTime);
}

/**
 * @brief Atualiza a rotação de um corpo com base na velocidade angular.
 *
 * Usa a equação: ? = ?0 + ? * ?t, onde:
 * ?0 é a rotação inicial, ? é a velocidade angular, e ?t é o intervalo de tempo.
 *
 * @param rotation A rotação atual do corpo como um quatérnion.
 * @param angularVelocity A velocidade angular do corpo (em radianos por segundo).
 * @param deltaTime O intervalo de tempo (em segundos).
 * @return A nova rotação do corpo.
 */
quaternion integrateRotation(const quaternion& rotation, const vector3d& angularVelocity, decimal deltaTime) {
    // Converte a velocidade angular em um quatérnion para integrar a rotação.
    quaternion deltaRotation = quaternion::fromAngularVelocity(angularVelocity * deltaTime);
    return (deltaRotation * rotation).normalized();
}

/**
 * @brief Atualiza o tensor de inércia de um corpo com base em sua orientação.
 *
 * Quando um corpo gira, seu tensor de inércia pode precisar ser recalculado para a nova orientação.
 * Essa função aplica a rotação atual do corpo ao tensor de inércia local.
 *
 * @param inertiaTensor O tensor de inércia local (em kg·m²).
 * @param rotation A rotação atual do corpo como um quatérnion.
 * @return O tensor de inércia atualizado no espaço global.
 */
matrix3x3 updateInertiaTensor(const matrix3x3& inertiaTensor, const quaternion& rotation) {
    // Aplica a rotação ao tensor de inércia.
    matrix3x3 rotationMatrix = rotation.toMatrix();
    return rotationMatrix * inertiaTensor * rotationMatrix.transpose();
}

/**
 * @brief Aplica um torque a um corpo e atualiza sua rotação e velocidade angular.
 *
 * Usa o torque aplicado para calcular a aceleração angular e atualiza a rotação do corpo.
 *
 * @param angularVelocity A velocidade angular atual do corpo (em radianos por segundo).
 * @param torque O torque aplicado ao corpo (em Newton-metros).
 * @param inertia O momento de inércia do corpo (em kg·m²).
 * @param rotation A rotação atual do corpo como um quatérnion.
 * @param deltaTime O intervalo de tempo (em segundos).
 * @return A nova rotação do corpo.
 */
quaternion applyTorqueAndUpdateRotation(vector3d& angularVelocity, const vector3d& torque, decimal inertia, quaternion& rotation, decimal deltaTime) {
    // Calcula a aceleração angular com base no torque.
    vector3d angularAcceleration = calculateAngularAcceleration(torque, inertia);
    // Atualiza a velocidade angular.
    angularVelocity = updateAngularVelocity(angularVelocity, angularAcceleration, deltaTime);
    // Atualiza a rotação do corpo.
    rotation = integrateRotation(rotation, angularVelocity, deltaTime);
    return rotation;
}
} // namespace gpp
