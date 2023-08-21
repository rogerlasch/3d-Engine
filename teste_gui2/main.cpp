
#include<windowlib/windowlib.h>
#include<fmod/fmod.hpp>

using namespace std;
using namespace rgames;

#define G_TITLE "Main Window"
#define G_WINDOW_WIDTH 1500
#define G_WINDOW_HEIGHT 1500

void checkKeyboard();

rint64 timestep=0;
FMOD_VECTOR me;
FMOD_VECTOR sp;
    FMOD::System* hsystem;
    FMOD::Sound* sound;
    FMOD::Channel* channel;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
        setlocale(LC_ALL, "Portuguese");
//fmod
    FMOD_RESULT result = FMOD::System_Create(&hsystem);
    if (result != FMOD_OK) {
        return 1;
    }

    result = hsystem->init(32, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        return 1;
    }

    result = hsystem->createSound("music.mp3", FMOD_LOOP_NORMAL|FMOD_3D | FMOD_DEFAULT, nullptr, &sound);
    if (result != FMOD_OK) {
        return 1;
    }
me={0.0f, 0.0f, 0.0f};
sp={10.0f, 10.0f, 10.0f};
    hsystem->set3DListenerAttributes(0, &me, nullptr, nullptr, nullptr);
    result = hsystem->playSound(sound, nullptr, true, &channel);
    if (result != FMOD_OK) {
        return 1;
    }
    channel->set3DAttributes(&sp, nullptr);
    channel->setPaused(false);  // Iniciar a reprodução
//End fmod

r_window* window=createMainWindow(G_TITLE, 0,0, G_WINDOW_WIDTH, G_WINDOW_HEIGHT);
while(window->is_open())
{
this_thread::sleep_for(chrono::milliseconds(5));
window->update();
hsystem->update();
checkKeyboard();
}
    sound->release();
    hsystem->close();
    hsystem->release();
return 0;
}

void checkKeyboard()
{
if(rkey_pressed('c'))
{
_speak("{:.0f}:{:.0f}:{:.0f}", me.x, me.z, me.y);
}
if((rgames::get_timestamp_ms()-timestep)>=100)
{
timestep=rgames::get_timestamp_ms();
if(rkey_down(SDLK_UP)) me.z++;
else if(rkey_down(SDLK_DOWN)) me.z--;
else if(rkey_down(SDLK_RIGHT)) me.x++;
else if(rkey_down(SDLK_LEFT)) me.x--;
else if(rkey_down(SDLK_PAGEUP)) me.y++;
else if(rkey_down(SDLK_PAGEDOWN)) me.y--;

if(me.x>100) me.x=100;
if(me.x<0) me.x=0;
if(me.y>100) me.y=100;
if(me.y<0) me.y=0;
if(me.z>100) me.z=100;
if(me.z<0) me.z=0;
    hsystem->set3DListenerAttributes(0, &me, nullptr, nullptr, nullptr);
}
}
