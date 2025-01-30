

#include<sstream>
#include"../types.h"
#include"../math/vector3d.h"
#include"WorldInfo.h"

using namespace std;

namespace gpp{
WorldInfo::WorldInfo(){
cleanup();
}

WorldInfo::WorldInfo(const WorldInfo& wf){
*this=wf;
}

WorldInfo& WorldInfo::operator=(const WorldInfo& wf){
if(this!=&wf){
this->gravity=wf.gravity;
this->center=wf.center;
this->alf=wf.alf;
}
return *this;
}

string WorldInfo::toString()const{
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Gravidade="<<gravity<<endl;
ss<<"Centro="<<center<<endl;
ss<<"Alf="<<alf<<endl;
return ss.str();
}

void WorldInfo::cleanup(){
gravity.zero();
center.zero();
alf=1.0f;
}

void WorldInfo::toDefault(){
gravity={0.0f, 0.0f, -9.81f};
center.zero();
alf=1024*1024;
}
}
