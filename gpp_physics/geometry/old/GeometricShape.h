
#ifndef GEOMETRICSHAPE_H
#define GEOMETRICSHAPE_H

#include"../math/math.h"

namespace gpp {

enum GEOMETRICTYPES {
    GTYPE_DEFAULT = 0,
    GTYPE_SPHERE = 1,
    GTYPE_BOX = 2,
    GTYPE_CAPSULE = 4,
GTYPE_CYLINDER=8
};

class GeometricShape {
private:
    uint32 gtype;

public:

vector3d lastPosition;
    vector3d position;
    quaternion orientation;

public:
    GeometricShape(uint32 gtype, const vector3d& position = { 0.0f, 0.0f, 0.0f }, const quaternion& orientation = quaternion()) : gtype(gtype), position(position), orientation(orientation) {lastPosition=position;}

    virtual ~GeometricShape() {}

inline virtual     std::string toString() const {
        return "Type: " + std::to_string(gtype) + "\nPosition=" + position.toString() + "\nOrientation=" + orientation.toString();
    }

inline uint32 getType()const{return gtype;}

inline virtual void setLastPosition(const vector3d& lastPosition){this->lastPosition=lastPosition;}
inline vector3d getLastPosition()const{return this->lastPosition;}

inline virtual void setPosition(const vector3d& position){this->position=position;}
inline vector3d getPosition()const{return this->position;}

inline void setOrientation(const quaternion& q){this->orientation=q;}
inline quaternion getOrientation()const{return this->orientation;}

inline     virtual bool collidingPoint(const vector3d& pt) { return false; }

inline virtual vector3d getSupportPoint(const vector3d& dir)const{return position;}
inline     virtual vector3d getClosestPoint(const vector3d& pt) { return position; }

inline     virtual void rotate(const quaternion& q) {}

inline     virtual void rotate(const vector3d& origin, const quaternion& q) {}

inline     virtual void translate(const vector3d& ts) {}
inline virtual void getAABB(vector3d& tMin, vector3d& tMax)const {tMin=position; tMax=position;}

inline virtual void updateOrientation(const quaternion& q){
this->orientation=q*orientation;
if(orientation.length()>1.0f){
orientation.normalize();
}
}
};
}
#endif
