

#include"physics.h"

using namespace std;

vector3d calculateBuoyancy(const vector3d& objectSize, float fluidDensity, float gravity, const vector3d& fluidVelocity, const quaternion& orientation) {
    // Calcular a profundidade até o topo e base do objeto
    float ht = objectSize.z / 2.0;
    float hb = -objectSize.z / 2.0;

    // Calcular as pressões no topo e base do objeto
    float Pt = fluidDensity * gravity * ht;
    float Pb = fluidDensity * gravity * (-hb);

    // Calcular a força de empuxo
    float volume = objectSize.x * objectSize.y * objectSize.z;
quaternion buoyancyForce = quaternion(0, 0, fluidDensity * gravity * volume);
    buoyancyForce = orientation * buoyancyForce;
return vector3d(buoyancyForce.x, buoyancyForce.y, buoyancyForce.z);
}

double calcularForcaArrasto(double coeficienteArrasto, double areaFrontal, double densidadeFluido, double velocidade)
{
    return 0.5 * coeficienteArrasto * areaFrontal * densidadeFluido * velocidade * velocidade;
}
