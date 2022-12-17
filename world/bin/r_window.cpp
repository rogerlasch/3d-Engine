


#include<sstream>
#include<stdexcept>
#include<unordered_map>
#include<string>
#include<bass.h>
#include<Tolk.h>
#include<sdl2/sdl.h>
#include<fmt/core.h>
#include"../types.h"
#include"../geometry/vector3d.h"
#include"../../gpp/gpp.h"
#include"r_window.h"

using namespace gpp;
using namespace std;

static r_window* mainwindow=NULL;
static unordered_map<int32, uint32> keyboard;

wstring dlb_string_to_wstring(const string& str)
{
locale loc;
const auto& _Facet=use_facet<codecvt<wchar_t, char, mbstate_t>>(loc);
wstring final=L"";
final.resize(str.size());
mbstate_t _State{};
const char* istart=str.c_str();
const char* iend=(str.c_str()+str.size());
const char* inext=NULL;
wchar_t* ostart=(wchar_t*)final.c_str();
wchar_t* oend=(wchar_t*)(final.c_str()+final.size());
wchar_t* onext=NULL;
const auto Result=_Facet.in(_State, istart, iend, inext, ostart, oend, onext);
if(Result==codecvt_base::ok)
{
final.resize(static_cast<size_t>(onext-ostart));
return final;
}
return L"";
}

r_window::r_window(const string& title, uint32 x, uint32 y, uint32 width, uint32 height)
{
this->title=title;
if(!BASS_Init(-1, 44100, BASS_DEVICE_3D, NULL, NULL))
{
throw runtime_error(fmt::format("Erro ao iniciar o bass. Erro: {}", BASS_ErrorGetCode()));
}
int32 res=SDL_Init(SDL_INIT_VIDEO);
if(res!=0)
{
throw runtime_error(fmt::format("Erro ao iniciar a sdl. Erro: {}", res));
}
this->handle=SDL_CreateWindow( this->title.c_str(), x, y, width, height, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);
if(this->handle==NULL)
{
BASS_Free();
SDL_Quit();
throw runtime_error("Não foi possível criar a janela principal do programa.");
}
this->renderer=SDL_CreateRenderer(this->handle, -1, 0);
if(this->renderer==NULL)
{
SDL_DestroyWindow(this->handle);
BASS_Free();
SDL_Quit();
throw runtime_error("Falha ao criar o renderizador da janela.");
}
Tolk_Load();
SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
keyboard.clear();
devices.insert(BASS_GetDevice());
mainwindow=this;
int64 start=get_timestamp_ms();
while((abs(get_timestamp_ms()-start))<350)
{
SDL_Delay(5);
SDL_Event ev;
if(SDL_PollEvent(&ev)!=0)
{
SDL_PushEvent(&ev);
}
}
}

r_window::~r_window()
{
if(this->handle!=NULL)
{
SDL_DestroyWindow(this->handle);
}
for(auto it=devices.begin(); it!=devices.end(); ++it)
{
BASS_SetDevice((*it));
BASS_Free();
}
this->handle=NULL;
this->renderer=NULL;
this->title="";
this->device=-1;
this->devices.clear();
mainwindow=NULL;
SDL_Quit();
Tolk_Unload();
}

void r_window::setTitle(const string& title)
{
if(this->handle!=NULL)
{
this->title=title;
SDL_SetWindowTitle(this->handle, title.c_str());
}
}

string r_window::getTitle()const
{
return this->title;
}

int32 r_window::getDevice()const
{
return this->device;
}

void r_window::setGvol(int32 gvol)
{
BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, gvol);
}

int32 r_window::getGvol()const
{
return BASS_GetConfig(BASS_CONFIG_GVOL_STREAM);
}

bool r_window::setDevice(int32 d)
{
if(devices.find(d)==devices.end())
{
if(!BASS_Init(d, 44100, BASS_DEVICE_3D, NULL, NULL))
{
return false;
}
devices.insert(d);
}
if(!BASS_SetDevice(d))
{
return false;
}
this->device=d;
BASS_Start();
return true;
}

bool r_window::is_open()const
{
return this->handle!=NULL;
}

