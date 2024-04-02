

#ifndef BODYHELPER_H
#define BODYHELPER_H

namespace gpp{

template<class T>
inline T* createBodySphere(uint32 id, const vector3d& position, float radius, const quaternion& orientation=quaternion_from_euler_angles(0,0,0)){
T* rb=new T();
rb->setId(id);
Sphere3d* s=new Sphere3d(position, radius, orientation);
rb->setGeometricShape(s);
return rb;
}

template<class T>
inline T* createBodyBox(uint32 id, const vector3d& position, const vector3d& alf, const quaternion& orientation=quaternion_from_euler_angles(0,0,0)){
T* rb=new T();
rb->setId(id);
Box3d* b=new Box3d(position, alf, orientation);
rb->setGeometricShape(b);
return rb;
}

template<class T>
inline T* createBodyCapsule(uint32 id, const vector3d& p1, const vector3d& p2, decimal radius, const quaternion& orientation=quaternion_from_euler_angles(0,0,0)){
T* rb=new T();
rb->setId(id);
Capsule3d* b=new Capsule3d(p1, p2, radius, orientation);
rb->setGeometricShape(b);
return rb;
}
}
#endif
