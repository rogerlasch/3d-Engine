
#pragma warning(disable: 4244)
#include<fstream>
#include<gpp/debug_system.h>
#include<unordered_set>
#include<functional>
#include<set>
#include<unordered_map>
#include<bass.h>
#include"r_types.h"
#include"rsound.h"
using namespace std;
using namespace gpp;
namespace rgames
{
inline vector3d frombvec(const BASS_3DVECTOR& bv)
{
return vector3d(bv.x, bv.z, bv.y);
}

inline BASS_3DVECTOR tobvec(const vector3d& v)
{
BASS_3DVECTOR bv;
bv.x=v.x;
bv.y=v.z;
bv.z=v.y;
return bv;
}

rsound::rsound()
{
if(refcount.load()>0)
{
throw _GEXCEPT("O sistema de som já foi inicializado...");
}
refcount.fetch_add(1);
idevice=-1;
if(!deviceInit(idevice, 44100))
{
refcount.fetch_sub(1);
throw _GEXCEPT(safe_format("Erro ao inicializar o dispositivo de som. Código: {}", getLastError()));
}
idevice=BASS_GetDevice();
}

rsound::~rsound()
{
if(refcount.load()>0)
{
refcount.fetch_sub(1);
}
for(auto it=hsounds.begin(); it!=hsounds.end(); ++it)
{
BASS_SampleFree(it->second->handle);
delete it->second;
}
for(auto it=hstreams.begin(); it!=hstreams.end(); ++it)
{
closeStream(it->second);
delete it->second;
}
hsounds.clear();
hstreams.clear();
BASS_SetDevice(idevice);
BASS_Free();
}

ruint32 rsound::getLastError()const
{
return BASS_ErrorGetCode();
}

ruint32 rsound::getActiveState(ruint32 handle)const
{
switch(BASS_ChannelIsActive(handle))
{
case BASS_ACTIVE_PLAYING:
return RST_PLAYING;
case BASS_ACTIVE_PAUSED:
return RST_PAUSED;
case BASS_ACTIVE_STOPPED:
return RST_STOPPED;
}
return RST_UNKNOWN;
}

bool rsound::deviceInit(rint32 d, ruint32 freq)
{
return BASS_Init(d, freq, BASS_DEVICE_3D, NULL, NULL);
}

bool rsound::deviceFree(rint32 d)
{
if(BASS_SetDevice(d))
{
return BASS_Free();
}
return false;
}

bool rsound::setVolume(rint32 vol)
{
return BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol);
}

rint32 rsound::getVolume()const
{
return BASS_GetConfig(BASS_CONFIG_GVOL_SAMPLE);
}

bool rsound::setPan(ruint32 handle, float pan)
{
return BASS_ChannelSetAttribute(handle, BASS_ATTRIB_PAN, pan);
}

float rsound::getPan(ruint32 handle)const
{
float value=0.0f;
BASS_ChannelGetAttribute(handle, BASS_ATTRIB_PAN, &value);
return value;

}

ruint32 rsound::play(const std::string& filename, ruint32 flags)
{
ruint32 handle=getChannel(filename, flags);
if(handle>0)
{
if((flags&RSOUND_PAUSED)==0)
{
BASS_ChannelPlay(handle, TRUE);
return handle;
}
}
return handle;
}

ruint32 rsound::play(const std::string& filename, const vector3d& position, ruint32 flags)
{
ruint32 handle=getChannel(filename, flags);
if(handle>0)
{
setSoundPosition(handle, position);
if((flags&RSOUND_PAUSED)==0)
{
BASS_ChannelPlay(handle, TRUE);
return handle;
}
}
return handle;
}

bool rsound::play(ruint32 handle, bool fstart)
{
return BASS_ChannelPlay(handle, fstart);
}

bool rsound::pause(ruint32 handle)
{
return BASS_ChannelPause(handle);
}

bool rsound::close(ruint32 handle)
{
return BASS_ChannelStop(handle);
}

bool rsound::closeStream(ruint32 handle)
{
return BASS_StreamFree(handle);
}

