
#ifndef AABB_H
#define AABB_H

namespace gpp {

class AABB {
public:
    vector3d min; // Canto mínimo da AABB
    vector3d max; // Canto máximo da AABB

    // Construtores
    AABB(const vector3d& m1 = {0, 0, 0}, const vector3d& m2 = {0, 0, 0});
    AABB(const AABB& ab);
    AABB& operator=(const AABB& ab);

    // Métodos
    std::string toString() const; // Retorna uma descrição textual da AABB
    vector3d getCenter() const;   // Retorna o centro da AABB
    vector3d getExtents() const;  // Retorna as extensões da AABB (largura, altura, profundidade)
    vector3d getDirection(const AABB& ab) const; // Retorna a direção da AABB atual para outra AABB
    bool intersects(const AABB& ab) const; // Verifica se há interseção com outra AABB
    void addAABB(const AABB& ab); // Expande a AABB para incluir outra AABB
    bool rayCast(RayInfo* info) const; // Realiza raycasting contra a AABB
};
} // namespace gpp
#endif // AABB_H
