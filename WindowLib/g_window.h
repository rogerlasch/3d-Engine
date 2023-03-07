


#ifndef G_WINDOW_H
#define G_WINDOW_H

#include<gtypes.h>
using namespace gpp;
#include<set>
#include<format>
#include<sdl2/sdl.h>
#include <SDL2/SDL_opengl.h>
#include"..\\gpp_physics\\geometry\\vector3d.h"
#include"gsound.h"
#include"gkeyboard.h"
#include"gspeak.h"

class g_window
{
private:
int32 device;
int32 fps;
int32 fps_interval;
int64 fps_start_time;
bool hasfocus;
std::string title;
std::set<int32> devices;
SDL_Window* handle;
SDL_GLContext renderer;
public:
g_window(const std::string& title="Main window", uint32 x=0, uint32 y=0, uint32 width=600, uint32 height=600);
g_window(const g_window& rw)=delete;
g_window& operator=(const g_window& rw)=delete;
~g_window();
bool hasFocus()const;
void setTitle(const std::string& title);
std::string getTitle()const;
void setFPS(int32 fps);
int32 getFPS()const;
bool setDevice(int32 device);
int32 getDevice()const;
void setGvol(int32 gvol);
int32 getGvol()const;
bool is_open()const;
void close();
void show();
void hide();
void make_loop();
void setBackgroundColor(const vector3d& color={0.0f, 0.0f, 0.0f});
};

g_window* createMainWindow(const std::string& title, int32 x, int32 y, int32 w, int32 h);
void destroyMainWindow();
g_window* getMainWindow();
bool mainWindowHasFocus();
bool mainWindowIsOpen();
void mainWindowShow();
void mainWindowHide();
#endif
