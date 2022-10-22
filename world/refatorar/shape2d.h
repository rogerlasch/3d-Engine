

#ifndef SHAPE2D_H
#define SHAPE2d_H

namespace gpp
{

enum SHAPE_2DTYPE
{
SD2_GENERIC=0,
SD2_BOX,
SD2_CIRCLE,
SD2_CONE,
SD2_ELIPSE,
SD2_CONVEX
};

class shape2d
{
private:
int gtype;
public:
inline shape2d(int gtype){this->gtype=gtype;}
shape2d(const shape2d& ch)=delete;
shape2d& operator=(const shape2d& ch)=delete;
inline virtual ~shape2d(){}
int getGtype()const {return this->gtype;}
virtual float getArea() {return 0.00f;}
virtual float getVolume() {return 0.00f;}
virtual float getSurfaceArea() {return 0.00f;}
virtual vector3d getCenter() {return vector3d(0.00f, 0.00f, 0.00f);}
virtual bool is2d()const {return true;}
virtual bool is3d()const {return false;}
};
}
#endif
