


#include<gl/glew.h>
#include<sstream>
#include<mutex>
#include<stdexcept>
#include<unordered_map>
#include<map>
#include<string>
#include<bass.h>
#include<Tolk.h>
#include<gpp/debug_system.h>
#include<gpp/string_utils.h>
#include"g_window.h"

using namespace std;
using namespace gpp;

static g_window* mainwindow=NULL;
static mutex mtx_window;

g_window::g_window(const string& title, uint32 x, uint32 y, uint32 width, uint32 height)
{
this->title=title;
if(!gsound_device_init(-1))
{
throw runtime_error("Erro ao inicializar o sistema de som...");
}
this->setDevice(gsound_get_device());
int32 res=SDL_Init(SDL_INIT_VIDEO);
if(res!=0)
{
gsound_device_free_all();
throw runtime_error(std::vformat("Erro ao iniciar a sdl. Erro: {}", std::make_format_args(res)));
}
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
handle = SDL_CreateWindow(title.c_str(), x, y, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!handle)
    {
gsound_device_free_all();
SDL_Quit();
throw runtime_error("Erro ao criar a janela sdl.");
    }
renderer= SDL_GL_CreateContext(handle);
    if (!renderer)
    {
gsound_device_free_all();
SDL_DestroyWindow(handle);
SDL_Quit();
throw runtime_error("Erro ao criar o contexto opengl.");
    }
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
gsound_device_free_all();
SDL_DestroyWindow(handle);
SDL_Quit();
throw runtime_error("Error to init GLEW!");
    }
glViewport(x, y, width, height);
gspeak_init();
hasfocus=true;
setFPS(35);
}

g_window::~g_window()
{
if(this->handle!=NULL)
{
SDL_DestroyWindow(this->handle);
}
gsound_device_free_all();
gspeak_unload();
this->handle=NULL;
this->renderer=NULL;
this->title="";
this->device=-1;
SDL_Quit();
}

bool g_window::hasFocus()const
{
return this->hasfocus;
}

void g_window::setTitle(const string& title)
{
if(this->handle!=NULL)
{
this->title=title;
SDL_SetWindowTitle(this->handle, title.c_str());
}
}

string g_window::getTitle()const
{
return this->title;
}

void g_window::setFPS(int32 fps)
{
if((fps<10)||(fps>120))
{
return;
}
this->fps=fps;
this->fps_interval=(fps/1000);
fps_start_time=get_timestamp_ms();
}

int32 g_window::getFPS()const
{
return this->fps;
}

int32 g_window::getDevice()const
{
return this->device;
}

void g_window::setGvol(int32 gvol)
{
BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, gvol);
}

int32 g_window::getGvol()const
{
return BASS_GetConfig(BASS_CONFIG_GVOL_STREAM);
}

bool g_window::setDevice(int32 d)
{
if(d==device) return true;
if((!gsound_device_started(d))&&(!gsound_device_init(d)))
{
return false;
}
if(!gsound_set_device(d))
{
return false;
}
this->device=d;
return true;
}

bool g_window::is_open()const
{
return this->handle!=NULL;
}

void g_window::close()
{
if(this->handle!=NULL)
{
    SDL_GL_DeleteContext(renderer);
SDL_DestroyWindow(this->handle);
}
this->handle=NULL;
this->renderer=NULL;
this->title="";
}

void g_window::show()
{
if(is_open())
{
SDL_ShowWindow(this->handle);
}
}

void g_window::hide()
{
if(is_open())
{
SDL_HideWindow(this->handle);
}
}

void g_window::make_loop()
{
if(!is_open())
{
return;
}
SDL_Event ev;
while(SDL_PollEvent(&ev)!=0)
{
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
switch(gkey_get_state(ev.key.keysym.sym))
{
case sk_up:
gkey_set_state(ev.key.keysym.sym, sk_pressed);
break;
case sk_pressed:
gkey_set_state(ev.key.keysym.sym, sk_down);
break;
default:
break;
}
break;
}
case SDL_KEYUP:
{
gkey_set_state(ev.key.keysym.sym, sk_up);
break;
}
case SDL_WINDOWEVENT:
{
switch(ev.window.event)
{
case SDL_WINDOWEVENT_FOCUS_GAINED:
{
hasfocus=true;
break;
}
case SDL_WINDOWEVENT_FOCUS_LOST:
{
hasfocus=false;
break;
}
}
break;
}
case SDL_TEXTINPUT:
{
break;
}
case SDL_TEXTEDITING:
case SDL_MOUSEMOTION:
case SDL_MOUSEBUTTONDOWN:
case SDL_MOUSEBUTTONUP:
case SDL_MOUSEWHEEL:
{
break;
}
}
}

int32 x=gsound_get_default_device_number();
if(this->getDevice()!=x)
{
this->setDevice(x);
}
int64 end=get_timestamp_ms();
if((end-fps_start_time)>=fps_interval)
{
        SDL_GL_SwapWindow(handle);
fps_start_time=end;
}
}

void g_window::setBackgroundColor(const vector3d& color)
{
glClearColor(color.x, color.y, color.z, 1.0f);
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

//Funções da janela principal...

g_window* createMainWindow(const std::string& title, int32 x, int32 y, int32 w, int32 h)
{
lock_guard<mutex> lck(mtx_window);
if(mainwindow!=NULL) return NULL;
mainwindow=new g_window(title, x, y, w, h);
return mainwindow;
}

void destroyMainWindow()
{
lock_guard<mutex> lck(mtx_window);
if(mainwindow!=NULL)
{
mainwindow->close();
delete mainwindow;
mainwindow=NULL;
}
}

g_window* getMainWindow()
{
lock_guard<mutex> lck(mtx_window);
return mainwindow;
}

bool mainWindowHasFocus()
{
lock_guard<mutex> lck(mtx_window);
return ((mainwindow==NULL) ? false : mainwindow->hasFocus());
}

bool mainWindowIsOpen()
{
lock_guard<mutex> lck(mtx_window);
return mainwindow!=NULL&&mainwindow->is_open();
}

void mainWindowShow()
{
g_window* window=getMainWindow();
if(window!=NULL)
{
window->show();
}
}

void mainWindowHide()
{
g_window* window=getMainWindow();
if(window!=NULL)
{
window->hide();
}
}
