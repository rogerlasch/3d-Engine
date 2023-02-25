

#ifndef R_WINDOW_H
#define R_WINDOW_H

#include <vector>
#include <string>
#include <set>
#include <sdl2/sdl.h>
#include"..\\..\\gpp\\gpp.h"
#include"..\\..\\gpp_physics\\gpp_physics.h"
using namespace gpp;

enum r_key_state
{
    sk_up = 0,
    sk_pressed,
    sk_down
};

enum r_sound_flags
{
    rsound_default = 0,
    rsound_autofree = 1,
    rsound_3d = 2,
    rsound_loop = 4,
    rsound_paused = 8
};

using namespace gpp;
typedef std::vector<gpp::vector3d> POLYGON;
class r_window
{
private:
    std::string title;
    SDL_Window *handle;
SDL_GLContext* renderer;

public:
    r_window(const std::string &title = "Main window", int x = 0, int y = 0, int width = 600, int height = 600);
    r_window(const r_window &rw) = delete;
    r_window &operator=(const r_window &rw) = delete;
    ~r_window();
    void setTitle(const std::string &title);
    std::string getTitle() const;
    bool is_open() const;
    void close();
    void make_loop();
    void clear(unsigned char r, unsigned char g, unsigned char b);
    void setBackGroundColor(unsigned char r, unsigned char g, unsigned char b);
void drawBox(box3d* box, uint8 r, uint8 g, uint8 b);
void drawCircle(sphere3d* s, uint8 r, uint8 g, uint8 b);
};

bool key_pressed(int key_code);
bool key_down(int key_code);
bool key_up(int key_code);
#endif
