


#include<sstream>
#include<vector>
#include<mutex>
#include<shared_mutex>
#include<unordered_map>
#include<algorithm>
#include"../types.h"
#include"../math/math.h"
#include"../geometry/geometry.h"
#include"../collision/collision.h"
#include"../world/RigidBody.h"
#include"octree.h"

using namespace std;

namespace gpp
{
octreenode::octreenode()
{
parent=0;
id=0;
level=0;
radius=0.0f;
center.zero();
hbodies.clear();
oct=NULL;
}

octreenode::~octreenode()
{
parent=0;
id=0;
level=0;
radius=0.0f;
center={0,0,0};
hbodies.clear();
oct=NULL;
}

string octreenode::toString()const
{
stringstream ss;
ss<<fixed;
ss.precision(2);
vector3d m1=center-radius;
vector3d m2=center+radius;
ss<<"Level: "<<level<<", "<<"Id: "<<id<<", Parent:"<<parent<<endl;
string axis="XYZ";
for(uint32 i=0; i<3; i++)
{
        ss << axis[i]<<": "<<m1[i] << ":" << m2[i] << ", ";
}
ss<<endl;
ss<<"Objetos: "<<hbodies.size()<<endl;
return ss.str();
}

void octreenode::setOctree(octree* oct){
this->oct=oct;
}

octree* octreenode::getOctree()const{
return this->oct;
}

void octreenode::createChilds(){
float step=radius*0.5f;
if(pchilds==255)return;
for(uint64 i=0; i<8; i++){
if((this->pchilds&(1<<i))==0){
octreenode* sf = new octreenode();
            sf->center.x = ((i & 1) ? step : -step);
            sf->center.y = ((i & 2) ? step : -step);
            sf->center.z = ((i & 4) ? step : -step);
            sf->center +=this->center;
            sf->radius = step;
sf->id=(this->id<<3)+i;
sf->parent=id;
sf->pchilds=0;
sf->level=this->level+1;
this->pchilds|=1<<i;
sf->setOctree(getOctree());
getOctree()->insertNode(sf);
}
}

}

}
