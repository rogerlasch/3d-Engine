
#ifndef SPHERE3D_H
#define SPHERE3D_H

namespace gpp {

class Sphere3d : public GeometricShape {
private:
    decimal radius;

public:
    Sphere3d(decimal radius, const Transform& transform = Transform());
    ~Sphere3d() override = default;

    decimal getRadius() const{return radius;}
    void setRadius(decimal r){this->radius=r;}

    std::string toString() const override;
    vector3d getClosestPoint(const vector3d& pt) const override;
    bool contains(const vector3d& pt) const override;
    bool rayCast(RayInfo* info) const override;
    AABB getAABB() const override;
    decimal getVolume() const override;
    decimal getSurfaceArea() const override;
    matrix3x3 getInertiaTensor(decimal mass) const override;
};
} // namespace gpp
#endif // SPHERE3D_H
