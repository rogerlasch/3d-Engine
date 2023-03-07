#include "structs.h"
#include <cmath>

using namespace std;

bool isColliding(const sphere3d& s, const box3d& b, CollisionData& data)
{
    // Calcular o ponto mais pr�ximo na caixa em rela��o ao centro da esfera
    float x = std::max(b.min.x, std::min(s.center.x, b.min.x + b.measures.x));
    float y = std::max(b.min.y, std::min(s.center.y, b.min.y + b.measures.y));
    float z = std::max(b.min.z, std::min(s.center.z, b.min.z + b.measures.z));

    // Calcular a dist�ncia entre o centro da esfera e o ponto mais pr�ximo na caixa
    float distance = std::sqrt((x - s.center.x) * (x - s.center.x) +
                               (y - s.center.y) * (y - s.center.y) +
                               (z - s.center.z) * (z - s.center.z));

    // Se a dist�ncia for menor que o raio da esfera, h� colis�o
    if (distance < s.radius)
    {
        // Preencher o objeto CollisionData com informa��es sobre a colis�o
        data.point = {x, y, z};
        data.normal = {(x - s.center.x) / distance, 
                        (y - s.center.y) / distance, 
                        (z - s.center.z) / distance};
        data.depth = s.radius - distance;
        return true;
    }

    return false;
}

int main()
{
setlocale(LC_ALL, "Portuguese");
sphere3d s({7, 0, 0}, 5.0f);
box3d b({0,0,0}, {10,10,10});
CollisionData data;
while(!isColliding(s, b, data))
{
s.center.x--;
}
cout<<data<<endl;
return 0;
}
