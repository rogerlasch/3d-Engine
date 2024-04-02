


#include<sstream>
#include<vector>
#include<mutex>
#include<shared_mutex>
#include<algorithm>
#include"../types.h"
#include"../math/math.h"
#include"../RigidBody.h"
#include"octreenode.h"

using namespace std;

namespace gpp
{
octreenode::octreenode()
{
id=0;
level=0;
radius=0.0f;
center={0,0,0};
bodies.clear();
}

octreenode::~octreenode()
{
id=0;
level=0;
radius=0.0f;
center={0,0,0};
}

string octreenode::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
vector3d m1=center-radius;
vector3d m2=center+radius;
ss<<"Level: "<<level<<", "<<"Key: "<<id<<endl;
string axis="XYZ";
for(uint32 i=0; i<3; i++)
{
        ss << axis[i]<<": "<<m1[i] << ":" << m2[i] << ", ";
}
ss<<endl;
ss<<"Objetos: "<<bodies.size()<<endl;
return ss.str();
}
}
