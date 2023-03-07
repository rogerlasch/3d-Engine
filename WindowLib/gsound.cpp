


#include<gpp/debug_system.h>
#include<set>
#include<bass.h>
#include"gsound.h"

using namespace std;
using namespace gpp;

static set<int32> devices;

bool gsound_device_init(int32 index)
{
if(gsound_device_started(index))
{
return true;
}
if(BASS_Init(index, 44100, BASS_DEVICE_3D, NULL, NULL))
{
devices.insert(index);
BASS_Set3DFactors(1.0f, 1.0f, 1.0f);
BASS_Apply3D();
return true;
}
return false;
}

void gsound_device_free(int32 index)
{
if(gsound_set_device(index))
{
BASS_Free();
devices.erase(index);
}
}

void gsound_device_free_all()
{
for(auto& it : devices)
{
BASS_SetDevice(it);
BASS_Free();
}
devices.clear();
}

bool gsound_device_started(int32 index)
{
if(gsound_set_device(index))
{
return BASS_IsStarted();
}
return false;
}

bool gsound_set_device(int32 index)
{
return BASS_SetDevice(index);
}

int32 gsound_get_default_device_number()
{
class DefaultDeviceFinder
{
public:
int32 index;
string name;
DefaultDeviceFinder()
{
}
void refresh()
{
index=-1;
name="";
uint32 flags=BASS_DEVICE_ENABLED|BASS_DEVICE_DEFAULT;
BASS_DEVICEINFO info;
uint32 x=0;
while(index<0)
{
x++;
if((!BASS_GetDeviceInfo(x, &info))&&(BASS_ErrorGetCode()==BASS_ERROR_DEVICE))
{
break;
}
if((info.flags&flags)==flags)
{
index=x;
name=((info.name==NULL) ? "" : info.name);
break;
}
}
}
};
static DefaultDeviceFinder df;
df.refresh();
return df.index;
}

int32 gsound_get_device()
{
return BASS_GetDevice();
}

uint32 gsound_play(const string& filename, uint32 flags)
{
static vector3d default_position={0, 0, 0};
return gsound_play(filename, flags, default_position);
}

uint32 gsound_play(const string& filename, uint32 flags, const vector3d& position)
{
if(filename.size()==0)
{
return 0;
}
uint32 fl=0;
fl|=BASS_STREAM_PRESCAN;
if((flags&gsound_3d)) fl=BASS_SAMPLE_MONO|BASS_SAMPLE_3D;
if((flags&gsound_autofree)) fl|=BASS_STREAM_AUTOFREE;
if((flags&gsound_loop)) fl|=BASS_SAMPLE_LOOP;
uint32 handle=BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, fl);
if((handle==0)||((flags&gsound_paused)))
{
return handle;
}
if((flags&gsound_3d))
{
gsound_set_sound_position(handle, position);
}
BASS_ChannelPlay(handle, TRUE);
return handle;
}

bool gsound_close(uint32 handle)
{
return BASS_StreamFree(handle);
}

bool gsound_set_listener_position(vector3d* position, vector3d* velocity, vector3d* front, vector3d* top)
{
BASS_3DVECTOR vp, vv, vf, vt;
if(position!=NULL)
{
vp.x=position->x;
vp.y=position->z;
vp.z=position->y;
}
if(velocity!=NULL)
{
vv.x=velocity->x;
vv.y=velocity->z;
vv.z=velocity->y;
}
if(front!=NULL)
{
vf.x=front->x;
vf.y=front->z;
vf.z=front->y;
}
if(top!=NULL)
{
vt.x=top->x;
vt.y=top->z;
vt.z=top->y;
}
bool success=BASS_Set3DPosition( position==NULL ? NULL : &vp, velocity==NULL ? NULL : &vv, front==NULL ? NULL : &vf, top==NULL ? NULL : &vt);
if(success)
{
BASS_Apply3D();
}
return success;
}

bool gsound_get_listener_position(vector3d& vect)
{
BASS_3DVECTOR v;
if(!BASS_Get3DPosition(&v, NULL, NULL, NULL))
{
return false;
}
vect.x=v.x;
vect.y=v.y;
vect.z=v.z;
return true;
}

bool gsound_set_sound_position(uint32 handle, const vector3d& vect)
{
BASS_3DVECTOR v;
v.x=vect.x;
v.y=vect.y;
v.z=vect.z;
bool success=BASS_ChannelSet3DPosition(handle, &v, NULL, NULL);
if(success)
{
BASS_Apply3D();
}
return success;
}

bool gsound_get_sound_position(uint32 handle, vector3d& vect)
{
BASS_3DVECTOR v;
if(!BASS_ChannelGet3DPosition(handle, &v, NULL, NULL))
{
return false;
}
vect.x=v.x;
vect.y=v.y;
vect.z=v.z;
return true;
}

bool gsound_set_attribute(uint32 handle, uint32 attrib, float value)
{
switch(attrib)
{
case gsound_attrib_vol:
attrib=BASS_ATTRIB_VOL;
break;
case gsound_attrib_pan:
attrib=BASS_ATTRIB_PAN;
break;
default:
return false;
}
return BASS_ChannelSetAttribute(handle, attrib, value);
}

float gsound_get_attribute(uint32 handle, uint32 attrib)
{
switch(attrib)
{
case gsound_attrib_vol:
attrib=BASS_ATTRIB_VOL;
break;
case gsound_attrib_pan:
attrib=BASS_ATTRIB_PAN;
break;
default:
return false;
}
float val=-1;
if(!BASS_ChannelGetAttribute(handle, attrib, &val))
{
return -1;
}
return val;
}
