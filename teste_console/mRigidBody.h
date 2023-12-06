#ifndef MRIGIDBODY_H
#define MRIGIDBODY_H

using namespace gpp;

class mRigidBody {
public:
    float mass;
    float invMass;
    vector3d pos;
    vector3d vel;
    vector3d acceleration;
    vector3d forces;

    // Novas variáveis para representar momento angular e orientação
vector3d torque;
    vector3d angularMomentum;
    vector3d angularVelocity;
    quaternion orientation;  // Certifique-se de incluir a classe Quaternion

matrix3x3 inertia;
matrix3x3 invInertia;

    mRigidBody() {
        mass = 0.0f;
        invMass = 0.0f;
        pos = {0.0f, 0.0f, 0.0f};
        vel = {0.0f, 0.0f, 0.0f};
        forces = {0.0f, 0.0f, 0.0f};
        acceleration = {0.0f, 0.0f, 0.0f};

        // Inicialize os novos membros
torque = {0.0f, 0.0f, 0.0f};
        angularMomentum = {0.0f, 0.0f, 0.0f};
        angularVelocity = {0.0f, 0.0f, 0.0f};
        orientation = quaternion();
inertia.setDiagonal({1/6, 1/6, 1/6});
invInertia=inertia.Inverse();
    }

    void applyForce(const vector3d& f) {
        forces += f;
    }

void applyTorque(const vector3d& t){
torque+=t;
}
std::string toString() const {
    std::stringstream ss;
    ss << "Mass: " << mass << "\n";
    ss << "Inverse Mass: " << invMass << "\n";
    ss << "Position: (" << pos.x << ", " << pos.y << ", " << pos.z << ")\n";
    ss << "Velocity: (" << vel.x << ", " << vel.y << ", " << vel.z << ")\n";
    ss << "Acceleration: (" << acceleration.x << ", " << acceleration.y << ", " << acceleration.z << ")\n";
    ss << "Forces: (" << forces.x << ", " << forces.y << ", " << forces.z << ")\n";
    ss << "Angular Momentum: (" << angularMomentum.x << ", " << angularMomentum.y << ", " << angularMomentum.z << ")\n";
    ss << "Angular Velocity: (" << angularVelocity.x << ", " << angularVelocity.y << ", " << angularVelocity.z << ")\n";
    ss << "Orientation: (" << orientation.w << ", " << orientation.x << ", " << orientation.y << ", " << orientation.z << ")\n";

    return ss.str();
}

void update(float dt) {
    // Etapa 1: Calcular a aceleração atual baseada nas forças
    acceleration = forces * invMass;

    // Etapa 2: Usar o integrador RK4 para atualizar a posição e velocidade
    // Note que esta é uma versão simplificada, e não inclui orientação ou momento angular
    // Você precisará expandi-la para incluir esses conceitos.
    vector3d k1v = acceleration * dt;
    vector3d k1x = vel * dt;

    vector3d k2v = acceleration * dt;
    vector3d k2x = (vel + k1v * 0.5f) * dt;

    vector3d k3v = acceleration * dt;
    vector3d k3x = (vel + k2v * 0.5f) * dt;

    vector3d k4v = acceleration * dt;
    vector3d k4x = (vel + k3v) * dt;

    vel += (k1v + k2v * 2.0f + k3v * 2.0f + k4v) / 6.0f;
    pos += (k1x + k2x * 2.0f + k3x * 2.0f + k4x) / 6.0f;

    // Limpar as forças após a atualização
    forces = {0.0f, 0.0f, 0.0f};

    angularMomentum += torque * dt;

    angularVelocity = invInertia * angularMomentum;  // Use a matriz de inércia
    quaternion dq = (angularVelocity * orientation) * (0.5f * dt);
    orientation += dq;
    orientation.normalize();  // Normalizar o quaternion

torque={0,0,0};
}
};

#endif
