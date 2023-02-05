#include <iostream>
#include <gpp/gpp.h>
#include <gpp_physics/gpp_physics.h>
#include <cmath>

using namespace gpp;
using namespace std;

int main() {
setlocale(LC_ALL, "Portuguese");
// Cria uma esfera
sphere3d s;
s.center = vector3d(5.0f, 5.0f, 5.0f);
s.radius = 2.0f;
// Cria uma caixa
box3d b;
b.min = vector3d(2.0f, 2.0f, 2.0f);
b.measures = vector3d(6.0f, 6.0f, 6.0f);
// Cria um objeto de dados de colisão
CollisionData data;
// Chama o método sphereBox
if (collision3d::sphereBox(&s, &b, &data)) {
cout << "Colisão detectada!" << endl;
cout << "Ponto de colisão: " << data.point.x << ", "
<< data.point.y << ", " << data.point.z << endl;
cout << "Vetor normal: " << data.normal.x << ", " << data.normal.y
<< ", " << data.normal.z << endl;
cout << "Profundidade: " << data.depth << endl;
} else {
cout << "Nenhuma colisão detectada." << endl;
}
return 0;
}
