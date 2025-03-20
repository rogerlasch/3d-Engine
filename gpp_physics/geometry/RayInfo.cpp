

#include"geometry.h"
#include"RayInfo.h"

using namespace std;

namespace gpp{

RayInfo::RayInfo(){
clear();
}

RayInfo::RayInfo(const vector3d& origin, const vector3d& dir){
this->origin=origin;
this->dir=dir;
id=0;
target=NULL;
enterPoint={0,0,0};
outPoint={0,0,0};
enterDist=0;
outDist=0;
colliding=false;
}

RayInfo::RayInfo(const RayInfo& rinfo){
*this=rinfo;
}

RayInfo& RayInfo::operator=(const RayInfo& rinfo){

if(this!=&rinfo){

this->id=rinfo.id;
this->target=rinfo.target;
this->origin=rinfo.origin;
this->dir=rinfo.dir;
this->enterDist=rinfo.enterDist;
this->outDist=rinfo.outDist;
this->enterPoint=rinfo.enterPoint;
this->outPoint=rinfo.outPoint;
this->colliding=rinfo.colliding;
}

return *this;
}

string RayInfo::toString()const{
stringstream ss;
ss<<fixed;
ss.precision(2);

ss<<"id="<<id<<", hasTarget?="<<boolalpha<<(target!=NULL)<<", isColliding?="<<boolalpha<<colliding<<endl;
ss<<"origin="<<origin<<", dir="<<dir<<endl;
ss<<"enterDist="<<enterDist<<", enterPoint="<<enterPoint<<endl;
ss<<"outDist="<<outDist<<", outPoint="<<outPoint<<endl;
return ss.str();
}

void RayInfo::clear() {
id=0;
target=NULL;
origin={0,0,0};
dir={0,0,0};
enterPoint={0,0,0};
outPoint={0,0,0};
enterDist=0;
outDist=0;
colliding=false;
}
}
