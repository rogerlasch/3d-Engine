
#include "vector3d.h"
#include "quaternion.h"
#include "matrix3x3.h"
#include "matrix4x4.h"

namespace gpp {

// Fun��es de f�sica b�sicas

/**
 * @brief Calcula a for�a gravitacional entre dois corpos.
 *
 * Usa a f�rmula F = G * (m1 * m2) / d^2 para calcular a for�a gravitacional entre dois corpos.
 *
 * @param position1 Posi��o do primeiro corpo (em metros).
 * @param position2 Posi��o do segundo corpo (em metros).
 * @param mass1 Massa do primeiro corpo (em kg). Deve ser um valor positivo.
 * @param mass2 Massa do segundo corpo (em kg). Deve ser um valor positivo.
 * @param gravitationalConstant Constante gravitacional, valor padr�o � 9.81 m/s�.
 *
 * @return A for�a gravitacional entre os dois corpos como um vetor.
 */

vector3d calculateGravitationalForce(const vector3d& position1, const vector3d& position2, decimal mass1, decimal mass2, decimal gravitationalConstant = 9.81f) {
    vector3d distance = position2 - position1;
    decimal dSquared = vector3d::dot(distance, distance);
    decimal forceMagnitude = gravitationalConstant * ((mass1 * mass2) / dSquared);
    return distance.normalize() * forceMagnitude;
}

/**
 * @brief Calcula a for�a de atrito.
 *
 * Usa a f�rmula F_atrito = coeficiente * F_normal para calcular a for�a de atrito.
 *
 * @param normalForce For�a normal aplicada ao objeto (em Newtons). Deve ser positiva.
 * @param coefficientOfFriction Coeficiente de atrito, variando entre 0 e 1. 0 significa superf�cie perfeitamente lisa, 1 significa atrito m�ximo.
 *
 * @return A for�a de atrito aplicada ao objeto.
 */
decimal calculateFrictionForce(decimal normalForce, decimal coefficientOfFriction) {
    return normalForce * coefficientOfFriction;
}

/**
 * @brief Calcula a for�a resultante em um corpo.
 *
 * Soma duas for�as vetoriais aplicadas a um corpo.
 *
 * @param force1 Primeira for�a aplicada ao corpo.
 * @param force2 Segunda for�a aplicada ao corpo.
 *
 * @return A for�a resultante no corpo como um vetor.
 */
vector3d calculateNetForce(const vector3d& force1, const vector3d& force2) {
    return force1 + force2;
}

/**
 * @brief Calcula o impulso (momentum) de um corpo.
 *
 * Usa a f�rmula P = m * v para calcular o impulso de um corpo, onde P � o impulso, m � a massa, e v � a velocidade.
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
 * @brief Verifica se dois corpos colidiram e retorna o vetor normal da colis�o.
 *
 * Usa a dist�ncia entre os centros dos corpos e seus raios para determinar se h� uma colis�o.
 *
 * @param position1 Posi��o do primeiro corpo (em metros).
 * @param radius1 Raio do primeiro corpo (em metros). Deve ser um valor positivo.
 * @param position2 Posi��o do segundo corpo (em metros).
 * @param radius2 Raio do segundo corpo (em metros). Deve ser um valor positivo.
 * @param collisionNormal Refer�ncia onde o vetor normal da superf�cie de colis�o ser� armazenado.
 *                        Esse vetor estar� normalizado, apontando do primeiro corpo para o segundo.
 *                        Se n�o houver colis�o, o vetor retornado ser� o vetor nulo.
 *
 * @return Verdadeiro se os corpos colidiram, falso caso contr�rio.
 */
bool checkCollision(const vector3d& position1, decimal radius1, const vector3d& position2, decimal radius2, vector3d& collisionNormal) {
    vector3d displacement = position2 - position1;
    decimal distanceSquared = displacement.magnitudeSquared();
    decimal radiusSum = radius1 + radius2;

    if (distanceSquared <= (radiusSum * radiusSum)) {
        collisionNormal = displacement.normalized(); // Normal da colis�o, apontando de position1 para position2
        return true;
    } else {
        collisionNormal = vector3d(0, 0, 0); // Nenhuma colis�o, normal � o vetor nulo
        return false;
    }
}

/**
 * @brief Resolve a colis�o entre dois corpos ajustando suas velocidades.
 *
 * Utiliza a f�rmula de conserva��o de momento para ajustar as velocidades dos corpos ap�s uma colis�o.
 *
 * @param velocity1 Velocidade do primeiro corpo (em metros por segundo).
 * @param velocity2 Velocidade do segundo corpo (em metros por segundo).
 * @param mass1 Massa do primeiro corpo (em kg). Deve ser positiva.
 * @param mass2 Massa do segundo corpo (em kg). Deve ser positiva.
 * @param normal Vetor normal � superf�cie de colis�o.
 */
void resolveCollision(vector3d& velocity1, vector3d& velocity2, decimal mass1, decimal mass2, const vector3d& normal) {
    vector3d relativeVelocity = velocity2 - velocity1;
    decimal velocityAlongNormal = relativeVelocity.dot(normal);
    if (velocityAlongNormal > 0) return; // N�o resolve se j� est�o se afastando

    decimal restitution = 0.5f; // Coeficiente de restitui��o (pode ser ajustado)
    decimal impulse = -(1 + restitution) * velocityAlongNormal / (1/mass1 + 1/mass2);

    vector3d impulseVector = normal * impulse;
    velocity1 -= impulseVector / mass1;
    velocity2 += impulseVector / mass2;
}

/**
 * @brief Calcula a energia cin�tica de um corpo.
 *
 * Usa a f�rmula E_k = 1/2 * m * v^2 para calcular a energia cin�tica.
 *
 * @param mass Massa do corpo (em kg). Deve ser positiva.
 * @param velocity Velocidade do corpo (em metros por segundo).
 *
 * @return A energia cin�tica do corpo (em joules).
 */
decimal calculateKineticEnergy(decimal mass, const vector3d& velocity) {
    return 0.5f * mass * vector3d::dot(velocity, velocity);
}

/**
 * @brief Calcula o momento de in�rcia de uma esfera.
 *
 * Usa a f�rmula I = 2/5 * m * r^2 para calcular o momento de in�rcia de uma esfera.
 *
 * @param mass Massa da esfera (em kg). Deve ser positiva.
 * @param radius Raio da esfera (em metros). Deve ser positivo.
 *
 * @return O momento de in�rcia da esfera.
 */
decimal calculateMomentOfInertiaSphere(decimal mass, decimal radius) {
    return (2.0f / 5.0f) * mass * (radius * radius);
}

/**
 * @brief Verifica a estabilidade de um corpo com base em sua posi��o vertical e orienta��o.
 *
 * Essa fun��o pode ser usada para verificar se um corpo est� "em p�" ou equilibrado. 
 * A estabilidade depende tanto da posi��o do corpo (se est� acima de um certo limite, como o solo)
 * quanto da orienta��o do corpo (se sua rota��o est� dentro de um limiar aceit�vel).
 *
 * @param position A posi��o do corpo no espa�o (em metros). A altura (componente y) deve ser positiva.
 * @param orientation A orienta��o do corpo representada como um quat�rnion.
 * @param stabilityThreshold O limiar de estabilidade da orienta��o (entre 0 e 1). Quanto menor o valor,
 *        mais est�vel � o corpo. Exemplo: 0.1 indica que a rota��o est� pr�xima da posi��o de equil�brio.
 * 
 * @return Verdadeiro se o corpo for est�vel (orienta��o dentro do limiar e acima do solo), falso caso contr�rio.
 */
bool checkStability(const vector3d& position, const quaternion& orientation, decimal stabilityThreshold) {
    // Crit�rio 1: O corpo deve estar acima do solo (posi��o.z >= 0).
    if (position.z < 0) {
        return false;
    }

    // Crit�rio 2: O comprimento do quat�rnion deve estar abaixo do limiar de estabilidade.
    // Isso verifica se a rota��o do corpo est� "pr�xima" da orienta��o considerada est�vel.
    if (orientation.length() > stabilityThreshold) {
        return false;
    }

    // Se ambos os crit�rios forem atendidos, o corpo � considerado est�vel.
    return true;
}

// Fun��es utilit�rias de f�sica

/**
 * @brief Calcula o �ngulo necess�rio para alinhar dois vetores.
 *
 * Usa o produto escalar para calcular o �ngulo entre dois vetores.
 *
 * @param vector1 O primeiro vetor.
 * @param vector2 O segundo vetor.
 *
 * @return O �ngulo em radianos entre os dois vetores.
 */
decimal calculateAngleBetweenVectors(const vector3d& vector1, const vector3d& vector2) {
    decimal dotProduct = vector3d::dot(vector1, vector2);
    return std::acos(dotProduct / (vector1.length() * vector2.length()));
}

/**
 * @brief Calcula a acelera��o linear de um corpo com base na for�a aplicada.
 *
 * A acelera��o � calculada pela f�rmula: F = ma, onde:
 * F � a for�a aplicada, m � a massa do corpo e a � a acelera��o.
 *
 * @param force A for�a aplicada ao corpo (em Newtons).
 * @param mass A massa do corpo (em kg). Deve ser um valor positivo.
 * @return A acelera��o do corpo (em metros por segundo ao quadrado).
 */
vector3d calculateLinearAcceleration(const vector3d& force, decimal mass) {
    if (mass <= 0) {
        throw std::invalid_argument("Mass must be positive");
    }
    return force / mass;
}

/**
 * @brief Atualiza a velocidade linear de um corpo com base na acelera��o.
 *
 * Usa a equa��o: v = v0 + a * ?t, onde:
 * v0 � a velocidade inicial, a � a acelera��o, e ?t � o intervalo de tempo.
 *
 * @param velocity A velocidade atual do corpo (em metros por segundo).
 * @param acceleration A acelera��o do corpo (em metros por segundo ao quadrado).
 * @param deltaTime O intervalo de tempo (em segundos).
 * @return A nova velocidade do corpo.
 */
vector3d updateLinearVelocity(const vector3d& velocity, const vector3d& acceleration, decimal deltaTime) {
    return velocity + (acceleration * deltaTime);
}

/**
 * @brief Atualiza a posi��o de um corpo com base na velocidade linear.
 *
 * Usa a equa��o: p = p0 + v * ?t, onde:
 * p0 � a posi��o inicial, v � a velocidade, e ?t � o intervalo de tempo.
 *
 * @param position A posi��o atual do corpo (em metros).
 * @param velocity A velocidade do corpo (em metros por segundo).
 * @param deltaTime O intervalo de tempo (em segundos).
 * @return A nova posi��o do corpo.
 */
vector3d integratePosition(const vector3d& position, const vector3d& velocity, decimal deltaTime) {
    return position + (velocity * deltaTime);
}

/**
 * @brief Calcula a acelera��o angular de um corpo com base no torque e no momento de in�rcia.
 *
 * A acelera��o angular � calculada pela f�rmula: t = Ia, onde:
 * t � o torque, I � o momento de in�rcia, e a � a acelera��o angular.
 *
 * @param torque O torque aplicado ao corpo (em Newton-metros).
 * @param inertia O momento de in�rcia do corpo (em kg�m�). Deve ser um valor positivo.
 * @return A acelera��o angular do corpo (em radianos por segundo ao quadrado).
 */
vector3d calculateAngularAcceleration(const vector3d& torque, decimal inertia) {
    if (inertia <= 0) {
        throw std::invalid_argument("Inertia must be positive");
    }
    return torque / inertia;
}

/**
 * @brief Atualiza a velocidade angular de um corpo com base na acelera��o angular.
 *
 * Usa a equa��o: ? = ?0 + a * ?t, onde:
 * ?0 � a velocidade angular inicial, a � a acelera��o angular, e ?t � o intervalo de tempo.
 *
 * @param angularVelocity A velocidade angular atual do corpo (em radianos por segundo).
 * @param angularAcceleration A acelera��o angular do corpo (em radianos por segundo ao quadrado).
 * @param deltaTime O intervalo de tempo (em segundos).
 * @return A nova velocidade angular do corpo.
 */
vector3d updateAngularVelocity(const vector3d& angularVelocity, const vector3d& angularAcceleration, decimal deltaTime) {
    return angularVelocity + (angularAcceleration * deltaTime);
}

/**
 * @brief Atualiza a rota��o de um corpo com base na velocidade angular.
 *
 * Usa a equa��o: ? = ?0 + ? * ?t, onde:
 * ?0 � a rota��o inicial, ? � a velocidade angular, e ?t � o intervalo de tempo.
 *
 * @param rotation A rota��o atual do corpo como um quat�rnion.
 * @param angularVelocity A velocidade angular do corpo (em radianos por segundo).
 * @param deltaTime O intervalo de tempo (em segundos).
 * @return A nova rota��o do corpo.
 */
quaternion integrateRotation(const quaternion& rotation, const vector3d& angularVelocity, decimal deltaTime) {
    // Converte a velocidade angular em um quat�rnion para integrar a rota��o.
    quaternion deltaRotation = quaternion::fromAngularVelocity(angularVelocity * deltaTime);
    return (deltaRotation * rotation).normalized();
}

/**
 * @brief Atualiza o tensor de in�rcia de um corpo com base em sua orienta��o.
 *
 * Quando um corpo gira, seu tensor de in�rcia pode precisar ser recalculado para a nova orienta��o.
 * Essa fun��o aplica a rota��o atual do corpo ao tensor de in�rcia local.
 *
 * @param inertiaTensor O tensor de in�rcia local (em kg�m�).
 * @param rotation A rota��o atual do corpo como um quat�rnion.
 * @return O tensor de in�rcia atualizado no espa�o global.
 */
matrix3x3 updateInertiaTensor(const matrix3x3& inertiaTensor, const quaternion& rotation) {
    // Aplica a rota��o ao tensor de in�rcia.
    matrix3x3 rotationMatrix = rotation.toMatrix();
    return rotationMatrix * inertiaTensor * rotationMatrix.transpose();
}

/**
 * @brief Aplica um torque a um corpo e atualiza sua rota��o e velocidade angular.
 *
 * Usa o torque aplicado para calcular a acelera��o angular e atualiza a rota��o do corpo.
 *
 * @param angularVelocity A velocidade angular atual do corpo (em radianos por segundo).
 * @param torque O torque aplicado ao corpo (em Newton-metros).
 * @param inertia O momento de in�rcia do corpo (em kg�m�).
 * @param rotation A rota��o atual do corpo como um quat�rnion.
 * @param deltaTime O intervalo de tempo (em segundos).
 * @return A nova rota��o do corpo.
 */
quaternion applyTorqueAndUpdateRotation(vector3d& angularVelocity, const vector3d& torque, decimal inertia, quaternion& rotation, decimal deltaTime) {
    // Calcula a acelera��o angular com base no torque.
    vector3d angularAcceleration = calculateAngularAcceleration(torque, inertia);
    // Atualiza a velocidade angular.
    angularVelocity = updateAngularVelocity(angularVelocity, angularAcceleration, deltaTime);
    // Atualiza a rota��o do corpo.
    rotation = integrateRotation(rotation, angularVelocity, deltaTime);
    return rotation;
}
} // namespace gpp
