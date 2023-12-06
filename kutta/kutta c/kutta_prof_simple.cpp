#include <iostream>
#include<sstream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include"g_start.h"

using namespace gpp;
using namespace std;

double func_k1(double dt, double m, double T, double C, double v)
{
    double Farrasto = C * v * v;
    double f = T - Farrasto;
    double a = f / m;
    return dt * a;
}

double func_k2(double k1, double dt, double m, double T, double C, double v)
{
    double Farrasto = C * (v * v + k1 / 2);
    double f = T - Farrasto;
    double a = f / m;
    return dt * a;
}

double func_k3(double k2, double dt, double m, double T, double C, double v)
{
    double Farrasto = C * (v * v + k2 / 2);
    double f = T - Farrasto;
    double a = f / m;
    return dt * a;
}

double func_k4(double k3, double dt, double m, double T, double C, double v)
{
    double Farrasto = C * (v * v + k3);
    double f = T - Farrasto;
    double a = f / m;
    return dt * a;
}

int main()
{
G_STARTER gst;
    double k1, k2, k3, k4, vNovo, sNovo;
    cout.precision(4);

    // Dados do objeto
    double m = 2.0f;
    double Cd = 0.1f;
    double A = 1.0f; // Substitua pelo valor desejado
    double p = 0.1f;
    double T = 5.0f;

    // Cálculo do coeficiente de arrasto
    double C = 0.5 * p * A * Cd;

    // Dados da simulação
    double dt = 0.1f;
    int iter = 50;
    double posIni = 0.0f;
    double velIni = 0.0f;
    double tIni = 0.0f;

    // Valores iniciais
    double y[2] = {velIni, posIni};

    cout << "Início: y = " << posIni << " m, " << velIni << " m/s" << endl;

    for (double i = tIni; i < iter; i += dt)
    {
        k1 = func_k1(dt, m, T, C, y[0]);
        k2 = func_k2(k1, dt, m, T, C, y[0]);
        k3 = func_k3(k2, dt, m, T, C, y[0]);
        k4 = func_k4(k3, dt, m, T, C, y[0]);

        vNovo = y[0] + (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        sNovo = y[1] + vNovo * dt;

        y[0] = vNovo;
        y[1] = sNovo;

        cout << "Passo " << i << ": y = " << sNovo << " m, " << vNovo << " m/s" << endl;
    }

    return 0;
}
