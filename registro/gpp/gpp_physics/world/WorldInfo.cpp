

#include<sstream>
#include"../common.h"
#include"../math/math.h"
#include"WorldInfo.h"

using namespace std;

namespace gpp{
WorldInfo::WorldInfo()
{
cleanup();
}

WorldInfo::WorldInfo(const WorldInfo& dinfo)
{
*this=dinfo;
}

WorldInfo& WorldInfo::operator=(const WorldInfo& dinfo)
{
this->currenttime=dinfo.currenttime;
this->timestep=dinfo.timestep;
this->airDencity=dinfo.airDencity;
this->min=dinfo.min;
this->max=dinfo.max;
this->gravity=dinfo.gravity;
this->flags=dinfo.flags;
return *this;
}

WorldInfo::~WorldInfo()
{
}

string WorldInfo::toString()const
{
#define _TF(d) (((flags&d)==d) ? "True" : "False")
stringstream ss;
ss<<fixed;
ss.precision(2);
ss<<"Tempo atual do mundo em segundos: "<<currenttime<<endl;
ss<<"Último passo de tempo: "<<timestep<<endl;
ss<<"Gravidade habilitada? "<<_TF(GWF_GRAVITY)<<endl;
ss<<"Dencidade do ar habilitado? "<<_TF(GWF_AIRDENCITY)<<endl;
if((flags&GWF_AIRDENCITY)){
ss<<"Dencidade do ar: "<<airDencity<<endl;
}
ss<<"Gravidade: "<<gravity<<endl;
ss<<"Limites do mundo: ";
string s = "XYZ";
for(uint32 i=0; i<3; i++)
{
ss<<s[i]<<":"<<min[i]<<":"<<max[i]<<endl;
}
return ss.str();
}

void WorldInfo::cleanup()
{
min={0,0,0};
max={0,0,0};
gravity={0,0,0};
currenttime=0;
timestep=0.0f;
flags=0;
airDencity=0.0f;
}

void WorldInfo::toDefault()
{
float dm=1000000;
min={0,0,0};
max={dm, dm, dm};
gravity={0,0, -9.81f};
airDencity=1.25f;
flags=GWF_GRAVITY|GWF_AIRDENCITY;;
currenttime=0.0f;
timestep=0.0f;
}
}
