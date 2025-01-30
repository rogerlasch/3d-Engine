
#ifndef CAPSULE3D_H
#define CAPSULE3D_H

namespace gpp {

class Capsule3d : public GeometricShape {
public:
decimal radius;
decimal length;
vector3d axis;
Capsule3d(const vector3d& p1={0.0f, 0.0f, 0.0f}, const vector3d& p2={0.0f, 0.0f, 0.0f}, decimal radius=0.1f, const quaternion& orientation=quaternion());
    virtual ~Capsule3d();
virtual std::string toString() const;
virtual bool collidingPoint(const vector3d& pt);
virtual vector3d getSupportPoint(const vector3d& dir)const;
virtual vector3d getClosestPoint(const vector3d& pt);
    virtual void rotate(const quaternion& q) ;
    virtual void rotate(const vector3d& origin, const quaternion& q) ;
    virtual void translate(const vector3d& ts) ;
    virtual void getAABB(vector3d& tMin, vector3d& tMax)const;
virtual void getSegment(vector3d& p1, vector3d& p2)const;
};
}
#endif
