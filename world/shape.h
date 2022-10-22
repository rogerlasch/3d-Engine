

#ifndef SHAPE_H
#define SHAPE_H

namespace gpp
{
class shape
{
protected:
uint32 gtype;
public:
shape(uint32 gtype=0) {this->gtype=gtype;}
virtual ~shape(){}
uint32 getGtype()const {return gtype;}
virtual float getArea()=0;
virtual float getVolume()=0;
virtual float getSurfaceArea()=0;
virtual vector3d getCenter()=0;
virtual bool is2d()const=0;
virtual bool is3d()const=0;
virtual void translate(const vector3d& v)=0;
virtual void rotate(float angle, const vector3d& origin)=0;
};
}
#endif
