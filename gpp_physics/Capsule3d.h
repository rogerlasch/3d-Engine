


#ifndef CAPSULE3D_H
#define CAPSULE3D_H

namespace gpp{

class Capsule3d : virtual public  RigidBody{
public:
float alf;
float radius;
Capsule3d(float alf=0.5f, float radius=0.2f);
virtual ~Capsule3d();
virtual std::string toString()const;
virtual void calculateInertia();
void getLine(vector3d& min, vector3d& max)const;
virtual float getAabb(vector3d& min, vector3d& max)const;
};
}
#endif
