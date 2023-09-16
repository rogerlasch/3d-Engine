


#ifndef R_WINDOW_H
#define R_WINDOW_H

#include<set>
#include<format>
#include<functional>
#include<sdl2/sdl.h>
#include <SDL2/SDL_opengl.h>
#include"r_types.h"
#include"rstring_utils.h"
#include"rkeyboard.h"
#include"rspeak.h"
#include"windialogs.h"
#include"rsound.h"

namespace rgames
{

typedef std::function<void(rint32, rint32)> RKEYBOARDHANDLER;
class r_window
{
private:
rint32 device;
rint32 fps;
rint32 fps_interval;
rint64 fps_start_time;
bool hasfocus;
std::string title;
SDL_Window* handle;
SDL_GLContext renderer;
RKEYBOARDHANDLER hkeyboard;
std::vector<RdialogBase*> hdialogs;
std::unique_ptr<rsound> hsound;
public:
r_window(const std::string& title="Main window", ruint32 x=0, ruint32 y=0, ruint32 width=600, ruint32 height=600);
r_window(const r_window& rw)=delete;
r_window& operator=(const r_window& rw)=delete;
~r_window();
bool hasFocus()const;
rsound* getSound()const;
void setTitle(const std::string& title);
std::string getTitle()const;
void setFPS(rint32 fps);
rint32 getFPS()const;
void setKeyboardHandler(RKEYBOARDHANDLER hk);
RKEYBOARDHANDLER getKeyboardHandler()const;
bool is_open()const;
void close();
void show();
void hide();
void pushDialog(RdialogBase* rd);
void clearDialogs();
void run();
void update();
void update_gui();
void handleKeyboard(rint32 hstate, rint32 hkey);
private:
void init_window(const std::string& title, int x, int y, int w, int h);
};

r_window* createMainWindow(const std::string& title, rint32 x, rint32 y, rint32 w, rint32 h);
void destroyMainWindow();
r_window* getMainWindow();
bool mainWindowHasFocus();
bool mainWindowIsOpen();
void mainWindowShow();
void mainWindowHide();

void notify_user(const std::string& s, const std::string& filename="");
rint64 get_timestamp_ms();
}
#endif
