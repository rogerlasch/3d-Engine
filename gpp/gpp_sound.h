

#ifndef GPP_SOUND_H
#define GPP_SOUND_H

namespace gpp
{

enum GPP_SOUND_FLAGS
{
GSOUND_LOOPED=(1<<0),
GSOUND_AUTOFREE=(1<<1)
};

//Sound interface...
gpp_sound
{
private:
bool active;
vector3d listenerposition;
gpp_sound* hsound=NULL;
public:
static inline void setSound(gpp_sound* gs)
{
if(hsound!=NULL)
{
delete hsound;
}
this->hsound=gs;
}
static inline gpp_sound* getSound()const
{
return this->hsound;
}
virtual bool init()=0;
virtual void unload()=0;
virtual void setListenerPosition(const vector3d& position)=0;
virtual vector3d getListenerPosition()const=0;
virtual bool setSoundPosition(uint32 handle, const vector3d& position)=0;
virtual vector3d getSoundPosition(uint32 handle)const=0;
virtual uint32 playStationary(const std::string& filename, uint32 flags=0)=0;
virtual uint32 play3d(const std::string& filename, const vector3d& sound_position, uint32 flags=0)=0;
virtual bool destroySound(uint32 handle)=0;
};
}
#endif