void r_window::close()
{
if(this->handle!=NULL)
{
SDL_DestroyWindow(this->handle);
}
this->handle=NULL;
this->renderer=NULL;
this->title="";
}

void r_window::make_loop()
{
if(!is_open())
{
return;
}
SDL_Event ev;
while(SDL_PollEvent(&ev)!=0)
{
if(key_pressed(SDLK_ESCAPE))
{
this->close();
}
switch(ev.type)
{
case SDL_QUIT:
case SDL_APP_TERMINATING:
{
this->close();
break;
}
case SDL_KEYDOWN:
{
auto it=keyboard.find(ev.key.keysym.sym);
if(it==keyboard.end())
{
keyboard.insert(make_pair(ev.key.keysym.sym, sk_pressed));
}
else
{
switch(it->second)
{
case sk_up:
{
it->second=sk_pressed;
break;
}
case sk_pressed:
case sk_down:
{
it->second=sk_down;
break;
}
}
}
break;
}
case SDL_KEYUP:
{
auto it=keyboard.find(ev.key.keysym.sym);
if(it!=keyboard.end())
{
it->second=sk_up;
}
break;
}
case SDL_WINDOWEVENT:
case SDL_TEXTEDITING:
case SDL_TEXTINPUT:
case SDL_MOUSEMOTION:
case SDL_MOUSEBUTTONDOWN:
case SDL_MOUSEBUTTONUP:
case SDL_MOUSEWHEEL:
{
break;
}
}
SDL_RenderPresent(this->renderer);
}
check_sounddevices();
}

void r_window::check_sounddevices()
{
if(!this->is_open())
{
return;
}
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
DefaultDeviceFinder df;
df.refresh();
if((df.index==-1)||(df.index==device))
{
return;
}
this->setDevice(df.index);
}

//Funções de teclado...

bool key_pressed(int32 key_code)
{
auto it=keyboard.find(key_code);
if(it==keyboard.end())
{
return false;
}
if(it->second==sk_pressed)
{
it->second=sk_down;
return true;
}
return false;
}

bool key_down(int32 key_code)
{
auto it=keyboard.find(key_code);
return ((it==keyboard.end()) ? false : it->second==sk_down);
}

bool key_up(int32 key_code)
{
auto it=keyboard.find(key_code);
return ((it==keyboard.end()) ? true : it->second==sk_up);
}

uint32 rsound_play(const std::string& filename, uint32 flags)
{
static vector3d default_position={0, 0, 0};
return rsound_play(filename, flags, default_position);
}

uint32 rsound_play(const string& filename, uint32 flags, const vector3d& position)
{
if(filename.size()==0)
{
return 0;
}
uint32 fl=0;
fl|=BASS_STREAM_PRESCAN;
if((flags&rsound_3d))
{
fl=BASS_SAMPLE_MONO|BASS_SAMPLE_3D;
}
if((flags&rsound_autofree))
{
fl|=BASS_STREAM_AUTOFREE;
}
if((flags&rsound_loop))
{
fl|=BASS_SAMPLE_LOOP;
}
BASS_SetDevice(mainwindow->getDevice());
uint32 handle=BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, fl);
if((handle==0)||((flags&rsound_paused)))
{
return handle;
}
if((flags&rsound_3d))
{
rsound_set_sound_position(handle, position);
}
BASS_ChannelPlay(handle, TRUE);
return handle;
}

bool rsound_close(uint32 handle)
{
return BASS_StreamFree(handle);
}

bool rsound_set_listener_position(const vector3d& vect)
{
BASS_3DVECTOR v;
v.x=vect.x;
v.y=vect.z;
v.z=vect.y;
bool success=BASS_Set3DPosition(&v, NULL, NULL, NULL);
if(success)
{
BASS_Apply3D();
}
return success;
}

bool rsound_get_listener_position(vector3d& vect)
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

bool rsound_set_sound_position(uint32 handle, const vector3d& vect)
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

bool rsound_get_sound_position(uint32 handle, vector3d& vect)
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

void r_speak(const std::string& s, bool interrupt)
{
Tolk_Speak(dlb_string_to_wstring(s).c_str(), interrupt);
}