//3d

bool rsound::setOrientation(const vector3d& front, const vector3d& top)
{
BASS_3DVECTOR f, t;
f=tobvec(front);
t=tobvec(top);
if(BASS_Set3DPosition(NULL, NULL, &f, &t))
{
BASS_Apply3D();
return true;
}
return false;
}

bool rsound::getOrientation(vector3d& front, vector3d& top)
{
BASS_3DVECTOR f, t;
if(BASS_Get3DPosition(NULL, NULL, &f, &t))
{
front=frombvec(f);
top=frombvec(t);
return true;
}
return false;
}

bool rsound::setListenerPosition(const vector3d& position)
{
BASS_3DVECTOR p=tobvec(position);
if(BASS_Set3DPosition(&p, NULL, NULL, NULL))
{
BASS_Apply3D();
return true;
}
return false;
}

vector3d rsound::getListenerPosition()const
{
vector3d v;
BASS_3DVECTOR p;
if(BASS_Get3DPosition(&p, NULL, NULL, NULL))
{
v=frombvec(p);
}
return v;
}

bool rsound::setSoundPosition(ruint32 handle, const vector3d& position)
{
BASS_3DVECTOR p=tobvec(position);
if(BASS_ChannelSet3DPosition(handle, &p, NULL, NULL))
{
BASS_Apply3D();
return true;
}
return false;
}

vector3d rsound::getSoundPosition(ruint32 handle)
{
vector3d v;
BASS_3DVECTOR p;
if(BASS_ChannelGet3DPosition(handle, &p, NULL, NULL))
{
v=frombvec(p);
}
return v;
}

void rsound::update()
{
updateDefaultDevice();
unordered_set<string> hfiles;
int64 st=get_timestamp_ms();
for(auto it=hsounds.begin(); it!=hsounds.end(); ++it)
{
if(((st-it->second->hcreate)>300000)&&(BASS_SampleGetChannels(it->second->handle ,
NULL)<=0)) {
BASS_SampleFree(it->second->handle);
hfiles.insert(it->first);
}
}
if(hfiles.size()>0)
{
for(auto& it : hfiles)
{
hsounds.erase(it);
}
}
}

//Private metods...

ruint32 rsound::translateFlags(ruint32 flags, bool hstream)
{
ruint32 f=0;
if(hstream==false)
{
if((flags&RSOUND_LOOP)) f|=BASS_SAMPLE_LOOP;
}
else
{
f|=BASS_STREAM_PRESCAN;
if((flags&RSOUND_LOOP)) f|=BASS_SAMPLE_LOOP;
if((flags&RSOUND_3D)) f|=BASS_SAMPLE_MONO|BASS_SAMPLE_3D;
if((flags&RSOUND_AUTOFREE)) f|=BASS_STREAM_AUTOFREE;
}
return f;
}

ruint32 rsound::getChannel(const std::string& filename, ruint32 flags)
{
if((flags&RSOUND_STREAM))
{
ruint32 f=RSOUND_LOOP|RSOUND_AUTOFREE;
if((flags&f)==f)
{
_GINFO("O arquivo {} está sendo carregado com as flags RSOUND_LOOP|RSOUND_AUTOFREE. Não é possível usar as duas em conjunto. Por tanto, RSOUND_AUTOFREE foi suprimida.", filename);
flags=flags^RSOUND_AUTOFREE;
}
return loadStream(filename, flags);
}
return loadSample(filename, flags);
}

ruint32 rsound::loadStream(const string& filename, ruint32 flags)
{
auto it=hstreams.find(filename);
ruint32 bflags=translateFlags(flags, true);
if(it==hstreams.end())
{
rsound_data* sf=new rsound_data();
if(!loadFromFile(filename, sf->data))
{
_GERROR("Não foi possível carregar o arquivo {}.", filename);
delete sf;
return 0;
}
HSTREAM handle=BASS_StreamCreateFile(TRUE, sf->data.c_str(), 0, sf->data.size(), bflags);
if(handle==0)
{
delete sf;
_GERROR("Erro ao carregar o arquivo {}. Erro: {}", filename, BASS_ErrorGetCode());
return 0;
}
sf->hcreate=get_timestamp_ms();
sf->handles.insert(handle);
hstreams.insert(make_pair(filename, sf));
return handle;
}
HSTREAM handle=BASS_StreamCreateFile(TRUE, it->second->data.c_str(), 0, it->second->data.size(), bflags);
if(handle==0)
{
_GERROR("Erro ao carregar o arquivo {}. Erro: {}", filename, BASS_ErrorGetCode());
return 0;
}
it->second->handles.insert(handle);
return handle;
}

