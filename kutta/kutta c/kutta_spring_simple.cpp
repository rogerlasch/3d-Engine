
#include<iostream>
#include"vec.h"

using namespace std;

    struct State
    {
        float x;      // position
        float v;      // velocity
    };

    float acceleration( const State & state, double t );
    struct Derivative
    {
        float dx;      // dx/dt = velocity
        float dv;      // dv/dt = acceleration
    };

    Derivative evaluate( const State & initial,
                         double t,
                         float dt,
                         const Derivative & d )
    {
        State state;
        state.x = initial.x + d.dx*dt;
        state.v = initial.v + d.dv*dt;
        Derivative output;
        output.dx = state.v;
        output.dv = acceleration( state, t+dt );
        return output;
    }

    float acceleration( const State & state, double t )
    {
        const float k = 15.0f;
        const float b = 0.1f;
        return -k * state.x - b * state.v;
    }

    void integrate( State & state,
                    double t,
                    float dt )
    {
        Derivative a,b,c,d;
        a = evaluate( state, t, 0.0f, Derivative() );
        b = evaluate( state, t, dt*0.5f, a );
        c = evaluate( state, t, dt*0.5f, b );
        d = evaluate( state, t, dt, c );
        float dxdt = 1.0f / 6.0f *
            ( a.dx + 2.0f * ( b.dx + c.dx ) + d.dx );
        float dvdt = 1.0f / 6.0f *
            ( a.dv + 2.0f * ( b.dv + c.dv ) + d.dv );
        state.x = state.x + dxdt * dt;
        state.v = state.v + dvdt * dt;
    }

int main() {
    // Configuração inicial
    State state;
    state.x = 1.0f; // Posição inicial
    state.v = 0.0f; // Velocidade inicial
    float dt = 0.1f; // Intervalo de tempo (passo de simulação)

    for (int i = 0; i < 50; i++) {
        cout << "Iteração " << i << ": Posição = " << state.x << ", Velocidade = " << state.v << endl;

        // Atualiza o estado usando o algoritmo RK4
        integrate(state, i * dt, dt);
    }

    return 0;
}
