#include <iostream>
#include<sstream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include"g_start.h"

using namespace gpp;
using namespace std;

vector3d func_k1(float dt, float m, const vector3d& T, float C, const vector3d& v)
{
float Farrasto = C * vector3d::dotProduct(v, v);
vector3d vdir=-vector3d::normalize(v)+numeric_limits<float>::epsilon();
_GINFO("Vdir: {}, fa:{}, fa2:{}", vdir.toString(), Farrasto, (vdir*Farrasto).toString());
vector3d f = T +(vdir*Farrasto);
vector3d a = f / m;
    return dt * a;
}

vector3d func_k2(const vector3d&  k1, float dt, float m, const vector3d& T, float C, const vector3d& v)
{
float Farrasto = C * (vector3d::dotProduct(v, v) + k1.length() / 2);
vector3d vdir=-vector3d::normalize(v)+numeric_limits<float>::epsilon();
vector3d f = T +(vdir*Farrasto);
vector3d a = f / m;
    return dt * a;
}

vector3d func_k3(const vector3d&  k2, float dt, float m, const vector3d& T, float C, const vector3d& v)
{
float Farrasto = C * (vector3d::dotProduct(v, v) + k2.length() / 2);
vector3d vdir=-vector3d::normalize(v)+numeric_limits<float>::epsilon();
vector3d f = T +(vdir*Farrasto);
vector3d a = f / m;
    return dt * a;
}

vector3d func_k4(const vector3d& k3, float dt, float m, const vector3d& T, float C, const vector3d& v)
{
vector3d Farrasto = C * (vector3d::dotProduct(v, v) + k3.length());
vector3d vdir=-vector3d::normalize(v)+numeric_limits<float>::epsilon();
vector3d f = T +(vdir*Farrasto);
vector3d a = f / m;
    return dt * a;
}

int main()
{
G_STARTER gst;
vector3d k1, k2, k3, k4, vNovo, sNovo;
    cout.precision(4);

    // Dados do objeto
    float m = 2.0f;
    float Cd = 0.1f;
    float A = 1.0f; // Substitua pelo valor desejado
    float p = 0.1f;
vector3d T = {-9.81f, 0.0f, 0.0f};

    // Cálculo do coeficiente de arrasto
    float C = 0.5 * p * A * Cd;

    // Dados da simulação
    float dt = 0.1f;
    int iter = 50;
vector3d posIni = {0.0f, 0.0f, 0.0f};
vector3d velIni = {0.0f, 0.0f, 0.0f};
    float tIni = 0.0f;

    // Valores iniciais
vector3d y[2] = {velIni, posIni};

    cout << "Início: y = " << posIni << " m, " << velIni << " m/s" << endl;

    for (float i = tIni; i < iter; i += dt)
    {
        k1 = func_k1(dt, m, T, C, y[0]);
        k2 = func_k2(k1, dt, m, T, C, y[0]);
        k3 = func_k3(k2, dt, m, T, C, y[0]);
        k4 = func_k4(k3, dt, m, T, C, y[0]);

        vNovo = y[0] + (k1 + 2 * k2 + 2 * k3 + k4) / 6+numeric_limits<float>::epsilon();
        sNovo = y[1] + vNovo * dt;

        y[0] = vNovo;
        y[1] = sNovo;

//        cout << "Passo " << i << ": y = " << sNovo << " m, " << vNovo << " m/s" << endl;
    }

    return 0;
}
