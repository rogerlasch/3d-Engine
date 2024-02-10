


#ifndef BOX3D_H
#define BOX3D_H

namespace gpp{

class Box3d : virtual public RigidBody{
public:
vector3d alf;
Box3d(const vector3d& alf={1.0f, 1.0f, 1.0f});
virtual ~Box3d();
std::string toString()const override;
virtual void calculateInertia()override;
void getMinMax(vector3d& min, vector3d& max);
virtual float getAabb(vector3d& min, vector3d& max)const override;
};
}
#endif
