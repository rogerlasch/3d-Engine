


#ifndef RSOUND_H
#define RSOUND_H

#include<string>
#if !defined(GPP_VECTOR3D_H)
#include<gpp_physics/math/math.h>
using namespace gpp;
#endif
#include<unordered_set>

namespace rgames
{


enum R_SOUND_FLAGS
{
RSOUND_DEFAULT=0,
RSOUND_AUTOFREE=1,
RSOUND_3D=2,
RSOUND_LOOP=4,
RSOUND_PAUSED=8,
RSOUND_STREAM=16
};

enum RSOUND_ACTIVE_STATES
{
RST_UNKNOWN=0,
RST_PLAYING,
RST_PAUSED,
RST_STOPPED
};

class rsound_data
{
public:
ruint32 handle=0;
rint64 hcreate=0;
std::unordered_set<ruint32> handles;
std::string data="";
rsound_data()
{
}
rsound_data(const rsound_data& rd)
{
*this=rd;
}
rsound_data& operator=(const rsound_data& rd)
{
this->handle=rd.handle;
this->hcreate=rd.hcreate;
this->handles=rd.handles;
this->data=rd.data;
return*this;
}
};

class rsound
{
private:
rint32 idevice;
std::unordered_map<std::string, rsound_data*> hsounds;
std::unordered_map<std::string, rsound_data*> hstreams;
std::atomic<ruint32> refcount;
public:
rsound();
rsound(const rsound& rs)=delete;
rsound& operator=(const rsound& rs)=delete;
virtual ~rsound();
ruint32 getLastError()const;
ruint32 getActiveState(ruint32 handle)const;
bool deviceInit(rint32 d, ruint32 freq);
bool deviceFree(rint32 d);
bool setVolume(rint32 vol);
rint32 getVolume()const;
bool setPan(ruint32 handle, float pan);
float getPan(ruint32 handle)const;
ruint32 play(const std::string& filename, ruint32 flags=0);
ruint32 play(const std::string& filename, const vector3d& position, ruint32 flags);
bool play(ruint32 handle, bool fstart=true);
bool pause(ruint32 handle);
bool close(ruint32 handle);
bool closeStream(ruint32 handle);
//3d
bool setOrientation(const vector3d& front, const vector3d& top);
bool getOrientation(vector3d& front, vector3d& top);
bool setListenerPosition(const vector3d& position);
vector3d getListenerPosition()const;
bool setSoundPosition(ruint32 handle, const vector3d& position);
vector3d getSoundPosition(ruint32 handle);
void update();
private:
ruint32 translateFlags(ruint32 flags, bool hstream=false);
ruint32 getChannel(const std::string& filename, ruint32 flags);
ruint32 loadStream(const std::string& filename, ruint32 flags);
ruint32 loadSample(const std::string& filename, ruint32 flags);
void closeStream(rsound_data* sf);
bool loadFromFile(const std::string& filename, std::string& data);
void updateDefaultDevice();
};
}

#endif
