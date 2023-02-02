

#include<signal.h>
#include<iostream>
#include<gpp/gpp.h>
#include<gpp_physics/gpp_physics.h>

using namespace std;
using namespace gpp;

int main()
{
vector3d a(1,2,3);
vector3d b(4,5,6);
vector3d c(7,8,9);
float result=vector3d::tripleProduct(a, b, c);
cout<<"Resultado: "<<result<<endl;
return 0;
}
