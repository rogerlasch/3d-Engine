

#ifndef SHAPE_H
#define SHAPE_H

namespace gpp
{


class shape
{
public:
virtual float getArea()=0;
virtual float getVolume()=0;
virtual float getSurfaceArea()=0;
virtual vector3d getCenter()=0;
virtual bool is2d()const=0;
virtual bool is3d()const=0;
};
}
#endif
