#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>
#include"physics.h"

using namespace std;

const double PI = acos(-1.0);

int main() {
setlocale(LC_ALL, "Portuguese");
    double r = 0.0285; // raio das bolas em metros
    double m = 0.156; // massa das bolas em kg
    double e = 0.9; // coeficiente de restituição
    double v1n = 6.0; // velocidade inicial da bola 1 em m/s
    double v2n = 0.0; // velocidade inicial da bola 2 em m/s
    double theta = 30.0 * PI / 180.0; // ângulo de lançamento em radianos
    double vn, v1n_new, v2n_new, v1t_new, v2t_new, v1x_new, v1y_new, v2x_new, v2y_new;
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    // calcular o vetor normal unitário da colisão
    double nx = 0.866;
    double ny = -0.5;

    // calcular a velocidade normal relativa das bolas
    vn = (v1n - v2n) * nx;

    // calcular a velocidade normal final da bola 1
    v1n_new = (e * (v1n * m - vn * m + v2n * m) + m * v1n - m * v2n) / (2.0 * m);

    // calcular a velocidade normal final da bola 2
    v2n_new = (e * (v2n * m + vn * m - v1n * m) + m * v2n + m * v1n) / (2.0 * m);

    // calcular a velocidade tangencial final das bolas
    v1t_new = v1n_new * sin_theta;
    v2t_new = v2n_new * sin_theta;

    // calcular a velocidade final das bolas nos eixos x e y
    v1x_new = v1n_new * cos_theta + v1t_new * cos_theta;
    v1y_new = v1t_new * sin_theta;
    v2x_new = v2n_new * cos_theta + v2t_new * cos_theta;
    v2y_new = v2t_new * sin_theta;

    // imprimir as velocidades finais das bolas
    cout << "Velocidade final da bola 1: (" << v1x_new << ", " << v1y_new << ")" << endl;
    cout << "Velocidade final da bola 2: (" << v2x_new << ", " << v2y_new << ")" << endl;
    return 0;
}
