
#ifndef CAPSULE3D_H
#define CAPSULE3D_H

namespace gpp {

class Capsule3d : public GeometricShape {
private:
    decimal radius;       // Raio da cápsula
decimal eight;
vector3d axis;

public:
    // Construtor
    Capsule3d(const vector3d& startPoint, const vector3d& endPoint, decimal radius, const Transform& transform = Transform());

    // Destrutor
    ~Capsule3d() override = default;

    // Getters e Setters

decimal getEight()const{return this->eight;}
vector3d getAxis()const{return this->axis;}
    decimal getRadius() const { return radius; }
    void setRadius(decimal r) { radius = r; }

void getSegment(vector3d& tstart, vector3d& tend)const;
    std::string toString() const override;
    vector3d getClosestPoint(const vector3d& pt) const override;
    bool contains(const vector3d& pt) const override;
    bool rayCast(RayInfo* info) const override;
    AABB getAABB() const override;
    decimal getVolume() const override;
    decimal getSurfaceArea() const override;
    matrix3x3 getInertiaTensor(decimal mass) const override;
void rotate(const quaternion& q) override ;
};
} // namespace gpp
#endif // CAPSULE3D_H
