


#ifndef SPHERE3D_H
#define SPHERE3D_H

namespace gpp{

class Sphere3d : virtual public RigidBody{
public:
float radius;
Sphere3d(float radius=1.0f);
virtual ~Sphere3d();
virtual std::string toString()const;
virtual void calculateInertia();
virtual float getAabb(vector3d& min, vector3d& max)const;
};
}
#endif
