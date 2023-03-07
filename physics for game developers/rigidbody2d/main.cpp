
#define _INCLUDE_GPP
#include<windowlib.h>
#include"rigidbody2d.h"

using namespace std;

//Janela...
#define G_TITLE "Main Window"
#define G_WINDOW_WIDTH 1500
#define G_WINDOW_HEIGHT 1500

#define _TIMESTEP 0.1f
float total_time=0.0f;
rigidbody2dlist bodies;
rigidbody2dlist obstacles;

void initialize(uint32 n);
void initobstacles();
void printAll(const string& msg="");
void updateSimulation(float dt);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
        setlocale(LC_ALL, "Portuguese");
g_window* window=createMainWindow(G_TITLE, 0,0, G_WINDOW_WIDTH, G_WINDOW_HEIGHT);
/*
for(int i=0; i<=1500; i+=250)
{
for(int i1=0; i1<=1500; i1+=250)
{
vector3d pos(i, i1);
gsound_play("sounds/beep.mp3", gsound_loop|gsound_3d, pos);
}
}
*/
vector3d spos={100, 100, 0};
uint32 handle= gsound_play("sounds/beep.mp3", gsound_loop|gsound_3d, spos);
initialize(1);
int64 timestep=get_timestamp_ms();
while(window->is_open())
{
wait_ms(5);
window->make_loop();
int64 end=get_timestamp_ms();
if(end-timestep>100)
{
updateSimulation(_TIMESTEP);
vector3d pos, vel, front, top;
pos=bodies[0]->vPosition;
vel=bodies[0]->vVelocity;
front.x=-cos(degrees_to_radians(bodies[0]->fOrientation));
front.z=-sin(degrees_to_radians(bodies[0]->fOrientation));
top.y=1.0f;
gsound_set_listener_position(&pos, NULL, &front, &top);
timestep=end;
vector3d v;
}
if(gkey_pressed('c'))
{
_speak("{:.0f}:{:.0f}", bodies[0]->vPosition.x, bodies[0]->vPosition.y);
}
if(gkey_pressed('t'))
{
vector3d v=bodies[0]->ThrustForce;
_speak("{:.0f}:{:.0f}", v.x, v.y);
}
if(gkey_pressed('o'))
{
_speak("{} graus", bodies[0]->fOrientation);
}
}
        return 0;
}

void initialize(uint32 n)
{
for(uint32 i=0; i<n; i++)
{
shared_rigidbody2d sf=make_shared<rigidbody2d>();
sf->fOrientation=90.0f;
sf->vPosition.x=50;
sf->vPosition.y=50;
bodies.push_back(sf);
}
}

void initobstacles()
{
}

void printAll(const string& msg)
{
if(msg.size()>0)
_FLOG("{}", msg);
for(auto& it : bodies)
{
_FLOG("{}", it->toString());
}
}

void updateSimulation(float dt)
{
total_time+=dt;
bodies[0]->setThrusters(false, false);
if(gkey_down(SDLK_UP)) bodies[0]->modulateThrust(true);
if(gkey_down(SDLK_DOWN)) bodies[0]->modulateThrust(false);
   if (gkey_down(SDLK_RIGHT))    bodies[0]->setThrusters(true, false);
   if (gkey_down(SDLK_LEFT))    bodies[0]->setThrusters(false, true);

for(auto& it : bodies)
{
it->updateBodyEuler(dt);
}
}
