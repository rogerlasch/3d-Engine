


#include<sstream>
#include<vector>
#include<mutex>
#include<shared_mutex>
#include<algorithm>
#include"../gpp_physics.h"
#include"octreenode.h"

using namespace std;
namespace gpp
{
octreenode::octreenode()
{
level=0;
radius=0.0f;
center={0,0,0};
parent=NULL;
childs.clear();
bodies.clear();
}

octreenode::~octreenode()
{
level=0;
radius=0.0f;
center={0,0,0};
parent=NULL;
bodies.clear();
for(auto& it : childs)
{
delete it;
}
childs.clear();
}

string octreenode::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
vector3d m1=center-radius;
vector3d m2=center+radius;
ss<<"Nível: "<<level<<endl;
for(uint32 i=0; i<3; i++)
{
        ss << m1[i] << ":" << m2[i] << ", ";
}
ss<<endl;
ss<<"Objetos: "<<bodies.size()<<endl;
for (auto& it : childs) {
    ss << it->toString() << endl;
}
return ss.str();
}
}
