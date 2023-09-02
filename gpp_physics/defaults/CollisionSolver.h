
/**
*Classe que implementa um solucionador de colisões.
*Na física, quando 2 corpos se chocam, uma série de coisas precisa acontecer.
*Calcular o impulso resultante para cada corpo é uma dessas coisas. Ou seja, cada vez que uma bola atinge o chão, ela deve quicar cada vez mais baixo até por sim ficar em repouso no chão.
*Esta classe precisa do ponto de contato da colisão entre os corpos, o vetor normal da direção da colisão, e também a profundidade de penetração, para poder afastar os corpos de forma que eles não colidam mais.
**/
#ifndef COLLISIONSOLVER_H
#define COLLISIONSOLVER_H


namespace gpp
{

class CollisionSolver : public iCollisionSolver
{
public:
void solve(std::vector<CollisionInfo>& collisions);
void solvePair(CollisionInfo& info);
};
}
#endif
