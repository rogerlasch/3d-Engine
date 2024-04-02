
#ifndef BOX3D_H
#define BOX3D_H

namespace gpp {

class Box3d : public GeometricShape {
public:
vector3d alf;
std::array<vector3d, 3> axis;
Box3d(const vector3d& position={0.0f, 0.0f, 0.0f}, const vector3d& alf={0.0f, 0.0f, 0.0f}, const quaternion& orientation=quaternion());
    virtual ~Box3d();
virtual std::string toString() const;
virtual bool collidingPoint(const vector3d& pt);
virtual vector3d getClosestPoint(const vector3d& pt);
    virtual void rotate(const quaternion& q) ;
    virtual void rotate(const vector3d& origin, const quaternion& q) ;
    virtual void translate(const vector3d& ts) ;
    virtual void getAABB(vector3d& tMin, vector3d& tMax)const;
inline virtual void getLocalAxis(std::array<vector3d, 3>& axis){axis=this->axis;}
virtual void getVertices(std::vector<vector3d>& vertices)const;
};
}
#endif