ruint32 rsound::loadSample(const string& filename, ruint32 flags)
{
auto it=hsounds.find(filename);
if(it==hsounds.end())
{
ruint32 bflags=translateFlags(flags);
bflags=bflags|BASS_SAMPLE_MONO|BASS_SAMPLE_3D|BASS_SAMPLE_VAM|BASS_SAMPLE_MUTEMAX|BASS_SAMPLE_OVER_VOL|BASS_SAMPLE_OVER_POS|BASS_SAMPLE_OVER_DIST;
ruint32 handle=BASS_SampleLoad(FALSE, filename.c_str(), 0,0, 65535, flags);
if(handle==0)
{
_GERROR("Erro ao carregar o arquivo {}. Erro: {}", filename, BASS_ErrorGetCode());
return 0;
}
rsound_data* hs=new rsound_data();
hs->handle=handle;
hs->hcreate=get_timestamp_ms();
hsounds.insert(make_pair(filename, hs));
return BASS_SampleGetChannel(handle, FALSE);
}
return BASS_SampleGetChannel(it->second->handle, FALSE);
}

void rsound::closeStream(rsound_data* sf)
{
for(auto& it :sf->handles)
{
BASS_StreamFree(it);
}
sf->handles.clear();
sf->data.resize(0);
}

bool rsound::loadFromFile(const std::string& filename, std::string& data)
{
try {
data.resize(0);
ifstream ifs(filename, ios::binary);
if(!ifs.is_open())
{
return false;
}
ifs.seekg(0, ifs.end);
streamoff size=ifs.tellg();
ifs.seekg(0, ifs.beg);
data.resize(size);
ifs.read(&data[0], size);
streamoff len=ifs.gcount();
data.resize(len);
ifs.close();
return true;
} catch(const exception& e) {
data.resize(0);
_GCRITICAL(_GEXCEPT(e.what()).what());
}
return false;
}

void rsound::updateDefaultDevice()
{
function<rint32(rint32)> FDD=[](rint32 d)->rint32{
BASS_DEVICEINFO info;
ruint32 flags=BASS_DEVICE_ENABLED|BASS_DEVICE_DEFAULT;
rint32 x=0;
while(BASS_GetDeviceInfo(x, &info))
{
if((info.flags&flags))
{
return x;
}
}
return 0;
};
function<void(ruint32, rint32)> HPP=[&](ruint32 handle, rint32 md) {
vector<ruint32> arr;
ruint32 n=BASS_SampleGetChannels(handle, NULL);
if(n==0) return;
arr.resize(n, 0);
for(auto& it : arr)
{
switch(md)
{
case 0:
BASS_ChannelSetDevice(it, idevice);
break;
case 1:
BASS_ChannelPause(it);
break;
case 2:
BASS_ChannelPlay(handle, FALSE);
}
}
};
function<void(unordered_set<ruint32>&)> hst=[&](unordered_set<ruint32>& handles){
for(auto& it : handles)
{
BASS_ChannelSetDevice(it, idevice);
}
};
rint32 d=FDD(idevice);
if(d==idevice) return;
idevice=d;
BASS_SetDevice(idevice);
BASS_Pause();
for(auto it=hsounds.begin(); it!=hsounds.end(); ++it)
{
HPP(it->second->handle, 0);
}
for(auto it=hstreams.begin(); it!=hstreams.end(); ++it)
{
hst(std::ref(it->second->handles));
}
BASS_Start();
}
}
