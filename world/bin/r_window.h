


#ifndef R_WINDOW_H
#define R_WINDOW_H

#include<set>

enum r_key_state
{
sk_up=0,
sk_pressed,
sk_down
};

enum r_sound_flags
{
rsound_default=0,
rsound_autofree=1,
rsound_3d=2,
rsound_loop=4,
rsound_paused=8
};

class r_window
{
private:
gpp::int32 device;
std::string title;
std::set<gpp::int32> devices;
SDL_Window* handle;
SDL_Renderer* renderer;
public:
r_window(const std::string& title="Main window", gpp::uint32 x=0, gpp::uint32 y=0, gpp::uint32 width=600, gpp::uint32 height=600);
r_window(const r_window& rw)=delete;
r_window& operator=(const r_window& rw)=delete;
~r_window();
void setTitle(const std::string& title);
std::string getTitle()const;
bool setDevice(gpp::int32 device);
gpp::int32 getDevice()const;
void setGvol(gpp::int32 gvol);
gpp::int32 getGvol()const;
bool is_open()const;
void close();
void make_loop();
private:
void check_sounddevices();
};

bool key_pressed(gpp::int32 key_code);
bool key_down(gpp::int32 key_code);
bool key_up(gpp::int32 key_code);
gpp::uint32 rsound_play(const std::string& filename, gpp::uint32 flags=rsound_default);
gpp::uint32 rsound_play(const std::string& filename, gpp::uint32 flags, const gpp::vector3d& position);
bool rsound_close(gpp::uint32 handle);
bool rsound_set_listener_position(const gpp::vector3d& vect);
bool rsound_get_listener_position(gpp::vector3d& vect);
bool rsound_set_sound_position(gpp::uint32 handle, const gpp::vector3d& vect);
bool rsound_get_sound_position(gpp::uint32 handle, gpp::vector3d& vect);
#endif
