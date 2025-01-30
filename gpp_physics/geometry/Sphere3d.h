
#ifndef SPHERE3D_H
#define SPHERE3D_H

namespace gpp {

class Sphere3d : public GeometricShape {
public:
decimal radius;

Sphere3d(const vector3d& position={0.0f, 0.0f, 0.0f}, decimal radius=0.5f, const quaternion& orientation=quaternion());
    virtual ~Sphere3d();

virtual std::string toString() const;
virtual bool collidingPoint(const vector3d& pt);
virtual vector3d getSupportPoint(const vector3d& dir)const;
virtual vector3d getClosestPoint(const vector3d& pt);
    virtual void rotate(const quaternion& q) ;
    virtual void rotate(const vector3d& origin, const quaternion& q) ;
    virtual void translate(const vector3d& ts) ;
    virtual void getAABB(vector3d& tMin, vector3d& tMax)const;
};
}
#endif
