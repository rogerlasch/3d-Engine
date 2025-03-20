
#ifndef BOX3D_H
#define BOX3D_H

namespace gpp {

class Box3d : public GeometricShape {
private:
    vector3d extents;  // Metade das dimensões da caixa (largura, altura, profundidade)

public:
    // Construtor
    Box3d(const vector3d& extents = vector3d(1.0f, 1.0f, 1.0f), const Transform& transform = Transform());

    // Destrutor
    ~Box3d() override = default;

    // Getters e Setters
    vector3d getExtents() const { return extents; }
    void setExtents(const vector3d& e) { extents = e; }

    // Métodos da interface GeometricShape
    std::string toString() const override;
    vector3d getClosestPoint(const vector3d& pt) const override;
    bool contains(const vector3d& pt) const override;
    bool rayCast(RayInfo* info) const override;
    AABB getAABB() const override;
    decimal getVolume() const override;
    decimal getSurfaceArea() const override;
    matrix3x3 getInertiaTensor(decimal mass) const override;
void getVertices(std::vector<vector3d>& vertices)const;
};
} // namespace gpp
#endif // BOX3D_H
