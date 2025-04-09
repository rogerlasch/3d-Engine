

#include<sstream>
#include"geometry.h"

using namespace std;

namespace gpp{

    GeometricShape::GeometricShape(uint32 type, Transform* transform){
this->type=type;
this->transform=transform;

if(this->transform==NULL){
this->transform=new Transform();
}
}

GeometricShape::~GeometricShape(){
if(this->transform){
delete transform;
transform=NULL;
}
}

     string GeometricShape::toString() const{
stringstream ss;
ss<<fixed;
ss.precision(3);
quaternion q=transform->getOrientation();
ss<<"Type="<<type<<endl;
ss<<"Position="<<transform->getPosition()<<endl;
ss<<"Scale="<<transform->getScale()<<endl;
ss<<"Orientation Euler="<<quaternion_extract_euler_angles(q)<<endl;
ss<<"Orientation="<<q<<endl;

return ss.str();
}

string GeometricShape::getShortDescription()const{
stringstream ss;
ss<<fixed;
ss.precision(2);

ss<<"Type=GeometricShape, Tipo genérico para formas. Verifique o método sobrescrito. Tipo dele="<<type;
return ss.str();
}

    void GeometricShape::translate(const vector3d& translation){
transform->translate(translation);
}

void GeometricShape::rotateWithDecimal(decimal x, decimal y, decimal z){
//transform->rotate(x, y, z);
this->rotate(quaternion::fromEuler(x, y, z));
}

    void GeometricShape::rotate(const quaternion& rotation){
transform->rotate(rotation);
}

void GeometricShape::scale(const vector3d& scaling){
this->transform->setScale(scaling);
}
}
