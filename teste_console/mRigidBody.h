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

    mRigidBody(float mass) {
this->mass=mass;
        invMass = 1/mass;
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
    // Etapa 1: Calcular a aceleração linear atual baseada nas forças
    acceleration = forces * invMass;

    // Etapa 2: Atualizar a velocidade linear e a posição usando o método de Euler Simples
    vel += acceleration * dt;
    pos += vel * dt;

    // Etapa 3: Limpar as forças lineares após a atualização
    forces = {0.0f, 0.0f, 0.0f};

    // Etapa 4: Calcular a aceleração angular atual baseada nos torques
    vector3d angularAcceleration = invInertia * torque;

_GINFO("vv: {}", angularAcceleration .toString());
    // Etapa 5: Atualizar a velocidade angular, orientação e posição angular usando o método de Euler Simples
    angularVelocity += angularAcceleration * dt;
    orientation += quaternion(0.5f * dt, angularVelocity) * orientation;
    orientation.normalize();

    // Etapa 6: Limpar os torques angulares após a atualização
    torque = {0.0f, 0.0f, 0.0f};
}
};

#endif
