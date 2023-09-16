


#include<gl/glew.h>
#include<sstream>
#include<mutex>
#include<stdexcept>
#include<unordered_map>
#include<map>
#include<string>
#include"r_window.h"

using namespace std;

namespace rgames
{
static r_window* mainwindow=NULL;
static mutex mtx_window;

r_window::r_window(const string& title, ruint32 x, ruint32 y, ruint32 w, ruint32 h)
{
init_window(title, x, y, w, h);
}

r_window::~r_window()
{
if(is_open())
{
this->close();
}
rspeak_unload();
SDL_Quit();
this->handle=NULL;
this->renderer=NULL;
this->title="";
}

bool r_window::hasFocus()const
{
return this->hasfocus;
}

rsound* r_window::getSound()const
{
return hsound.get();
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

void r_window::setFPS(rint32 fps)
{
if((fps<10)||(fps>120))
{
return;
}
this->fps=fps;
this->fps_interval=(fps/1000);
fps_start_time=get_timestamp_ms();
}

rint32 r_window::getFPS()const
{
return this->fps;
}

void r_window::setKeyboardHandler(RKEYBOARDHANDLER hk)
{
this->hkeyboard=hk;
}

RKEYBOARDHANDLER r_window::getKeyboardHandler()const
{
return this->hkeyboard;
}

bool r_window::is_open()const
{
return this->handle!=NULL;
}

void r_window::close()
{
clearDialogs();
    SDL_GL_DeleteContext(renderer);
SDL_DestroyWindow(this->handle);
this->handle=NULL;
this->renderer=NULL;
this->title="";
}

void r_window::show()
{
if(is_open())
{
SDL_ShowWindow(this->handle);
}
}

void r_window::hide()
{
if(is_open())
{
SDL_HideWindow(this->handle);
}
}

void r_window::pushDialog(RdialogBase* rd)
{
if(hdialogs.size()>0)
{
ruint32 x=hdialogs.size()-1;
hdialogs[x]->enable(false);
}
hdialogs.push_back(rd);
rd->showModal();
}

void r_window::clearDialogs()
{
for(auto& it : hdialogs)
{
delete it;
}
hdialogs.clear();
}

void r_window::run()
{
while(is_open())
{
this_thread::sleep_for(chrono::milliseconds(5));
update();
}
}

void r_window::update()
{
update_gui();
}

void r_window::update_gui()
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
if(is_open())
{
this->close();
}
break;
}
case SDL_KEYDOWN:
{
switch(rkey_get_state(ev.key.keysym.sym))
{
case sk_up:
rkey_set_state(ev.key.keysym.sym, sk_pressed);
handleKeyboard(sk_pressed, ev.key.keysym.sym);
break;
case sk_pressed:
rkey_set_state(ev.key.keysym.sym, sk_down);
handleKeyboard(sk_down, ev.key.keysym.sym);
break;
case sk_down:
handleKeyboard(sk_down, ev.key.keysym.sym);
break;
default:
break;
}
break;
}
case SDL_KEYUP:
{
rkey_set_state(ev.key.keysym.sym, sk_up);
handleKeyboard(sk_up, ev.key.keysym.sym);
break;
}
case SDL_WINDOWEVENT:
{
switch(ev.window.event)
{
case SDL_WINDOWEVENT_CLOSE:
{
break;
}
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
/*
wstring str=string_utf8_decode(ev.text.text);
if(str.size()!=0)
{
}
ruint32 x=hdialogs.size();
if(x==0) break;
x--;
rtext* rt=static_cast<rtext*>(hdialogs[x]);
rt->append(str);
*/
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
rint64 end=get_timestamp_ms();
if((end-fps_start_time)>=fps_interval)
{
        SDL_GL_SwapWindow(handle);
fps_start_time=end;
}
}


void r_window::handleKeyboard(rint32 hstate, rint32 hkey)
{
if(hdialogs.size()==0)
{
if(hkeyboard)
{
hkeyboard(hstate, hkey);
}
}
else
{
ruint32 x=hdialogs.size()-1;
if(hdialogs[x]->isEnabled())
{
ruint32 result=hdialogs[x]->handleEvent(hstate, hkey);
switch(result)
{
case RD_CANCEL:
case RD_FINISH:
{
RdialogBase* hnext=NULL;
if(result==RD_FINISH)
{
hdialogs[x]->endModal();
hnext=hdialogs[x]->getNext();
}
delete hdialogs[x];
hdialogs.erase(hdialogs.begin()+x);
if(hnext!=NULL)
{
pushDialog(hnext);
}
else if(x>0)
{
x--;
hdialogs[x]->enable(true);
}
break;
}
}
}
}
}

//Métodos privados...

void r_window::init_window(const string& title, int x, int y, int w, int h)
{
this->title=title;
rspeak_init();
hsound=make_unique<rsound>();
rint32 res=SDL_Init(SDL_INIT_VIDEO);
if(res!=0)
{
throw runtime_error("Erro ao iniciar a sdl.");
}
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
handle = SDL_CreateWindow(title.c_str(), x, y, w, h, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!handle)
    {
SDL_Quit();
throw runtime_error("Erro ao criar a janela sdl.");
    }
renderer= SDL_GL_CreateContext(handle);
    if (!renderer)
    {
SDL_DestroyWindow(handle);
SDL_Quit();
throw runtime_error("Erro ao criar o contexto opengl.");
    }
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK)
    {
SDL_DestroyWindow(handle);
SDL_Quit();
throw runtime_error("Error to init GLEW!");
    }
glViewport(x, y, w, h);
hasfocus=true;
setFPS(35);
setKeyboardHandler(NULL);
}

//Funções da janela principal...

r_window* createMainWindow(const std::string& title, rint32 x, rint32 y, rint32 w, rint32 h)
{
lock_guard<mutex> lck(mtx_window);
if(mainwindow!=NULL) return NULL;
mainwindow=new r_window(title, x, y, w, h);
return mainwindow;
}

void destroyMainWindow()
{
lock_guard<mutex> lck(mtx_window);
if(mainwindow!=NULL)
{
if(mainwindow->is_open())
{
mainwindow->close();
}
delete mainwindow;
mainwindow=NULL;
}
}

r_window* getMainWindow()
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
r_window* window=getMainWindow();
if(window!=NULL)
{
window->show();
}
}

void mainWindowHide()
{
r_window* window=getMainWindow();
if(window!=NULL)
{
window->hide();
}
}

void notify_user(const std::string& s, const string& filename)
{
ralert* ra=new ralert(s, filename);
mainwindow->pushDialog(ra);
}

rint64 get_timestamp_ms()
{
chrono::system_clock::time_point tp=chrono::system_clock::now();
chrono::system_clock::duration dtn=tp.time_since_epoch();
return chrono::duration_cast<chrono::milliseconds>(dtn).count();
}

}
