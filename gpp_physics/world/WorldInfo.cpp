

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
ss<<"Gravidade: "<<gravity<<endl;
ss<<"Limites do mundo: ";
for(uint32 i=0; i<3; i++)
{
ss<<min[i]<<":"<<max[i];
if(i<2) ss<<",";
}
ss<<endl;
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
}

void WorldInfo::toDefault()
{
float dm=1000000;
min={0,0,0};
max={dm, dm, dm};
gravity={0,0, -9.81f};
flags=GWF_GRAVITY;
currenttime=0.0f;
timestep=0.0f;
}
}
