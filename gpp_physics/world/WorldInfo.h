


#ifndef WORLDINFO_H
#define WORLDINFO_H

namespace gpp{

enum GPP_WORLD_FLAGS{
GWF_GRAVITY=(1<<0),//Habilita gravidade...
GWF_WIND=(1<<1)//Habilita vento...
};

class WorldInfo{
public:
float currenttime;//Hora do mundo, desde o início...
float timestep;//Último passo de tempo usado...
vector3d min;
vector3d max;
vector3d gravity;
uint32 flags;
WorldInfo();
WorldInfo(const WorldInfo& dinfo);
WorldInfo& operator=(const WorldInfo& dinfo);
virtual ~WorldInfo();
std::string toString()const;
void cleanup();
void toDefault();
};
}
#endif
