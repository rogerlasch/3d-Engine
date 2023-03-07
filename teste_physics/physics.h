


#ifndef PHYSICS_H
#define PHYSICS_H

#include<gpp_physics/gpp_physics.h>
using namespace gpp;

vector3d calculateBuoyancy(const vector3d& objectSize, float fluidDensity, float gravity, const vector3d& fluidVelocity, const quaternion& orientation);
double calcularForcaArrasto(double coeficienteArrasto, double areaFrontal, double densidadeFluido, double velocidade);

#endif
