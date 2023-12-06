#include <iostream>
#include "vec.h"

using namespace std;

struct State
{
float mass;
vector3d forces;
    vector3d pos;  // posição (x, y, z)
    vector3d vel;  // velocidade (vx, vy, vz)
};

struct Derivative
{
    vector3d dx;  // dx/dt = velocidade
    vector3d dv;  // dv/dt = aceleração
};

Derivative evaluate(const State &initial, double t, float dt, const Derivative &d)
{
    State state;
    state.pos = initial.pos + d.dx * dt;
    state.vel = initial.vel + d.dv * dt;
    Derivative output;
    output.dx = state.vel;
    output.dv =initial.forces/initial.mass;
    return output;
}

void integrate(State &state, double t, float dt)
{
    Derivative a, b, c, d;
    a = evaluate(state, t, 0.0f, Derivative());
    b = evaluate(state, t, dt * 0.5f, a);
    c = evaluate(state, t, dt * 0.5f, b);
    d = evaluate(state, t, dt, c);
    vector3d dxdt = (1.0f / 6.0f) * (a.dx + 2.0f * (b.dx + c.dx) + d.dx);
    vector3d dvdt = (1.0f / 6.0f) * (a.dv + 2.0f * (b.dv + c.dv) + d.dv);
    state.pos = state.pos + dxdt * dt;
    state.vel = state.vel + dvdt * dt;
}

int main()
{
    cout << fixed;
    cout.precision(2);
    State state;
state.mass=5.0f;
state.forces={0,0,0};
    state.pos = {50,50,50};
    state.vel = {0.0f, 0.0f, 0.0f}; // Velocidade inicial (vx, vy, vz)
    float dt = 0.2f; // Intervalo de tempo (passo de simulação)

    for (int i = 0; i < 50; i++)
    {
        cout << "Iteração " << i << ": Posição = (" << state.pos.x << ", " << state.pos.y << ", " << state.pos.z << "), Velocidade = (" << state.vel.x << ", " << state.vel.y << ", " << state.vel.z << ")" << endl;

        // Atualiza o estado usando o algoritmo RK4
state.forces={0,0,-9.81f*state.mass};
integrate(state, (i*dt), dt);
    }

    return 0;
}
